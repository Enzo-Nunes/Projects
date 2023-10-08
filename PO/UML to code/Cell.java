package xxl.UMLracisms;

public class Cell {
	int _x, _y;
	Cell[] _dependants;
	CellValue _content;

	public Cell(int x, int y)
	{
		_x = x; _y = y;
	}

	public void update(CellValue value)
	{
		_content = value;
		//Update dependants
	}

	public ValueWrapper getValue()
	{
		return _content.getValue();
	}
}
