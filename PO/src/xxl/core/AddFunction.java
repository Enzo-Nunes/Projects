package xxl.core;

public class AddFunction extends BinaryFunction {
	
	public AddFunction(BinaryArgument firstArg, BinaryArgument secondArg)
	{
		super(firstArg, secondArg);
	}

	@Override
	void recalculate() throws Exception {
		_bufferedResult = new ValueWrapper(_arg1.getValue() + _arg2.getValue());
	}

	@Override
	CellValue deepCopy() {
		return new AddFunction(_arg1.deepCopy(), _arg2.deepCopy());
	}
}
