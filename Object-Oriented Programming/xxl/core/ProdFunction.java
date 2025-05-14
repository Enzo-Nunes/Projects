package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.PositionOutOfRangeException;

public class ProdFunction extends SpanFunction {
	public ProdFunction(Span argument) {
		super(argument);
		recalculate();
	}

	@Override
	public CellValue deepCopy() {
		return new ProdFunction(_argument.deepCopy());
	}

	@Override
	protected void recalculate() {
		int total = 1;

		for (Cell cell : _argument) {
			try {
				total *= cell.getValue().getInt();
			} catch (IncorrectValueTypeException | PositionOutOfRangeException | NullPointerException except) {
				_bufferedResult = null;
				return;
			}
		}

		_bufferedResult = new ValueWrapper(total);
	}

	@Override
	public String visualize() {
		String resultStr;

		if (_bufferedResult == null)
			resultStr = "#VALUE";
		else
			resultStr = _bufferedResult.visualize();

		return resultStr + "=PRODUCT(" + _argument.visualize() + ")";
	}

	@Override
	public String getFunctionName() {
		return "PRODUCT";
	}
}
