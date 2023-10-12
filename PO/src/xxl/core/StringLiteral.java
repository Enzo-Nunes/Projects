package xxl.core;

public class StringLiteral extends LiteralValue {
	private String _value;

	public StringLiteral(String value) {
		_value = value;
	}

	@Override
	public ValueWrapper getValue() {
		return new ValueWrapper(_value);
	}

	@Override
	protected void recalculate() {	}

	@Override
	public CellValue deepCopy() {
		return new StringLiteral(_value.toString());
	}

	@Override
	public String visualize()
	{
		return _value;
	}
}
