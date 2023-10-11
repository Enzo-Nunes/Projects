package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;

public class AddFunction extends BinaryFunction {
	
	public AddFunction(BinaryArgument firstArg, BinaryArgument secondArg)
	{
		super(firstArg, secondArg);
	}

	@Override
	void recalculate() throws IncorrectValueTypeException {
		_bufferedResult = new ValueWrapper(_arg1.getValue() + _arg2.getValue());
	}

	@Override
	CellValue deepCopy() {
		return new AddFunction(_arg1.deepCopy(), _arg2.deepCopy());
	}
}
