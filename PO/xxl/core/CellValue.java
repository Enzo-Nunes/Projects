package xxl.core;

import java.io.Serializable;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.PositionOutOfRangeException;

abstract class CellValue implements Serializable {
	abstract public ValueWrapper getValue() throws IncorrectValueTypeException, PositionOutOfRangeException;

	abstract protected void recalculate();

	abstract public CellValue deepCopy();

	abstract public String visualize();
}
