package xxl.core;

import java.util.Iterator;

public class SpanIterator implements Iterator<Cell> {
	private Position _start;
	private boolean _isRowSpan;
	private int _offset, _length;
	private Spreadsheet _sheet;

	public SpanIterator(Position start, boolean isRowSpan, int length, Spreadsheet containingSheet) {
		_start = start;
		_isRowSpan = isRowSpan;
		_length = length;
		_offset = 0;
		_sheet = containingSheet;
	}

	public boolean hasNext() {
		return _offset < _length;
	}

	public Cell next() {
		Position pos = getPositionFromOffset();
		Cell result = _sheet.getCell(pos);
		_offset++;
		return result;
	}

	public void remove() {
		throw new UnsupportedOperationException();
	}

	private Position getPositionFromOffset() {
		if (_isRowSpan)
			return new Position(_start.getX() + _offset, _start.getY());
		else
			return new Position(_start.getX(), _start.getY() + _offset);
	}
}
