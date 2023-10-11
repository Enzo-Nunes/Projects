package xxl.core;

import java.nio.channels.ClosedSelectorException;
import java.util.ArrayList;
import java.util.HashMap;

public class Spreadsheet {
	ArrayList<User> _owners;
	HashMap<Position, Cell> _cells;
	CutBuffer _cutBuffer;
	Parser _parser;



	public Spreadsheet(int width, int height) {
		_owners = new ArrayList<User>();
		// _owners.add(owner);

		_cells = new HashMap<Position, Cell>();

		_parser = new Parser(this);
	}

	public void addOwner(User owner) {
		_owners.add(owner);
	}
	
	public void removeOwner(User owner) {
		_owners.remove(owner);
	}

	public void setCellContent(Position position, CellValue content)
	{
		if (!_cells.containsKey(position))
			_cells.put(position, new Cell(position)).update(content);
		else
			_cells.get(position).update(content);
	}

	public ValueWrapper getCellContent(Position position) throws Exception
	{
		if (_cells.containsKey(position))
			return _cells.get(position).getValue();

		return null;
	}
}
