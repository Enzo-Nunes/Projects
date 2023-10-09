package xxl.core;

public class StringLiteral extends LiteralValue {
	private String _value;

	public StringLiteral(String value)
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
		return new StringLiteral(_value.toString());
	}
}
