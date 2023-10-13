package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.InvalidSpanException;

public class SubFunction extends BinaryFunction {

	public SubFunction(BinaryArgument firstArg, BinaryArgument secondArg) {
		super(firstArg, secondArg);
	}

	@Override
	public void recalculate() throws IncorrectValueTypeException, InvalidSpanException {
		_bufferedResult = new ValueWrapper(_arg1.getValue() - _arg2.getValue());
	}

	@Override
	public CellValue deepCopy() {
		return new SubFunction(_arg1, _arg2);
	}

	@Override
	public String visualize()
	{
		String resultStr;
		try
		{
			recalculate();
			resultStr = _bufferedResult.visualize();
		} catch (IncorrectValueTypeException | InvalidSpanException e) {
			resultStr = "#VALUE";
		}
		
		return resultStr + "=SUB(" + _arg1.visualize() + "," + _arg2.visualize() + ")";
	}
}
