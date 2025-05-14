package system

import (
	"fmt"
	"time"
)

type Vaccination struct {
	UserName    string
	Batch       *Batch
	AppliedDate time.Time
}

func NewVaccination(userName string, batch *Batch, appliedDate time.Time) *Vaccination {
	return &Vaccination{
		UserName:    userName,
		Batch:       batch,
		AppliedDate: appliedDate,
	}
}

func (v *Vaccination) Println() {
	fmt.Println(v.UserName, v.Batch.ID, v.AppliedDate.Format(DateOutputFormat))
}

func PrintlnVaccinations(vaccinations []*Vaccination) {
	for _, vaccination := range vaccinations {
		vaccination.Println()
	}
}
