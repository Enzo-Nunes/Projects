package xxl.UMLracisms;

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
		
	}

	public CellValue deepCopy()
	{
		return new IntegerLiteral(_value);
	}
}
