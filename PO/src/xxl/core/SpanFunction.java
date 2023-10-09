package xxl.core;

public abstract class SpanFunction extends FunctionValue {
	private Span _argument;

	public SpanFunction(Span argument) {
		_argument = argument;
	}
}
