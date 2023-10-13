package xxl.core;

class Position {
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

	public String visualize()
	{
		return _posX + ";" + _posY;
	}
}
