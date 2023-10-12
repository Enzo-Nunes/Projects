package xxl.core;

import java.util.ArrayList;
import java.util.HashMap;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.PositionOutOfRangeException;

public class Spreadsheet {
	ArrayList<User> _owners;
	HashMap<Position, Cell> _cells;
	CutBuffer _cutBuffer;
	Position _botLeftCorner; // aka size
	String _filename;

	public Spreadsheet(int width, int height) {
		_owners = new ArrayList<User>();
		// _owners.add(owner);

		_cells = new HashMap<Position, Cell>();
		_botLeftCorner = new Position(width, height);
	}

	public void addOwner(User owner) {
		_owners.add(owner);
	}

	public void removeOwner(User owner) {
		_owners.remove(owner);
	}

	public Cell getCell(Position position) throws PositionOutOfRangeException {
		if (!posInSpace(position))
			throw new PositionOutOfRangeException();

		if (_cells.containsKey(position))
			return _cells.get(position);

		return null;
	}

	public void setCellContent(Position position, CellValue content)
			throws IncorrectValueTypeException, PositionOutOfRangeException {
		if (!posInSpace(position))
			throw new PositionOutOfRangeException();

		if (!_cells.containsKey(position))
			_cells.put(position, new Cell(position)).update(content);
		else
			_cells.get(position).update(content);
	}

	public ValueWrapper getCellContent(Position position)
			throws IncorrectValueTypeException, PositionOutOfRangeException {
		if (!posInSpace(position))
			throw new PositionOutOfRangeException();

		if (_cells.containsKey(position))
			return _cells.get(position).getValue();

		return null;
	}

	private boolean posInSpace(Position pos) {
		return (pos.getX() >= 0 && pos.getX() < _botLeftCorner.getX())
				&& (pos.getY() >= 0 && pos.getY() < _botLeftCorner.getY());
	}

	public void setFilename(String filename) {
		_filename = filename;
	}

	public String getFilename() {
		return _filename;
	}
}
