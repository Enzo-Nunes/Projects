package xxl.core;

import java.io.Serializable;
import xxl.core.exception.IncorrectValueTypeException;

public class ValueWrapper implements Serializable {
	private int _int;
	private String _string;

	public ValueWrapper(int integer) {
		_int = integer;
	}

	public ValueWrapper(String string) {
		_string = string;
	}

	public String getString() throws IncorrectValueTypeException {
		if (_string == null)
			throw new IncorrectValueTypeException();

		return _string;
	}

	public int getInt() throws IncorrectValueTypeException {
		if (_string != null)
			throw new IncorrectValueTypeException(_string);

		return _int;
	}

	public String visualize() {
		if (_string != null)
			return "'" + _string;
		return "" + _int;
	}

	@Override
	public boolean equals(Object object) {
		if (!(object instanceof ValueWrapper))
			return false;

		ValueWrapper other = (ValueWrapper) object;

		if (other._string == null && _string != null)
			return false;
		if (other._string != null && _string == null)
			return false;

		if (other._int != _int)
			return false;

		if (other._string == null && _string == null)
			return true;

		return _string.equals(other._string);
	}
}
