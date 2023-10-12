package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;

public class SubFunction extends BinaryFunction {

	public SubFunction(BinaryArgument firstArg, BinaryArgument secondArg) {
		super(firstArg, secondArg);
	}

	@Override
	void recalculate() throws IncorrectValueTypeException {
		_bufferedResult = new ValueWrapper(_arg1.getValue() - _arg2.getValue());
	}

	@Override
	CellValue deepCopy() {
		return new SubFunction(_arg1, _arg2);
	}
}
