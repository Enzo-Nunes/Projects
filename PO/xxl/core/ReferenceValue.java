package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.InvalidSpanException;

public class ReferenceValue extends CellValue {
	private Position _referencedPos;
	private Spreadsheet _sheet;

	public ReferenceValue(Position referencedPosition, Spreadsheet containingSheet)
	{
		_referencedPos = referencedPosition;
		_sheet = containingSheet;
	}

	@Override
	public ValueWrapper getValue() throws InvalidSpanException, IncorrectValueTypeException
	{
		return _sheet.getCellContent(_referencedPos);
	}

	@Override
	protected void recalculate() { }

	@Override
	public CellValue deepCopy()
	{
		return new ReferenceValue(_referencedPos, _sheet);
	}

	@Override
	public String visualize()
	{
		String resultStr;
		try
		{
			ValueWrapper value = getValue();
			if (value == null)
				return "#VALUE" + "=" + _referencedPos.visualize();
			else
				resultStr = value.visualize();
		} catch (IncorrectValueTypeException | InvalidSpanException e) {
			resultStr = "#VALUE";
		}
		
		return resultStr + "=" + _referencedPos.visualize();
	}
}