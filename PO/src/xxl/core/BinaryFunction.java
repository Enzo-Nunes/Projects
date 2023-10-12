package xxl.core;

public abstract class BinaryFunction extends FunctionValue {
	protected BinaryArgument _arg1, _arg2;

	public BinaryFunction(BinaryArgument firstArg, BinaryArgument secondArg) {
		_arg1 = firstArg;
		_arg2 = secondArg;
	}
}
