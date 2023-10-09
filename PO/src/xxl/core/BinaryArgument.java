package xxl.core;

public class BinaryArgument {
	private int _literal;
	private Cell _refrence;

	public BinaryArgument(int literal)
	{
		_literal = literal;
	}

	public BinaryArgument(Cell reference)
	{
		_refrence = reference;
	}

	public int getValue() throws Exception
	{
		if (_refrence == null)
			return _literal;

		return _refrence.getValue().getInt();
	}
}
