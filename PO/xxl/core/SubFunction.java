package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.PositionOutOfRangeException;

public class SubFunction extends BinaryFunction {

	public SubFunction(BinaryArgument firstArg, BinaryArgument secondArg) {
		super(firstArg, secondArg);
		recalculate();
	}

	@Override
	public void recalculate() {
		try {
			_bufferedResult = new ValueWrapper(_arg1.getValue() - _arg2.getValue());
		} catch (NullPointerException | PositionOutOfRangeException | IncorrectValueTypeException e) {
			_bufferedResult = null;
		}
	}

	@Override
	public CellValue deepCopy() {
		return new SubFunction(_arg1, _arg2);
	}

	@Override
	public String visualize() {
		String resultStr;
		
		if (_bufferedResult == null)
			resultStr = "#VALUE";
		else
			resultStr = _bufferedResult.visualize();

		return resultStr + "=SUB(" + _arg1.visualize() + "," + _arg2.visualize() + ")";
	}
}
