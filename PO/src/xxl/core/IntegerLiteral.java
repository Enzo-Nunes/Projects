package xxl.core;

public class IntegerLiteral extends LiteralValue {
	private int _value;

	public IntegerLiteral(int value)
	{
		_value = value;
	}

	public ValueWrapper getValue()
	{
		return new ValueWrapper(_value);
	}

	public void recalculate()
	{
		return;
	}

	public CellValue deepCopy()
	{
		return new IntegerLiteral(_value);
	}
}
