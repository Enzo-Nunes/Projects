package xxl.core;

import xxl.core.exception.UnrecognizedEntryException;

class Parser {
	Spreadsheet _sheet;

	public Parser(Spreadsheet spreadsheet) {
		_sheet = spreadsheet;
	}

	private CellValue parseCellValue(String cellValue) throws UnrecognizedEntryException, NumberFormatException {
		if (cellValue.startsWith("="))
			return parseExpression(cellValue);
		else
			return parseLiteral(cellValue);
	}

	private CellValue parseLiteral(String cellValue) throws UnrecognizedEntryException, NumberFormatException {
		cellValue = cellValue.substring(1); //Remove leading '='
		if (Character.isDigit(cellValue.charAt(0)))
			return new IntegerLiteral(Integer.parseInt(cellValue));
		else
			return new StringLiteral(cellValue);
	}

	private CellValue parseExpression(String cellValue) throws UnrecognizedEntryException {
		if (cellValue.contains("("))
			return parseFunction(cellValue);
		else
			return parseReference(cellValue);
	}

	private CellValue parseFunction(String cellValue) throws UnrecognizedEntryException {
		int firstParent = cellValue.indexOf("(");
		int lastParent = cellValue.indexOf(")");
		String name = cellValue.substring(0, firstParent);
		String args = cellValue.substring(firstParent+1, lastParent-firstParent);

		if (args.contains(",")) //Takes several args
			return parseBinaryFunction(name, args);
		else
			return parseSpanFunction(name, args);
	}

	private CellValue parseBinaryFunction(String name, String argBlob) throws UnrecognizedEntryException {
		String[] args = argBlob.split(",");
		//TODO: Validate length
		BinaryArgument first = parseBinaryArgument(args[0]),
			second = parseBinaryArgument(args[1]);

		switch (name)
		{
			case "ADD":
				return new AddFunction(first, second);

			case "SUB":
				return new SubFunction(first, second);

			case "MUL":
				return new MulFunction(first, second);

			case "DIV":
				return new DivFunction(first, second);

			default:
				throw new UnrecognizedEntryException(name);
		}
	}

	private CellValue parseSpanFunction(String name, String arg) throws UnrecognizedEntryException, NumberFormatException {
		Span span = parseSpan(arg);

		switch (name)
		{
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
		Position pos = parsePosition(cellValue);

		return new ReferenceValue(pos, _sheet);
	}

	private BinaryArgument parseBinaryArgument(String arg) throws NumberFormatException
	{
		if (arg.contains(";"))
			return new BinaryArgument(parsePosition(arg), _sheet);
		else
			return new BinaryArgument(Integer.parseInt(arg));
	}

	private Span parseSpan(String src) throws NumberFormatException {
		String[] parts = src.split(":");
		//TODO: Check length
		return new Span(parsePosition(parts[0]), parsePosition(parts[1]), _sheet);
	}

	private Position parsePosition(String src) throws NumberFormatException {
		String[] parts = src.split(";");
		//TODO: Check length
		return new Position(Integer.parseInt(parts[0]), Integer.parseInt(parts[1]));
	}
}
