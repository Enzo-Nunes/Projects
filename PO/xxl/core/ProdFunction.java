package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.InvalidSpanException;

public class ProdFunction extends SpanFunction {
	public ProdFunction(Span argument) {
		super(argument);
	}

	@Override
	public CellValue deepCopy() {
		return new ProdFunction(_argument.deepCopy());
	}

	@Override
	protected void recalculate() throws InvalidSpanException {
		int total = 1;

		for (Cell cell : _argument) {
			try {
				total *= cell.getValue().getInt();
			} catch (IncorrectValueTypeException except) {
				_bufferedResult = new ValueWrapper("#VALUE");
			}
		}

		_bufferedResult = new ValueWrapper(total);
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

		return resultStr + "=PRODUCT(" + _argument.visualize() + ")";
	}
}
