package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.PositionOutOfRangeException;

public class ConcatFunction extends SpanFunction {
	public ConcatFunction(Span argument) {
		super(argument);
		recalculate();
	}

	@Override
	protected void recalculate() {
		String result = "";

		for (Cell cell : _argument) {
			try {
				result += cell.getValue().getString();
			} catch (IncorrectValueTypeException | PositionOutOfRangeException | NullPointerException e) {
				result += "";
			}
		}

		_bufferedResult = new ValueWrapper(result);
	}

	@Override
	public CellValue deepCopy() {
		return new ConcatFunction(_argument.deepCopy());
	}

	@Override
	public String visualize() {
		String resultStr;
		
		if (_bufferedResult == null)
			resultStr = "#VALUE";
		else
			resultStr = _bufferedResult.visualize();

		return resultStr + "=CONCAT(" + _argument.visualize() + ")";
	}
}
