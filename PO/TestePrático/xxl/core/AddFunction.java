package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.PositionOutOfRangeException;

class AddFunction extends BinaryFunction {

	public AddFunction(BinaryArgument firstArg, BinaryArgument secondArg) {
		super(firstArg, secondArg);
		recalculate();
	}

	@Override
	protected void recalculate() {
		try {
			_bufferedResult = new ValueWrapper(_arg1.getValue() + _arg2.getValue());
		} catch (IncorrectValueTypeException | PositionOutOfRangeException | NullPointerException e) {
			_bufferedResult = null;
		}
	}

	@Override
	public CellValue deepCopy() {
		return new AddFunction(_arg1.deepCopy(), _arg2.deepCopy());
	}

	@Override
	public String visualize() {
		String resultStr;

		if (_bufferedResult == null)
			resultStr = "#VALUE";
		else
			resultStr = _bufferedResult.visualize();

		return resultStr + "=ADD(" + _arg1.visualize() + "," + _arg2.visualize() + ")";
	}

	@Override
	public String getFunctionName() {
		return "ADD";
	}
}
