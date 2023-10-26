package xxl.core;

import java.util.ArrayList;

public class CutBuffer {
    private ArrayList<CellValue> _content;
	private boolean _isRowBuffer;

    public CutBuffer() {
        _content = new ArrayList<CellValue>();
    }

    public Span getSpan() {
        return new Span(new Position (1, 1), _content.size(), _isRowBuffer);
	}

    public void setContent(Span span) {
		_content.clear();
		_isRowBuffer = span.isRowSpan();
        for(Cell cell : span) {
			_content.add(cell.getContentCopy());
		}
    }
}