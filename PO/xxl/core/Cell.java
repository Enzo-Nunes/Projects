package xxl.core;

import java.io.Serializable;
import java.util.ArrayList;

import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.PositionOutOfRangeException;

public class Cell implements Serializable {
	private Position _position;
	private CellValue _content;
	private ArrayList<Observer> _subscribers;



	public Cell(Position pos) {
		_position = pos;
		_subscribers = new ArrayList<Observer>();
	}

	public Position getPosition() {
		return _position;
	}

	public CellValue getContentCopy() {
		if (_content == null)
			return null;
		return _content.deepCopy();
	}

	public void updateValue(CellValue value) {
		_content = value;
		update();
	}

	public ValueWrapper getValue() throws IncorrectValueTypeException, PositionOutOfRangeException {
		if (_content == null)
			return null;
		return _content.getValue();
	}

	public String visualize() {
		if (_content == null)
			return _position.visualize() + "|";
		return _position.visualize() + "|" + _content.visualize();
	}



	public void update()
	{
		if (_content == null)
			return;
			
		_content.recalculate();

		for (Observer obs : _subscribers)
			obs.update();
	}

	public void subscribe(Observer observer)
	{
		//FIXME: Maybe? Handle case where observed cell may not have been created yet
		_subscribers.add(observer);
	}

	public void unsubscribe(Observer observer)
	{
		_subscribers.remove(observer);
	}

	public void unsubscribeFromAll()
	{
		if (_content != null)
			_content.unsubscribeFromAll();
	}
}
