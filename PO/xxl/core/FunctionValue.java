package xxl.core;

public abstract class FunctionValue extends CellValue{
	protected ValueWrapper _bufferedResult;
	protected boolean _resultIsBuffered;

	@Override
	public void update()
	{
		recalculate();
	}

	@Override
	public ValueWrapper getValue() {
		if (_resultIsBuffered)
			return _bufferedResult;

		return _bufferedResult;
	}
}
