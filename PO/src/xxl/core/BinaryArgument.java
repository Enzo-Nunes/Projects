package xxl.core;

public class BinaryArgument {
	private int _literal;
	private Position _referencedPos;
	private Spreadsheet _sheet;

	public BinaryArgument(int literal)
	{
		_literal = literal;
	}

	public BinaryArgument(Position referencedPosition, Spreadsheet containingSheet)
	{
		_referencedPos = referencedPosition;
		_sheet = containingSheet;
	}

	public int getValue() throws Exception
	{
		if (_referencedPos == null)
			return _literal;

		return _sheet.getCellContent(_referencedPos).getInt();
	}
}
