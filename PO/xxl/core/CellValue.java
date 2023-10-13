package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.InvalidSpanException;

abstract class CellValue {
	abstract public ValueWrapper getValue() throws IncorrectValueTypeException, InvalidSpanException;
	abstract protected void recalculate() throws IncorrectValueTypeException, InvalidSpanException; //for future optimization
	abstract public CellValue deepCopy();
	abstract public String visualize();
}
