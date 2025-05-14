package commands

import (
	"IAED2025/pkg/logger"
	"IAED2025/pkg/system"
	"fmt"
)

func handleListVaccines(vaccineSystem *system.VaccineSystem, args []string) error {

	if len(args) == 0 {
		system.PrintlnBatches(vaccineSystem.Batches)
		return nil
	}

	for _, vaccine := range args {
		if batches, exists := vaccineSystem.VaccineBatches[vaccine]; !exists || len(batches) == 0 {
			fmt.Println(vaccineSystem.Logger.Error(logger.ErrNoSuchVaccine, vaccine))
		}
		system.PrintlnBatches(vaccineSystem.VaccineBatches[vaccine])
	}
	return nil

}
