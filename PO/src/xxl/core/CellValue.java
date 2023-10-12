package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.PositionOutOfRangeException;

public abstract class CellValue {
	abstract ValueWrapper getValue() throws IncorrectValueTypeException, PositionOutOfRangeException;
	abstract void recalculate() throws IncorrectValueTypeException, PositionOutOfRangeException; //for future optimization
	abstract CellValue deepCopy();
}
