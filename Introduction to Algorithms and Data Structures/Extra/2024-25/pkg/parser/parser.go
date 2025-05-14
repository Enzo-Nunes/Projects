package parser

import (
	"IAED2025/pkg/logger"
	"IAED2025/pkg/system"
	"strconv"
	"strings"
	"time"
)

func ParseBatchID(vaccineSystem *system.VaccineSystem, batchID string) (string, error) {
	if len(batchID) > system.MaxVaccineBatchIDLength {
		return "", vaccineSystem.Logger.Error(logger.ErrInvalidBatchID)
	}
	for _, char := range batchID {
		if !(char >= '0' && char <= '9') && !(char >= 'A' && char <= 'F') {
			return "", vaccineSystem.Logger.Error(logger.ErrInvalidBatchID)
		}
	}
	return batchID, nil
}

func ParseDate(vaccineSystem *system.VaccineSystem, dateStr string) (time.Time, error) {
	date, err := time.Parse(system.DateInputFormat, dateStr)
	if err != nil {
		return time.Time{}, vaccineSystem.Logger.Error(logger.ErrInvalidDate)
	}
	return date, nil
}

func ParseVaccineName(vaccineSystem *system.VaccineSystem, vaccineName string) (string, error) {
	if len(vaccineName) > system.MaxVaccineNameLength {
		return "", vaccineSystem.Logger.Error(logger.ErrInvalidVaccineName)
	}
	if strings.ContainsAny(vaccineName, " \t\n") {
		return "", vaccineSystem.Logger.Error(logger.ErrInvalidVaccineName)
	}
	return vaccineName, nil
}

func ParseQuantity(vaccineSystem *system.VaccineSystem, quantityStr string) (int, error) {
	quantity, err := strconv.Atoi(quantityStr)
	if err != nil || quantity <= 0 {
		return 0, vaccineSystem.Logger.Error(logger.ErrInvalidQuantity)
	}
	return quantity, nil
}

func ParseCommandLine(commandLine string) []string {
	var parts []string
	var current strings.Builder
	inQuotes := false

	for _, char := range commandLine {
		switch char {
		case '"':
			inQuotes = !inQuotes
		case ' ', '\t', '\n':
			if inQuotes {
				current.WriteRune(char)
			} else if current.Len() > 0 {
				parts = append(parts, current.String())
				current.Reset()
			}
		default:
			current.WriteRune(char)
		}
	}

	if current.Len() > 0 {
		parts = append(parts, current.String())
	}

	return parts
}
