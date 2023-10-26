package xxl.core;

public abstract class LiteralValue extends CellValue {
	@Override
	public void update() {
		return; //Do nothing
	}

	@Override
	public void subscribeToAll() {
		return; //Do nothing
	}

	@Override
	public void unsubscribeFromAll() {
		return; //Do nothing
	}
}
