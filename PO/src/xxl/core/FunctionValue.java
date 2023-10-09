package xxl.core;

public abstract class FunctionValue extends CellValue {
	protected ValueWrapper _bufferedResult;
	protected boolean _resultIsBuffered;

	@Override
	ValueWrapper getValue() throws Exception {
		if (_resultIsBuffered)
			return _bufferedResult;
		
		recalculate();
		return _bufferedResult;
	}
}
