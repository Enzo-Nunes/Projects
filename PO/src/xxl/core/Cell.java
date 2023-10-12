package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;

public class Cell {
	Position _position;
	CellValue _content;

	public Cell(Position pos){
		_position = pos;
	}
	public void update(CellValue value) throws IncorrectValueTypeException {
		_content = value;
	}

	public ValueWrapper getValue() throws IncorrectValueTypeException {
		_content.recalculate();
		return _content.getValue();
	}
}
