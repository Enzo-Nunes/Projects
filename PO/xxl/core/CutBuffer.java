package xxl.core;

import java.io.Serializable;
import java.util.ArrayList;

public class CutBuffer implements Serializable {
    private ArrayList<CellValue> _content;
	private boolean _isRowBuffer;

    public CutBuffer() {
        _content = new ArrayList<CellValue>();
    }

    public boolean isRowBuffer() { return _isRowBuffer; }

	public int getLength() { return _content.size(); }

    public void setContent(Span span) {
		_content.clear();
		_isRowBuffer = span.isRowSpan();
        for(Cell cell : span) {
			_content.add(cell.getContentCopy());
		}
    }

	public ArrayList<CellValue> getContent() {
		return _content;
	}

	public String visualize()
	{
		String ret = "";

		for (int i = 0; i < _content.size(); i++)
		{
			int x = 1 + (_isRowBuffer ? i : 0);
			int y = 1 + (_isRowBuffer ? 0 : i);

			String pos = "" + y + ";" + x;

			String value = _content.get(i) != null ? _content.get(i).visualize() : "";
			ret += pos + "|" + value;

			if (i < _content.size() - 1)
				ret += "\n";
		}

		return ret;
	}
}
