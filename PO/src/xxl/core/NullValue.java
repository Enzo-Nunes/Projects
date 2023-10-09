package xxl.core;

public class NullValue extends CellValue {
	public NullValue()
	{

	}

	public ValueWrapper getValue()
	{
		return null;
	}

	public void recalculate()
	{

	}

	public CellValue deepCopy()
	{
		return new NullValue();
	}
}
