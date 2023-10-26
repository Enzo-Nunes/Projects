package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.PositionOutOfRangeException;

public class ReferenceValue extends CellValue {
	private Position _referencedPos;
	private Spreadsheet _sheet;

	public ReferenceValue(Position referencedPosition, Spreadsheet containingSheet) {
		_referencedPos = referencedPosition;
		_sheet = containingSheet;
	}

	@Override
	public ValueWrapper getValue() throws PositionOutOfRangeException, IncorrectValueTypeException {
		return _sheet.getCellContent(_referencedPos);
	}

	@Override
	protected void recalculate() {
		return; //Do nothing
	}

	@Override
	public CellValue deepCopy() {
		return new ReferenceValue(_referencedPos, _sheet);
	}

	@Override
	public String visualize() {
		String resultStr;
		try {
			ValueWrapper value = getValue();
			if (value == null)
				resultStr = "#VALUE";
			else
				resultStr = value.visualize();
		} catch (IncorrectValueTypeException | PositionOutOfRangeException e) {
			resultStr = "#VALUE";
		}

		return resultStr + "=" + _referencedPos.visualize();
	}

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
