package xxl.core;

import java.util.Comparator;

public class PositionComparator implements Comparator<Cell> {

	@Override
	public int compare(Cell arg0, Cell arg1) {
		Position a0 = arg0.getPosition();
		Position a1 = arg1.getPosition();

		if (a0.getX() == a1.getX())
		{
			if (a0.getY() == a1.getY())
				return 0;

			return a0.getY() - a1.getY();
		}

		return a0.getX() - a1.getX();
	}
	
}
