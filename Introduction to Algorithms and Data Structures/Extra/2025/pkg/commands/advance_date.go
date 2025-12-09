package commands

import (
	"IAED2025/pkg/logger"
	"IAED2025/pkg/parser"
	"IAED2025/pkg/system"
	"fmt"
)

func handleAdvanceDate(vaccineSystem *system.VaccineSystem, args []string) error {

	date, err := parser.ParseDate(vaccineSystem, args[0])
	if err != nil {
		return err
	}

	if date.Before(vaccineSystem.CurrentDate) {
		return vaccineSystem.Logger.Error(logger.ErrInvalidDate)
	}

	vaccineSystem.CurrentDate = date
	fmt.Println(date.Format(system.DateOutputFormat))
	return nil
}
