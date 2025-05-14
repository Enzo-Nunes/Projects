package logger

// Available Languages
const (
	LanguageEnglish    = "en"
	LanguagePortuguese = "pt"
)

// Error message keys
const (
	ErrTooManyVaccines    = "ErrTooManyVaccines"
	ErrDuplicateBatchID   = "ErrDuplicateBatch"
	ErrInvalidBatchID     = "ErrInvalidBatchID"
	ErrInvalidVaccineName = "ErrInvalidVaccineName"
	ErrInvalidDate        = "ErrInvalidDate"
	ErrInvalidQuantity    = "ErrInvalidQuantity"
	ErrNoSuchVaccine      = "ErrNoSuchVaccine"
	ErrNoStock            = "ErrNoStock"
	ErrAlreadyVaccinated  = "ErrAlreadyVaccinated"
	ErrNoSuchBatch        = "ErrNoSuchBatch"
	ErrNoSuchUser         = "ErrNoSuchUser"
)

// English Error strings
const (
	EnglishErrTooManyVaccines    = "too many vaccines"
	EnglishErrDuplicateBatchID   = "duplicate batch number"
	EnglishErrInvalidBatchID     = "invalid batch"
	EnglishErrInvalidVaccineName = "invalid name"
	EnglishErrInvalidDate        = "invalid date"
	EnglishErrInvalidQuantity    = "invalid quantity"
	EnglishErrNoSuchVaccine      = "%s: no such vaccine"
	EnglishErrNoStock            = "no stock"
	EnglishErrAlreadyVaccinated  = "already vaccinated"
	EnglishErrNoSuchBatch        = "%s: no such batch"
	EnglishErrNoSuchUser         = "%s: no such user"
)

// Portuguese Error strings
const (
	PortugueseErrTooManyVaccines    = "demasiadas vacinas"
	PortugueseErrDuplicateBatchID   = "número de lote duplicado"
	PortugueseErrInvalidBatchID     = "lote inválido"
	PortugueseErrInvalidVaccineName = "nome inválido"
	PortugueseErrInvalidDate        = "data inválida"
	PortugueseErrInvalidQuantity    = "quantidade inválida"
	PortugueseErrNoSuchVaccine      = "%s: vacina inexistente"
	PortugueseErrNoStock            = "esgotado"
	PortugueseErrAlreadyVaccinated  = "já vacinado"
	PortugueseErrNoSuchBatch        = "%s: lote inexistente"
	PortugueseErrNoSuchUser         = "%s: utente inexistente"
)
