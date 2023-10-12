package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;

public abstract class CellValue {
	abstract ValueWrapper getValue() throws IncorrectValueTypeException;
	abstract void recalculate() throws IncorrectValueTypeException; //for future optimization
	abstract CellValue deepCopy();
}
