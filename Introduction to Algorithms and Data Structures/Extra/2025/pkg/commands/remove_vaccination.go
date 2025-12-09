package commands

import (
	"IAED2025/pkg/logger"
	"IAED2025/pkg/parser"
	"IAED2025/pkg/system"
	"fmt"
)

func handleRemoveVaccination(vaccineSystem *system.VaccineSystem, args []string) error {
	userName := args[0]
	if _, exists := vaccineSystem.UserVaccinations[userName]; !exists {
		return vaccineSystem.Logger.Error(logger.ErrNoSuchUser, userName)
	}
	if len(args) == 1 {
		fmt.Println(vaccineSystem.RemoveVaccinations(userName))
		return nil
	}

	date, err := parser.ParseDate(vaccineSystem, args[1])
	if err != nil {
		return err
	}
	if date.After(vaccineSystem.CurrentDate) {
		return vaccineSystem.Logger.Error(logger.ErrInvalidDate)
	}
	if len(args) == 2 {
		fmt.Println(vaccineSystem.RemoveVaccinationsWithFilter(
			userName,
			func(v *system.Vaccination) bool {
				return v.AppliedDate.Equal(date)
			},
		))
		return nil
	}

	BatchID := args[2]
	batch, exists := vaccineSystem.BatchIDBatches[BatchID]
	if !exists {
		return vaccineSystem.Logger.Error(logger.ErrNoSuchBatch, BatchID)
	}
	fmt.Println(vaccineSystem.RemoveVaccinationsWithFilter(
		userName,
		func(v *system.Vaccination) bool {
			return v.AppliedDate.Equal(date) && v.Batch == batch
		},
	))
	return nil

}
