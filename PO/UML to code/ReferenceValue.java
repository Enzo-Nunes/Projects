package xxl.UMLracisms;

public class ReferenceValue extends CellValue {
	Cell _refCell;

	public ReferenceValue(Cell referencedCell)
	{
		_refCell = referencedCell;
	}

	public ValueWrapper getValue()
	{
		return _refCell.getValue();
	}

	public void recalculate()
	{

	}

	public CellValue deepCopy()
	{
		return new ReferenceValue(_refCell);
	}
}
