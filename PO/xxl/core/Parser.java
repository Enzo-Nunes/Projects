package xxl.core;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.ParsingException;
import xxl.core.exception.InvalidSpanException;
import xxl.core.exception.SpreadsheetSizeException;
import xxl.core.exception.UnrecognizedEntryException;

class Parser {
	Spreadsheet _sheet;

	public Parser() {
	}

	public Spreadsheet parseFromFile(String filename)
			throws UnrecognizedEntryException, NumberFormatException, IncorrectValueTypeException, InvalidSpanException,
			FileNotFoundException, IOException, SpreadsheetSizeException, ParsingException {
		BufferedReader reader = new BufferedReader(new FileReader(filename));
		return parseData(reader);
	}

	private Spreadsheet parseData(BufferedReader reader) throws UnrecognizedEntryException, NumberFormatException,
			IncorrectValueTypeException, InvalidSpanException, IOException, SpreadsheetSizeException, ParsingException {
		int lineC = 0, colC = 0;
		String line;

		for (int i = 0; i < 2; i++) {
			line = reader.readLine();
			if (line.startsWith("linhas="))
				lineC = Integer.parseInt(line.substring(7));
			else if (line.startsWith("colunas="))
				colC = Integer.parseInt(line.substring(8));
			else
				throw new UnrecognizedEntryException(line);
		}

		if (lineC < 0 || colC < 0)
			throw new SpreadsheetSizeException("Spreadsheet lines and cols must not be negative.");
		else if (lineC == 0 || colC == 0)
			throw new SpreadsheetSizeException("Spreadsheet lines and cols must not be zero.");

		_sheet = new Spreadsheet(colC, lineC);

		while ((line = reader.readLine()) != null) {
			Cell cell = parseCellLine(line);
			_sheet.setCellContent(cell.getPosition(), cell.getContentCopy()); // TODO: Make better
		}

		return _sheet;
	}

	private Cell parseCellLine(String cellLine)
			throws UnrecognizedEntryException, NumberFormatException, ParsingException, InvalidSpanException {
		// Format: X;Y|content
		String[] parts = cellLine.split("\\|");
		CellValue value = null;
		if (parts.length == 2) {
			if (parts[1].length() != 0)
				value = parseCellValue(parts[1]);
		} else if (parts.length != 1)
			throw new UnrecognizedEntryException(cellLine);
		Position pos = Position.parse(parts[0]);
		Cell cell = new Cell(pos);
		cell.update(value);
		return cell;
	}

	private CellValue parseCellValue(String cellValue)
			throws UnrecognizedEntryException, NumberFormatException, ParsingException, InvalidSpanException {
		if (cellValue.startsWith("="))
			return parseExpression(cellValue);
		else
			return parseLiteral(cellValue);
	}

	private CellValue parseLiteral(String cellValue)
			throws UnrecognizedEntryException, NumberFormatException, InvalidSpanException {
		if (cellValue.charAt(0) == '\'')
			return new StringLiteral(cellValue.substring(1));
		else
			return new IntegerLiteral(Integer.parseInt(cellValue));
	}

	private CellValue parseExpression(String cellValue)
			throws UnrecognizedEntryException, ParsingException, InvalidSpanException {
		if (cellValue.contains("("))
			return parseFunction(cellValue);
		else
			return parseReference(cellValue);
	}

	private CellValue parseFunction(String cellValue)
			throws UnrecognizedEntryException, ParsingException, InvalidSpanException {
		cellValue = cellValue.substring(1); // Remove leading '='
		int firstParent = cellValue.indexOf("(");
		int lastParent = cellValue.indexOf(")");
		String name = cellValue.substring(0, firstParent);
		String args = cellValue.substring(firstParent + 1, lastParent);

		if (args.contains(",")) // Takes several args
			return parseBinaryFunction(name, args);
		else {
			return parseSpanFunction(name, args);
		}
	}

	private CellValue parseBinaryFunction(String name, String argBlob) throws UnrecognizedEntryException, ParsingException {
		String[] args = argBlob.split(",");
		if (args.length != 2)
			throw new ParsingException("Binary functions must have exactly 2 arguments.");
		BinaryArgument first = parseBinaryArgument(args[0]), second = parseBinaryArgument(args[1]);

		switch (name) {
		case "ADD":
			return new AddFunction(first, second);

		case "SUB":
			return new SubFunction(first, second);

		case "MUL":
			return new MulFunction(first, second);

		case "DIV":
			return new DivFunction(first, second);

		default:
			System.out.println("NVAL=" + name);
			throw new UnrecognizedEntryException(name);
		}
	}

	private CellValue parseSpanFunction(String name, String arg)
			throws UnrecognizedEntryException, NumberFormatException, ParsingException, InvalidSpanException {
		Span span = Span.parse(arg, _sheet);

		switch (name) {
		case "AVERAGE":
			return new AverageFunction(span);

		case "COALESCE":
			return new CoalesceFunction(span);

		case "CONCAT":
			return new ConcatFunction(span);

		case "PRODUCT":
			return new ProdFunction(span);

		default:
			throw new UnrecognizedEntryException(name);
		}
	}

	private CellValue parseReference(String cellValue) throws NumberFormatException {
		cellValue = cellValue.substring(1); // Remove leading '='
		Position pos = Position.parse(cellValue);

		return new ReferenceValue(pos, _sheet);
	}

	private BinaryArgument parseBinaryArgument(String arg) throws NumberFormatException {
		if (arg.contains(";"))
			return new BinaryArgument(Position.parse(arg), _sheet);
		else
			return new BinaryArgument(Integer.parseInt(arg));
	}
}
