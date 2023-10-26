package xxl.core;

import java.io.Serializable;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.InvalidSpanException;

abstract class CellValue implements Serializable {
	abstract public ValueWrapper getValue() throws IncorrectValueTypeException, InvalidSpanException;

	abstract protected void recalculate();

	abstract public CellValue deepCopy();

	abstract public String visualize();
}
