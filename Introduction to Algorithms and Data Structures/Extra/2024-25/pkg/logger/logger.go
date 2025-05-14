package logger

import "fmt"

type Logger struct {
	error_messages map[string]string
}

func NewLogger(language string) *Logger {
	switch language {
	case LanguagePortuguese:
		return &Logger{
			error_messages: map[string]string{
				ErrTooManyVaccines:    PortugueseErrTooManyVaccines,
				ErrDuplicateBatchID:   PortugueseErrDuplicateBatchID,
				ErrInvalidBatchID:     PortugueseErrInvalidBatchID,
				ErrInvalidVaccineName: PortugueseErrInvalidVaccineName,
				ErrInvalidDate:        PortugueseErrInvalidDate,
				ErrInvalidQuantity:    PortugueseErrInvalidQuantity,
				ErrNoSuchVaccine:      PortugueseErrNoSuchVaccine,
				ErrNoStock:            PortugueseErrNoStock,
				ErrAlreadyVaccinated:  PortugueseErrAlreadyVaccinated,
				ErrNoSuchBatch:        PortugueseErrNoSuchBatch,
				ErrNoSuchUser:         PortugueseErrNoSuchUser,
			},
		}

	case LanguageEnglish:
		return &Logger{
			error_messages: map[string]string{
				ErrTooManyVaccines:    EnglishErrTooManyVaccines,
				ErrDuplicateBatchID:   EnglishErrDuplicateBatchID,
				ErrInvalidBatchID:     EnglishErrInvalidBatchID,
				ErrInvalidVaccineName: EnglishErrInvalidVaccineName,
				ErrInvalidDate:        EnglishErrInvalidDate,
				ErrInvalidQuantity:    EnglishErrInvalidQuantity,
				ErrNoSuchVaccine:      EnglishErrNoSuchVaccine,
				ErrNoStock:            EnglishErrNoStock,
				ErrAlreadyVaccinated:  EnglishErrAlreadyVaccinated,
				ErrNoSuchBatch:        EnglishErrNoSuchBatch,
				ErrNoSuchUser:         EnglishErrNoSuchUser,
			},
		}
	default:
		return &Logger{
			error_messages: map[string]string{
				ErrTooManyVaccines:    EnglishErrTooManyVaccines,
				ErrDuplicateBatchID:   EnglishErrDuplicateBatchID,
				ErrInvalidBatchID:     EnglishErrInvalidBatchID,
				ErrInvalidVaccineName: EnglishErrInvalidVaccineName,
				ErrInvalidDate:        EnglishErrInvalidDate,
				ErrInvalidQuantity:    EnglishErrInvalidQuantity,
				ErrNoSuchVaccine:      EnglishErrNoSuchVaccine,
				ErrNoStock:            EnglishErrNoStock,
				ErrAlreadyVaccinated:  EnglishErrAlreadyVaccinated,
				ErrNoSuchBatch:        EnglishErrNoSuchBatch,
				ErrNoSuchUser:         EnglishErrNoSuchUser,
			},
		}
	}
}

func (l *Logger) Error(err_key string, args ...any) error {
	err_msg := l.error_messages[err_key]
	return fmt.Errorf(err_msg, args...)
}
