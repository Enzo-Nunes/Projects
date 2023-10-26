package xxl.core;

public abstract class SpanFunction extends FunctionValue {
	protected Span _argument;

	public SpanFunction(Span argument) {
		_argument = argument;
	}

	@Override
	public void subscribeToAll() {
		_argument.subscribe(this);
	}

	@Override
	public void unsubscribeFromAll() {
		_argument.unsubscribe(this);
	}
}
