package xxl.core;

import java.io.Serializable;

class Position implements Serializable {
	private int _posX, _posY;

	public Position(int positionX, int positionY) {
		_posX = positionX;
		_posY = positionY;
	}

	public static Position parse(String src) throws NumberFormatException {
		String[] parts = src.split(";");
		// TODO: Check length
		return new Position(Integer.parseInt(parts[1]), Integer.parseInt(parts[0]));
	}

	public int getX() {
		return _posX;
	}

	public int getY() {
		return _posY;
	}

	@Override
	public int hashCode() {
		return visualize().hashCode();
	}

	@Override
	public boolean equals(Object other) {
		Position pos;
		try {
			pos = (Position) other;
		} catch (ClassCastException e) {
			return false;
		}

		return _posX == pos._posX && _posY == pos._posY;
	}

	public String visualize() {
		return _posY + ";" + _posX;
	}
}
