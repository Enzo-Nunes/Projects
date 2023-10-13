package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.InvalidSpanException;

public class AverageFunction extends SpanFunction {
	public AverageFunction(Span argument) {
		super(argument);
	}

	@Override
	public CellValue deepCopy() {
		return new AverageFunction(_argument.deepCopy());
	}

	@Override
	protected void recalculate() throws InvalidSpanException {
		int total = 0;

		for (Cell cell : _argument) {
			try {
				total += cell.getValue().getInt();
			} catch (IncorrectValueTypeException except) {
				_bufferedResult = new ValueWrapper("#VALUE");
			}
		}

		int average = total / _argument.getLength();

		_bufferedResult = new ValueWrapper(average);
	}

	@Override
	public String visualize() {
		String resultStr;
		try {
			recalculate(); // TODO: Avoid repetition
			resultStr = _bufferedResult.visualize();
		} catch (InvalidSpanException e) {
			resultStr = "#VALUE";
		}

		return resultStr + "=AVERAGE(" + _argument.visualize() + ")";
	}
}
