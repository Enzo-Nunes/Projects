package commands

import (
	"IAED2025/pkg/logger"
	"IAED2025/pkg/parser"
	"IAED2025/pkg/system"
	"fmt"
)

func handleAddBatch(vaccineSystem *system.VaccineSystem, args []string) error {

	if vaccineSystem.TotalBatches >= system.MaxVaccineBatches {
		return vaccineSystem.Logger.Error(logger.ErrTooManyVaccines)
	}

	batchID, err := parser.ParseBatchID(vaccineSystem, args[0])
	if err != nil {
		return err
	}

	if _, exists := vaccineSystem.BatchIDBatches[batchID]; exists {
		return vaccineSystem.Logger.Error(logger.ErrDuplicateBatchID)
	}

	date, err := parser.ParseDate(vaccineSystem, args[1])
	if err != nil {
		return err
	}

	if date.Before(vaccineSystem.CurrentDate) {
		return vaccineSystem.Logger.Error(logger.ErrInvalidDate)
	}

	quantity, err := parser.ParseQuantity(vaccineSystem, args[2])
	if err != nil {
		return err
	}

	vaccineName, err := parser.ParseVaccineName(vaccineSystem, args[3])
	if err != nil {
		return err
	}

	batch := system.NewBatch(batchID, date, quantity, vaccineName)
	vaccineSystem.AddBatch(batch)
	fmt.Println(batchID)
	return nil
}
