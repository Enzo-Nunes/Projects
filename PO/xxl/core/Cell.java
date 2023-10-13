package xxl.core;

import java.io.Serializable;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.InvalidSpanException;

public class Cell implements Serializable {
	private Position _position;
	private CellValue _content;

	public Cell(Position pos){
		_position = pos;
	}

	public Position getPosition() { return _position; }
	public CellValue getContentCopy() { return _content.deepCopy(); }

	public void update(CellValue value) {
		_content = value;
	}

	public ValueWrapper getValue() throws IncorrectValueTypeException, InvalidSpanException {
		_content.recalculate();
		return _content.getValue();
	}

	public String visualize()
	{
		if (_content == null)
			return _position.visualize() + "|";
		return _position.visualize() + "|" + _content.visualize();
	}
}
