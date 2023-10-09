package xxl.core;

public abstract class CellValue {
	abstract ValueWrapper getValue() throws Exception;
	abstract void recalculate() throws Exception;
	abstract CellValue deepCopy();
}
