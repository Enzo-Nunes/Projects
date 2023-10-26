package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.PositionOutOfRangeException;

public class MulFunction extends BinaryFunction {

	public MulFunction(BinaryArgument firstArg, BinaryArgument secondArg) {
		super(firstArg, secondArg);
		recalculate();
	}

	@Override
	public void recalculate() {
		try {
			_bufferedResult = new ValueWrapper(_arg1.getValue() * _arg2.getValue());
		} catch (NullPointerException | IncorrectValueTypeException | PositionOutOfRangeException e) {
			_bufferedResult = null;
		}
	}

	@Override
	public CellValue deepCopy() {
		return new MulFunction(_arg1, _arg2);
	}

	@Override
	public String visualize() {
		String resultStr;

		if (_bufferedResult == null)
			resultStr = "#VALUE";
		else
			resultStr = _bufferedResult.visualize();

		return resultStr + "=MUL(" + _arg1.visualize() + "," + _arg2.visualize() + ")";
	}
}
