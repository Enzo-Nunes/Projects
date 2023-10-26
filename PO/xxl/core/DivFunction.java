package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.InvalidSpanException;

public class DivFunction extends BinaryFunction {

	public DivFunction(BinaryArgument firstArg, BinaryArgument secondArg) {
		super(firstArg, secondArg);
	}

	@Override
	protected void recalculate() throws IncorrectValueTypeException, InvalidSpanException {
		try {
			_bufferedResult = new ValueWrapper(_arg1.getValue() / _arg2.getValue());
		} catch (NullPointerException e) {
			_bufferedResult = null;
		}
	}

	@Override
	public CellValue deepCopy() {
		return new DivFunction(_arg1, _arg2);
	}

	@Override
	public String visualize() {
		String resultStr;
		try {
			recalculate();
			resultStr = _bufferedResult.visualize();
		} catch (IncorrectValueTypeException | InvalidSpanException | NullPointerException e) {
			resultStr = "#VALUE";
		}

		return resultStr + "=DIV(" + _arg1.visualize() + "," + _arg2.visualize() + ")";
	}
}
