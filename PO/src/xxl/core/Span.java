package xxl.core;

import java.util.Iterator;

public class Span implements Iterable<Cell> {
	private Position _start;
	private int _length;
	private Spreadsheet _sheet;
	private boolean _isRowSpan;

	public Span(Position start, Position end, Spreadsheet containingSheet) {
		_start = start;
		_length = end.getX() - start.getX() + 1;
		_sheet = containingSheet;
		_isRowSpan = isRowSpan(start, end);
	}

	private Span(Position start, int length, Spreadsheet containingSheet, boolean isRowSpan) {
		_start = start;
		_length = length;
		_sheet = containingSheet;
		_isRowSpan = isRowSpan;
	}

	public Span deepCopy() {
		return new Span(_start, _length, _sheet, _isRowSpan);
	}

	public int getLength() {
		return _length;
	}

	private static boolean isRowSpan(Position start, Position end) {
		return start.getY() == end.getY();
	}

	public Iterator<Cell> iterator() {
		return new SpanIterator(_start, _isRowSpan, _length, _sheet);
	}
}
