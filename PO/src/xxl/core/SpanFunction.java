package xxl.core;

public abstract class SpanFunction extends FunctionValue {
	protected Span _argument;

	public SpanFunction(Span argument) {
		_argument = argument;
	}
}
