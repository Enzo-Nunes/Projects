package xxl.core;

public class IntegerLiteral extends LiteralValue {
	private int _value;

	public IntegerLiteral(int value) {
		_value = value;
	}

	@Override
	public ValueWrapper getValue() {
		return new ValueWrapper(_value);
	}

	@Override
	protected void recalculate() { }

	@Override
	public CellValue deepCopy() {
		return new IntegerLiteral(_value);
	}

	@Override
	public String visualize()
	{
		return "" + _value;
	}
}
