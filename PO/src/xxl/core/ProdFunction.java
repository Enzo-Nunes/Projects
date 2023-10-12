package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;

public class ProdFunction extends SpanFunction {
	public ProdFunction(Span argument) {
		super(argument);
	}

	@Override
	public CellValue deepCopy() {
		return new ProdFunction(_argument.deepCopy());
	}

	public void recalculate() {
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
}
