package xxl.core;

import java.util.Iterator;

public class SpanIterator implements Iterator<Cell> {
	private Position _start;
	private Spreadsheet _sheet;
	private Position _current;

	public SpanIterator(Position start, Position end, Spreadsheet containingSheet) {
		_start = start;
		_end = end;
		_sheet = containingSheet;
		_current = _start;
	}

	public boolean hasNext() {
		return _current.compareTo(_end) <= 0;
	}

	public Cell next() {
		Cell result = _sheet.getCell(_current);
		_current = _current.next();
		return result;
	}

	public void remove() {
		throw new UnsupportedOperationException();
	}
}
