package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.PositionOutOfRangeException;

public class ReferenceValue extends CellValue {
	private Position _referencedPos;
	private Spreadsheet _sheet;

	public ReferenceValue(Position referencedPosition, Spreadsheet containingSheet)
	{
		_referencedPos = referencedPosition;
		_sheet = containingSheet;
	}

	public ValueWrapper getValue() throws PositionOutOfRangeException, IncorrectValueTypeException
	{
		return _sheet.getCellContent(_referencedPos);
	}

	public void recalculate()
	{
		return; //Unnecessary
	}

	public CellValue deepCopy()
	{
		return new ReferenceValue(_referencedPos, _sheet);
	}
}
