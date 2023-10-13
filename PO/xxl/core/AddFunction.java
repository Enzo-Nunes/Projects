package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.PositionOutOfRangeException;

class AddFunction extends BinaryFunction {

	public AddFunction(BinaryArgument firstArg, BinaryArgument secondArg) {
		super(firstArg, secondArg);
	}

	@Override
	protected void recalculate() throws IncorrectValueTypeException, PositionOutOfRangeException {
		_bufferedResult = new ValueWrapper(_arg1.getValue() + _arg2.getValue());
	}

	@Override
	public CellValue deepCopy() {
		return new AddFunction(_arg1.deepCopy(), _arg2.deepCopy());
	}

	@Override
	public String visualize()
	{
		String resultStr;
		try
		{
			recalculate();
			resultStr = _bufferedResult.visualize();
		} catch (IncorrectValueTypeException | PositionOutOfRangeException e) {
			resultStr = "#VALUE";
		}
		
		return resultStr + "=ADD(" + _arg1.visualize() + "," + _arg2.visualize() + ")";
	}
}
