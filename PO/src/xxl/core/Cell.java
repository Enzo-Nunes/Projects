package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.PositionOutOfRangeException;

public class Cell {
	Position _position;
	CellValue _content;

	public Cell(Position pos){
		_position = pos;
	}
	public void update(CellValue value) {
		_content = value;
	}

	public ValueWrapper getValue() throws IncorrectValueTypeException, PositionOutOfRangeException {
		_content.recalculate();
		return _content.getValue();
	}
}
