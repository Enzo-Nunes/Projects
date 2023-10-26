package xxl.core;

import java.util.ArrayList;
import java.util.HashMap;
import java.io.Serializable;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.InvalidSpanException;

public class Spreadsheet implements Serializable {
	private ArrayList<User> _owners;
	private HashMap<Position, Cell> _cells;
	// private CutBuffer _cutBuffer;
	private Position _botLeftCorner; // aka size
	private String _filename;
	private boolean _dirty;
	private CutBuffer _cutBuffer;

	public Spreadsheet(int width, int height) {
		_owners = new ArrayList<User>();
		// _owners.add(owner);

		_cells = new HashMap<Position, Cell>();
		_botLeftCorner = new Position(width, height);
		_cutBuffer = new CutBuffer();
		_dirty = false;
	}

	public void addOwner(User owner) {
		_owners.add(owner);
	}

	public void removeOwner(User owner) {
		_owners.remove(owner);
	}

	public Cell getCell(Position position) throws InvalidSpanException {
		if (!positionisValid(position))
			throw new InvalidSpanException();

		if (_cells.containsKey(position)) {
			Cell cell = _cells.get(position);
			return cell;
		}

		return null;
	}

	public void setCellContent(Position position, CellValue content)
			throws IncorrectValueTypeException, InvalidSpanException {
		if (!positionisValid(position))
			throw new InvalidSpanException();

		_dirty = true;

		if (!_cells.containsKey(position)) {
			Cell cell = new Cell(position);
			cell.updateValue(content);
			_cells.put(position, cell);
		} else
			_cells.get(position).updateValue(content);
	}

	public ValueWrapper getCellContent(Position position) throws IncorrectValueTypeException, InvalidSpanException {
		if (!positionisValid(position))
			throw new InvalidSpanException();

		if (_cells.containsKey(position))
			return _cells.get(position).getValue();

		return null;
	}

	public void insertCell(Position position, CellValue content) throws IncorrectValueTypeException, InvalidSpanException {
		if (!positionisValid(position))
			throw new InvalidSpanException();

		_dirty = true;

		if (!_cells.containsKey(position)) {
			Cell cell = new Cell(position);
			cell.update(content);
			_cells.put(position, cell);
		} else
			_cells.get(position).update(content);
	}

	public void updateCutBuffer(Span span) throws InvalidSpanException {
		_cutBuffer.setContent(span.deepCopy());
	}

	public void pasteCutBuffer(Span span) throws InvalidSpanException {

		Span bufferSpan = _cutBuffer.getSpan();
		int bufferLenght = bufferSpan.getLength();

		if (bufferSpan.isRowSpan() != span.isRowSpan())
			throw new InvalidSpanException();

		//FIXME No exceptions??
		if (bufferLenght == 0 || bufferLenght != span.getLength())
			return;

		if (span.isSingleCell()) {
			for (Cell cell : span) {
				//FIXMEN Add 'if' to check if inside the spreadsheet
				insertCell(cell.getPosition(), _cutBuffer.getContent(cell.getPosition()));
			}
		} else {
			//TODO deep copy each cell of the
		}

	}

	public boolean positionisValid(Position pos) {
		return (pos.getX() > 0 && pos.getX() <= _botLeftCorner.getX())
				&& (pos.getY() > 0 && pos.getY() <= _botLeftCorner.getY());
	}

	public void setFilename(String filename) {
		_filename = filename;
	}

	public String getFilename() {
		return _filename;
	}

	public void markAsClean() {
		_dirty = false;
	}

	public boolean isDirty() {
		return _dirty;
	}
}
