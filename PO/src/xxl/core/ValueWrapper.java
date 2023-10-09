package xxl.core;

public class ValueWrapper {
	private int _int;
	private String _string;

	public ValueWrapper(int integer)
	{
		_int = integer;
	}

	public ValueWrapper(String string)
	{
		_string = string;
	}

	public String getString() throws Exception
	{
		if (_string == null)
			throw new Exception(); //TODO: Replace with better exception

		return _string;
	}

	public int getInt() throws Exception
	{
		if (_string != null)
			throw new Exception(); //TODO: Replace with better exception

		return _int;
	}
}
