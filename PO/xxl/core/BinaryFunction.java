package xxl.core;

public abstract class BinaryFunction extends FunctionValue {
	protected BinaryArgument _arg1, _arg2;

	public BinaryFunction(BinaryArgument firstArg, BinaryArgument secondArg) {
		_arg1 = firstArg;
		_arg2 = secondArg;
	}

	@Override
	public void subscribeToAll() {
		_arg1.subscribe(this);
		_arg2.subscribe(this);
	}

	@Override
	public void unsubscribeFromAll() {
		_arg1.unsubscribe(this);
		_arg2.unsubscribe(this);
	}
}
