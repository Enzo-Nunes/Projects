package commands

import (
	"IAED2025/pkg/logger"
	"IAED2025/pkg/system"
	"fmt"
)

func handleRemoveBatch(vaccineSystem *system.VaccineSystem, args []string) error {
	BatchID := args[0]
	batch, exists := vaccineSystem.BatchIDBatches[BatchID];
	if !exists {
		return vaccineSystem.Logger.Error(logger.ErrNoSuchBatch, BatchID)
	}

	fmt.Println(batch.DosesApplied)
	vaccineSystem.RemoveBatch(BatchID)
	return nil
}