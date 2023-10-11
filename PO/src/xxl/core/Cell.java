package xxl.core;

import xxl.core.exception.IncorrectValueTypeException;

public class Cell {
	Position _position;
	Cell[] _dependants;
	CellValue _content;

	public Cell(int posX, int posY) {
		_position = new Position(posX, posY);
	}

	public Cell(Position pos){
		_position = pos;
	}

	public void update(CellValue value) {
		_content = value;
		//TODO: Update dependants
	}

	public ValueWrapper getValue() throws IncorrectValueTypeException {
		return _content.getValue();
	}
}
