package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;

public class DivFunction extends BinaryFunction {

	public DivFunction(BinaryArgument firstArg, BinaryArgument secondArg) {
		super(firstArg, secondArg);
	}

	@Override
	void recalculate() throws IncorrectValueTypeException {
		_bufferedResult = new ValueWrapper(_arg1.getValue() / _arg2.getValue());
	}

	@Override
	CellValue deepCopy() {
		return new DivFunction(_arg1, _arg2);
	}
}
