package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.PositionOutOfRangeException;

public class AverageFunction extends SpanFunction {
	public AverageFunction(Span argument) {
		super(argument);
	}

	@Override
	CellValue deepCopy() {
		return new AverageFunction(_argument.deepCopy());
	}

	public void recalculate() throws PositionOutOfRangeException {
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
}
