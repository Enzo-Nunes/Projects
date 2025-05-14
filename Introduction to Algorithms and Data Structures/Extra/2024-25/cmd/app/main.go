package main

import (
	"IAED2025/pkg/commands"
	"IAED2025/pkg/system"
	"io"
	"os"
)

func main() {

	var language string
	if len(os.Args) > 1 {
		language = os.Args[1]
	}
	vaccineSystem := system.NewVaccineSystem(language)

	if err := commands.ProcessCommands(vaccineSystem); err != nil {
		if err == io.EOF {
			return
		}
		panic(err)
	}
}
