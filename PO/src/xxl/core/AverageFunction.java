package xxl.core;

public class AverageFunction extends SpanFunction {
	public AverageFunction(Span argument) {
		super(argument);
	}

	@Override
	CellValue deepCopy() {
		return new AverageFunction(_argument.deepCopy());
	}

	public void recalculate() throws Exception {
		
	}
}
