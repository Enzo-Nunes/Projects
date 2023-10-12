package xxl.core;

public class NullValue extends CellValue {
	// TODO: Reconsider. Might not be needed due to hashmap

	public NullValue() {

	}

	public ValueWrapper getValue() {
		return null;
	}

	public void recalculate() {

	}

	public CellValue deepCopy() {
		return new NullValue();
	}
}
