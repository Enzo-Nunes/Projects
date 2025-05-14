package commands

import (
	"IAED2025/pkg/logger"
	"IAED2025/pkg/system"
)

func handleListVaccinations(vaccineSystem *system.VaccineSystem, args []string) error {

	if len(args) == 0 {
		system.PrintlnVaccinations(vaccineSystem.Vaccinations)
		return nil
	}

	userName := args[0]
	if vaccinations, exists := vaccineSystem.UserVaccinations[userName]; !exists || len(vaccinations) == 0 {
		return vaccineSystem.Logger.Error(logger.ErrNoSuchUser, userName)
	}
	system.PrintlnVaccinations(vaccineSystem.UserVaccinations[userName])

	return nil
}
