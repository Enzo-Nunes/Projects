package xxl.core;

import java.io.Serializable;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.PositionOutOfRangeException;

abstract class CellValue implements Serializable, Observer {
	public abstract ValueWrapper getValue() throws IncorrectValueTypeException, PositionOutOfRangeException;

	protected abstract void recalculate();

	public abstract CellValue deepCopy();

	public abstract String visualize();

	protected abstract void subscribeToAll();

	public abstract void unsubscribeFromAll();
}
