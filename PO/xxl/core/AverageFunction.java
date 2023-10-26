package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.PositionOutOfRangeException;

public class AverageFunction extends SpanFunction {
	public AverageFunction(Span argument) {
		super(argument);
		recalculate();
	}

	@Override
	public CellValue deepCopy() {
		return new AverageFunction(_argument.deepCopy());
	}

	@Override
	protected void recalculate() {
		int total = 0;

		for (Cell cell : _argument) {
			try {
				total += cell.getValue().getInt();
			} catch (IncorrectValueTypeException | PositionOutOfRangeException e) {
				_bufferedResult = null;
			}
		}

		int average = total / _argument.getLength();

		_bufferedResult = new ValueWrapper(average);
	}

	@Override
	public String visualize() {
		String resultStr;

		if (_bufferedResult == null)
			resultStr = "#VALUE";
		else
			resultStr = _bufferedResult.visualize();

		return resultStr + "=AVERAGE(" + _argument.visualize() + ")";
	}
}
