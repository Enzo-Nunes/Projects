package system

import (
	"fmt"
	"time"
)

type Batch struct {
	ID             string
	VaccineName    string
	ExpiryDate     time.Time
	DosesRemaining int
	DosesApplied   int
}

func NewBatch(id string, expiryDate time.Time, dosesRemaining int, vaccineName string) *Batch {
	return &Batch{
		ID:             id,
		VaccineName:    vaccineName,
		ExpiryDate:     expiryDate,
		DosesRemaining: dosesRemaining,
		DosesApplied:   0,
	}
}

func insertBatchSorted(batches []*Batch, newBatch *Batch) []*Batch {
	index := 0
	for i, batch := range batches {
		if newBatch.ExpiryDate.Before(batch.ExpiryDate) {
			break
		}
		if newBatch.ExpiryDate.Equal(batch.ExpiryDate) && newBatch.ID < batch.ID {
			break
		}
		index = i + 1
	}

	return append(batches[:index], append([]*Batch{newBatch}, batches[index:]...)...)
}

func (vb *Batch) Println() {
	fmt.Println(vb.VaccineName, vb.ID, vb.ExpiryDate.Format(DateOutputFormat), vb.DosesRemaining, vb.DosesApplied)
}

func PrintlnBatches(batches []*Batch) {
	for _, batch := range batches {
		batch.Println()
	}
}
