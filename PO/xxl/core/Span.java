package xxl.core;

import xxl.core.exception.ParsingException;
import xxl.core.exception.InvalidSpanException;

import java.util.Iterator;

public class Span implements Iterable<Cell> {
	private Position _start;
	private int _length;
	private Spreadsheet _sheet;
	private boolean _isRowSpan;

	public static Span parse(String src, Spreadsheet holder) throws ParsingException, InvalidSpanException {
		String[] parts = src.split(":");
		if (parts.length == 1) {
			Position pos = Position.parse(src);
			return new Span(pos, pos, holder);
		} else if (parts.length == 2)
			return new Span(Position.parse(parts[0]), Position.parse(parts[1]), holder);
		else
			throw new ParsingException();
	}

	public Span(Position start, Position end, Spreadsheet containingSheet) throws InvalidSpanException {
		_start = start;
		_length = end.getX() - start.getX() + 1;
		_sheet = containingSheet;
		_isRowSpan = isRowSpan(start, end);

		if (!containingSheet.positionisValid(start) || 
			!containingSheet.positionisValid(end))
			throw new InvalidSpanException();
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

	public String visualize()
	{
		int endX = _start.getX() + (_isRowSpan ? _length : 0);
		int endY = _start.getY() + (_isRowSpan ? 0 : _length);
		return _start.getX() + ";" + _start.getY() + ":" + endX + ";" + endY;
	}
}
