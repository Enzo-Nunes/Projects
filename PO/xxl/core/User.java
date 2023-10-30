package xxl.core;

import java.io.Serializable;
import java.util.List;
import java.util.ArrayList;
import java.util.Collections;

public class User implements Serializable {
	private final String _name;
	private List<Spreadsheet> _spreadsheets = new ArrayList<>();

	public User(String name) {
		_name = name;
	}

	public final String getName() {
		return _name;
	}

	void add(Spreadsheet s) {
		_spreadsheets.add(s);
	}

	public List<Spreadsheet> getSpreadSheets() {
		return Collections.unmodifiableList(_spreadsheets);
	}

}
