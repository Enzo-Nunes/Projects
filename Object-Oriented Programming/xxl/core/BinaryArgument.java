package xxl.core;

import java.io.Serializable;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.PositionOutOfRangeException;

public class BinaryArgument implements Serializable {
	private int _literal;
	private Position _referencedPos;
	private Spreadsheet _sheet;

	public BinaryArgument(int literal) {
		_literal = literal;
	}

	public BinaryArgument(Position referencedPosition, Spreadsheet containingSheet) {
		_referencedPos = referencedPosition;
		_sheet = containingSheet;
	}

	public int getValue() throws IncorrectValueTypeException, PositionOutOfRangeException {
		if (_referencedPos == null)
			return _literal;

		ValueWrapper value = _sheet.getCellContent(_referencedPos);

		return value.getInt();
	}

	public BinaryArgument deepCopy() {
		if (_referencedPos == null)
			return new BinaryArgument(_literal);

		return new BinaryArgument(_referencedPos, _sheet);
	}

	public String visualize() {
		if (_referencedPos == null)
			return "" + _literal;

		return _referencedPos.visualize();
	}

	public void subscribe(Observer obs) {
		if (_referencedPos == null)
			return;

		try {
			_sheet.getCell(_referencedPos).subscribe(obs);
		} catch (PositionOutOfRangeException e) {
			return; // Ignore
		}
	}

	public void unsubscribe(Observer obs) {
		if (_referencedPos == null)
			return;

		try {
			_sheet.getCell(_referencedPos).unsubscribe(obs);
		} catch (PositionOutOfRangeException e) {
			return; // Ignore
		}
	}
}
