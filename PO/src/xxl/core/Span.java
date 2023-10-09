package xxl.core;

public class Span {
	private Position _start, _end;
	private Spreadsheet _sheet;

	public Span(Position start, Position end, Spreadsheet containingSheet) {
		_start = start;
		_end = end;
		_sheet = containingSheet;
	}

	//TODO: Implement rest
}
