package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.PositionOutOfRangeException;

abstract class CellValue {
	abstract public ValueWrapper getValue() throws IncorrectValueTypeException, PositionOutOfRangeException;
	abstract protected void recalculate() throws IncorrectValueTypeException, PositionOutOfRangeException; //for future optimization
	abstract public CellValue deepCopy();
	abstract public String visualize();
}
