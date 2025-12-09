package system

import (
	"IAED2025/pkg/logger"
	"slices"
	"time"
)

type VaccineSystem struct {
	BatchIDBatches   map[string]*Batch
	VaccineBatches   map[string][]*Batch
	Batches          []*Batch
	UserVaccinations map[string][]*Vaccination
	Vaccinations     []*Vaccination
	TotalBatches     int
	CurrentDate      time.Time
	Logger           *logger.Logger
}

func NewVaccineSystem(language string) *VaccineSystem {
	return &VaccineSystem{
		BatchIDBatches:   make(map[string]*Batch),
		VaccineBatches:   make(map[string][]*Batch),
		Batches:          make([]*Batch, 0),
		UserVaccinations: make(map[string][]*Vaccination),
		Vaccinations:     make([]*Vaccination, 0),
		TotalBatches:     0,
		CurrentDate:      time.Date(2025, 1, 1, 0, 0, 0, 0, time.UTC),
		Logger:           logger.NewLogger(language),
	}
}

func (vs *VaccineSystem) AddBatch(batch *Batch) {
	vs.BatchIDBatches[batch.ID] = batch
	vs.VaccineBatches[batch.VaccineName] = insertBatchSorted(vs.VaccineBatches[batch.VaccineName], batch)
	vs.Batches = insertBatchSorted(vs.Batches, batch)
	vs.TotalBatches++
}

func (vs *VaccineSystem) RemoveBatch(batchID string) {
	batch := vs.BatchIDBatches[batchID]

	if batch.DosesApplied > 0 {
		batch.DosesRemaining = 0
		return
	}

	vs.Batches = slices.DeleteFunc(
		vs.Batches,
		func(b *Batch) bool {
			return b.ID == batchID
		})
	vs.VaccineBatches[batch.VaccineName] = slices.DeleteFunc(
		vs.VaccineBatches[batch.VaccineName],
		func(b *Batch) bool {
			return b.ID == batchID
		})
	delete(vs.BatchIDBatches, batchID)
	vs.TotalBatches--
}

func (vs *VaccineSystem) GetAllVaccines() []string {
	vaccines := make([]string, 0, len(vs.VaccineBatches))
	for vaccine := range vs.VaccineBatches {
		vaccines = append(vaccines, vaccine)
	}
	return vaccines
}

func (vs *VaccineSystem) HasStock(vaccineName string) bool {
	batches, exists := vs.VaccineBatches[vaccineName]
	if !exists || len(batches) == 0 {
		return false
	}
	for _, batch := range batches {
		if batch.DosesRemaining > 0 && !batch.ExpiryDate.Before(vs.CurrentDate) {
			return true
		}
	}
	return false
}

func (vs *VaccineSystem) AlreadyVaccinatedToday(userName string, vaccineName string) bool {
	vaccinations, exists := vs.UserVaccinations[userName]
	if !exists || len(vaccinations) == 0 {
		return false
	}
	for _, vaccination := range vaccinations {
		if vaccination.Batch.VaccineName == vaccineName && vaccination.AppliedDate.Equal(vs.CurrentDate) {
			return true
		}
	}
	return false
}

func (vs *VaccineSystem) GetNextBatch(vaccineName string) *Batch {
	for _, batch := range vs.VaccineBatches[vaccineName] {
		if batch.DosesRemaining > 0 && !batch.ExpiryDate.Before(vs.CurrentDate) {
			return batch
		}
	}
	return nil
}

func (vs *VaccineSystem) AddVaccination(userName string, vaccination *Vaccination) {
	vs.UserVaccinations[userName] = append(vs.UserVaccinations[userName], vaccination)
	vs.Vaccinations = append(vs.Vaccinations, vaccination)
	vaccination.Batch.DosesApplied++
	vaccination.Batch.DosesRemaining--
}

func (vs *VaccineSystem) RemoveVaccinations(userName string) int {
	removed := len(vs.UserVaccinations[userName])
	delete(vs.UserVaccinations, userName)
	return removed
}

func (vs *VaccineSystem) RemoveVaccinationsWithFilter(userName string, filter func(*Vaccination) bool) int {
	vaccinations, exists := vs.UserVaccinations[userName]
	if !exists || len(vaccinations) == 0 {
		return 0
	}
	var removed int
	vs.UserVaccinations[userName] = slices.DeleteFunc(
		vaccinations,
		func(v *Vaccination) bool {
			if filter(v) {
				removed++
				return true
			}
			return false
		},
	)
	return removed
}
