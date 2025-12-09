package commands

import (
	"IAED2025/pkg/parser"
	"IAED2025/pkg/system"
	"bufio"
	"fmt"
	"io"
	"os"
	"strings"
)

// ProcessCommands listens for user input and processes commands until 'q' is entered.
func ProcessCommands(vaccineSystem *system.VaccineSystem) error {
	scanner := bufio.NewScanner(os.Stdin)
	for {
		if !scanner.Scan() {
			break // Exit if input stream is closed
		}

		// Read and process the command
		command := strings.TrimSpace(scanner.Text())
		if err := handleCommand(vaccineSystem, command); err != nil {
			return err
		}
	}
	return nil
}

// handleCommand processes a single command with arguments.
func handleCommand(vaccineSystem *system.VaccineSystem, commandLine string) error {

	parts := parser.ParseCommandLine(commandLine)
	if len(parts) == 0 {
		return nil // Ignore empty lines
	}

	command := parts[0]

	var err error
	switch command {
	case CommandQuit:
		return io.EOF
	case CommandNewBatch:
		err = handleAddBatch(vaccineSystem, parts[1:])
	case CommandListVaccines:
		err = handleListVaccines(vaccineSystem, parts[1:])
	case CommandAdvanceDate:
		err = handleAdvanceDate(vaccineSystem, parts[1:])
	case CommandVaccinate:
		err = handleAddVaccination(vaccineSystem, parts[1:])
	case CommandListVaccinations:
		err = handleListVaccinations(vaccineSystem, parts[1:])
	case CommandRemoveBatch:
		err = handleRemoveBatch(vaccineSystem, parts[1:])
	case CommandRemoveVaccination:
		err = handleRemoveVaccination(vaccineSystem, parts[1:])
	}
	if err != nil {
		fmt.Println(err)
	}
	return nil
}
