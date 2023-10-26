package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.PositionOutOfRangeException;

public class CoalesceFunction extends SpanFunction {
	public CoalesceFunction(Span argument) {
		super(argument);
		recalculate();
		//TODO: Register observer
	}

	@Override
	protected void recalculate() {
		String result = "";

		for (Cell cell : _argument) {
			try {
				result = cell.getValue().getString();
				break;
			} catch (IncorrectValueTypeException | PositionOutOfRangeException e) {
				result = "";
			}
		}

		_bufferedResult = new ValueWrapper(result);
	}

	@Override
	public CellValue deepCopy() {
		return new CoalesceFunction(_argument.deepCopy());
	}

	@Override
	public String visualize() {
		String resultStr;

		if (_bufferedResult == null)
			resultStr = "#VALUE";
		else
		resultStr = _bufferedResult.visualize();

		return resultStr + "=COALESCE(" + _argument.visualize() + ")";
	}
}
