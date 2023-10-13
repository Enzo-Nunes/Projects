package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.InvalidSpanException;

public abstract class FunctionValue extends CellValue {
	protected ValueWrapper _bufferedResult;
	protected boolean _resultIsBuffered;

	@Override
	public ValueWrapper getValue() throws IncorrectValueTypeException, InvalidSpanException {
		if (_resultIsBuffered)
			return _bufferedResult;

		recalculate();
		return _bufferedResult;
	}
}
