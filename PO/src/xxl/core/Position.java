package xxl.core;

class Position {
	private int _posX, _posY;

	public Position(int positionX, int positionY) {
		_posX = positionX;
		_posY = positionY;
	}

	public int getX() {
		return _posX;
	}

	public int getY() {
		return _posY;
	}
}
