package xxl.core;

import java.util.ArrayList;
import java.util.HashMap;
import java.io.Serializable;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.InvalidSpanException;
import xxl.core.exception.PositionOutOfRangeException;
import xxl.core.exception.UnrecognizedEntryException;
import xxl.core.exception.ParsingException;

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
		_dirty = true;
	}

	public void addOwner(User owner) {
		_owners.add(owner);
	}

	public void removeOwner(User owner) {
		_owners.remove(owner);
	}

	public void setDirty(boolean dirty) {
		_dirty = dirty;
	}

	public Cell getCell(Position position) throws PositionOutOfRangeException {
		if (!positionisValid(position))
			throw new PositionOutOfRangeException();

		if (_cells.containsKey(position)) {
			Cell cell = _cells.get(position);
			return cell;
		} else {
			Cell cell = new Cell(position);
			_cells.put(position, cell);
			return cell;
		}
	}

	public void setCellContent(Position position, CellValue content)
			throws PositionOutOfRangeException {
		if (!positionisValid(position))
			throw new PositionOutOfRangeException();

		_dirty = true;

		if (!_cells.containsKey(position)) {
			Cell cell = new Cell(position);
			cell.updateValue(content);
			_cells.put(position, cell);
		} else
			_cells.get(position).updateValue(content);
	}

	public ValueWrapper getCellContent(Position position)
			throws IncorrectValueTypeException, PositionOutOfRangeException {
		if (!positionisValid(position))
			throw new PositionOutOfRangeException();

		if (_cells.containsKey(position))
			return _cells.get(position).getValue();

		return null;
	}

	public void clearCellContent(Position position) throws PositionOutOfRangeException {
		if (!positionisValid(position))
			throw new PositionOutOfRangeException();

		if (_cells.containsKey(position)) {
			_dirty = true;
			_cells.remove(position);
		}
	}

	public void clearSpanContent(Span span) throws PositionOutOfRangeException {
		for (Cell cell : span) {
			clearCellContent(cell.getPosition());
		}
	}

	public void updateCutBuffer(Span span) throws PositionOutOfRangeException {
		_cutBuffer.setContent(span.deepCopy());
	}

	public void pasteCutBuffer(Span span) throws InvalidSpanException, PositionOutOfRangeException {
		int bufferLength = _cutBuffer.getLength();

		// FIXME No exceptions??
		if (bufferLength == 0)
			return;

		// FIXME Repeated code. Optimizable?
		if (span.isSingleCell()) {
			for (Cell cell : span) {
				for (int i = 0; i < bufferLength; i++) {
					Position pos = _cutBuffer.isRowBuffer()
							? new Position(cell.getPosition().getX() + i, cell.getPosition().getY())
							: new Position(cell.getPosition().getX(), cell.getPosition().getY() + i);
					if (!positionisValid(pos))
						break;
					setCellContent(pos, _cutBuffer.getContent().get(i));
				}
			}
		} else {

			if (_cutBuffer.isRowBuffer() != span.isRowSpan())
				throw new InvalidSpanException();

			int i = 0;
			for (Cell cell : span) {
				setCellContent(cell.getPosition(), _cutBuffer.getContent().get(i));
				i++;
			}
		}
	}

	public CutBuffer getCutBuffer() {
		return _cutBuffer;
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

	public void clearSpan(Span span) {
		for (Cell cell : span) {
			cell.unsubscribeFromAll();
			Position pos = cell.getPosition();
			_cells.remove(pos);
		}
	}

	public String searchCellValues(String value) {
		Parser parser = new Parser();
		LiteralValue parsed;

		try {
			parsed = parser.parseLiteral(value);
		} catch (UnrecognizedEntryException | NumberFormatException | InvalidSpanException e) {
			return null;
		}

		ArrayList<Cell> matches = findCellsWithValue(parsed);

		String ret = "";

		for (Cell cell : matches)
			ret += cell.visualize() + "\n";

		return ret;
	}

	private ArrayList<Cell> findCellsWithValue(LiteralValue value) {
		ArrayList<Cell> ret = new ArrayList<Cell>();

		for (Cell cell : _cells.values()) {
			try {
				// System.out.println("\n" + value.visualize());
				// System.out.println(cell.getValue().visualize());
				// System.out.println(cell.getValue().equals(value.getValue()) + "\n");
				if (cell.getValue().equals(value.getValue()))
					ret.add(cell);
			} catch (PositionOutOfRangeException | IncorrectValueTypeException e) {
				continue; // Ignore
			}
		}

		ret.sort(new PositionComparator());

		return ret;
	}

	public String searchCellFunctions(String value) {
		ArrayList<Cell> matches = findCellsWithFunction(value);

		String ret = "";

		for (Cell cell : matches)
			ret += cell.visualize() + "\n";

		return ret;
	}

	private ArrayList<Cell> findCellsWithFunction(String funcName) {
		ArrayList<Cell> ret = new ArrayList<Cell>();

		for (Cell cell : _cells.values()) {
			if (cell.getFunctionName().contains(funcName))
				ret.add(cell);
		}

		ret.sort(new PositionComparator());

		return ret;
	}

	public void insertSpan(String span, String content)
			throws ParsingException, InvalidSpanException, UnrecognizedEntryException {
		Parser parser = new Parser(this);
		Span parsedSpan = Span.parse(span, this);
		CellValue parsedContent = parser.parseCellValue(content);

		for (Cell cell : parsedSpan)
			cell.updateValue(parsedContent.deepCopy());
	}
}
