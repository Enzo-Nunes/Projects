package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.InvalidSpanException;

public class CoalesceFunction extends SpanFunction {
	public CoalesceFunction(Span argument) {
		super(argument);
	}

	@Override
	protected void recalculate() throws InvalidSpanException {
		String result = "";

		for (Cell cell : _argument)
		{
			try
			{
				result = cell.getValue().getString();
				break;
			} catch (IncorrectValueTypeException e) {
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
		try
		{
			recalculate(); //TODO: Avoid repetition
			resultStr = _bufferedResult.visualize();
		} catch (InvalidSpanException e) {
			resultStr = "#VALUE";
		}
		
		return resultStr + "=COALESCE(" + _argument.visualize() + ")";
	}
}