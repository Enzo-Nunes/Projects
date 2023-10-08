package xxl.UMLracisms;

public class Spreadsheet {
	User[]		_owners;
	Cell[]		_cells;
	CutBuffer	_cutBuffer;

	public Spreadsheet(User owner, int width, int height) {
		_owners = new User[] { owner };
		// FIXME implement cell creation
	}

	public void addOwner(User owner) {
		// FIXME implement method
	}
	
	public void removeOwner(User owner) {
		// FIXME implement method
	}
}
