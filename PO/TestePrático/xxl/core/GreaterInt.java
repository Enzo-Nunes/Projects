package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.PositionOutOfRangeException;

public class GreaterInt extends SpanFunction {
	public GreaterInt(Span argument) {
		super(argument);
		recalculate();
	}

	@Override
	public CellValue deepCopy() {
		return new AverageFunction(_argument.deepCopy());
	}
	
	@Override
	protected void recalculate() {
		int big = 0;

		for (Cell cell : _argument) {
			try {
				if (cell.getValue().getInt() > big) {
					big = cell.getValue().getInt();
				}
			} catch (IncorrectValueTypeException | PositionOutOfRangeException | NullPointerException e) {
				continue;
			}
		}

		_bufferedResult = new ValueWrapper(big);
	}

	@Override
	public String visualize() {
		String resultStr;

		if (_bufferedResult == null)
			resultStr = "#VALUE";
		else
			resultStr = _bufferedResult.visualize();

		return resultStr + "=MAIORINT(" + _argument.visualize() + ")";
	}

	@Override
	public String getFunctionName() {
		return "AVERAGE";
	}
}
