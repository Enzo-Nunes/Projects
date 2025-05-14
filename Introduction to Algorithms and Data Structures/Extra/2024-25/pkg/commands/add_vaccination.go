package commands

import (
	"IAED2025/pkg/logger"
	"IAED2025/pkg/system"
	"fmt"
)

func handleAddVaccination(vaccineSystem *system.VaccineSystem, args []string) error {

	userName := args[0]
	vaccineName := args[1]

	if !vaccineSystem.HasStock(vaccineName) {
		return vaccineSystem.Logger.Error(logger.ErrNoStock)
	}

	if vaccineSystem.AlreadyVaccinatedToday(userName, vaccineName) {
		return vaccineSystem.Logger.Error(logger.ErrAlreadyVaccinated)
	}

	vaccination := system.NewVaccination(userName, vaccineSystem.GetNextBatch(vaccineName), vaccineSystem.CurrentDate)
	vaccineSystem.AddVaccination(userName, vaccination)
	fmt.Println(vaccination.Batch.ID)
	return nil
}
