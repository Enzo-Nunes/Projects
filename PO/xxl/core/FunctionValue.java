package xxl.core;

public abstract class FunctionValue extends CellValue{
	protected ValueWrapper _bufferedResult;

	@Override
	public void update()
	{
		recalculate();
	}

	@Override
	public ValueWrapper getValue() {
		return _bufferedResult;
	}
}
