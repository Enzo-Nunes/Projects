package xxl.UMLracisms;

public abstract class CellValue {
	abstract ValueWrapper getValue();
	abstract void recalculate();
	abstract CellValue deepCopy();
}
