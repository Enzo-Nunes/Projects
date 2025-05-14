package xxl.core;

public abstract class SpanFunction extends FunctionValue {
	protected Span _argument;

	public SpanFunction(Span argument) {
		_argument = argument;
		subscribeToAll();
	}

	@Override
	protected final void subscribeToAll() {
		_argument.subscribe(this);
	}

	@Override
	public final void unsubscribeFromAll() {
		_argument.unsubscribe(this);
	}
}
