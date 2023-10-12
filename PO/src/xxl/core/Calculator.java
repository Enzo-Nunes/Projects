package xxl.core;

import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.ObjectOutputStream;
import java.io.FileInputStream;
import java.io.ObjectInputStream;

import xxl.core.exception.ImportFileException;
import xxl.core.exception.IncorrectValueTypeException;
import xxl.core.exception.MissingFileAssociationException;
import xxl.core.exception.PositionOutOfRangeException;
import xxl.core.exception.UnavailableFileException;
import xxl.core.exception.UnrecognizedEntryException;

/**
 * Class representing a spreadsheet application.
 */
public class Calculator {
	/** The current spreadsheet. */
	private Spreadsheet _spreadsheet;
	private User _currentUser;
	private User[] _users;
	private static User _root;
	//TODO: mess with users stuff. Maybe create root when creating calculator? When to add new users?

	/**
	 * Return the current spreadsheet.
	 *
	 * @returns the current spreadsheet of this application. This reference can be
	 *          null.
	 */
	public final Spreadsheet getSpreadsheet() {
		return _spreadsheet;
	}

	// public void setSpreadsheet(Spreadsheet spreadsheet) {
	// _spreadsheet = spreadsheet;
	// }

	public void createNewSpreadsheet(int rows, int columns) {
		_spreadsheet = new Spreadsheet(rows, columns);
	}

	/**
	 * Saves the serialized application's state into the file associated to the
	 * current network.
	 *
	 * @throws FileNotFoundException           if for some reason the file cannot be
	 *                                         created or opened.
	 * @throws MissingFileAssociationException if the current network does not have
	 *                                         a file.
	 * @throws IOException                     if there is some error while
	 *                                         serializing the state of the network
	 *                                         to disk.
	 */
	public void save() throws FileNotFoundException, MissingFileAssociationException, IOException {
		try (ObjectOutputStream obOut = new ObjectOutputStream(new FileOutputStream(_spreadsheet.getFilename()));) {
			obOut.writeObject(_spreadsheet);
		}
	}

	/**
	 * Saves the serialized application's state into the specified file. The current
	 * network is
	 * associated to this file.
	 *
	 * @param filename the name of the file.
	 * @throws FileNotFoundException           if for some reason the file cannot be
	 *                                         created or opened.
	 * @throws MissingFileAssociationException if the current network does not have
	 *                                         a file.
	 * @throws IOException                     if there is some error while
	 *                                         serializing the state of the network
	 *                                         to disk.
	 */
	public void saveAs(String filename) throws FileNotFoundException, MissingFileAssociationException, IOException {
		try (ObjectOutputStream obOut = new ObjectOutputStream(new FileOutputStream(filename));) {
			obOut.writeObject(_spreadsheet);
			_spreadsheet.setFilename(filename);
		}
	}

	/**
	 * @param filename name of the file containing the serialized application's
	 *                 state
	 *                 to load.
	 * @throws UnavailableFileException if the specified file does not exist or
	 *                                  there is
	 *                                  an error while processing this file.
	 */
	public void load(String filename) throws UnavailableFileException, IOException, ClassNotFoundException {
		try (ObjectInputStream obIn = new ObjectInputStream(new FileInputStream(filename));) {
			Object temp = obIn.readObject();
			_spreadsheet = (Spreadsheet) temp;
		}
	}

	/**
	 * Read text input file and create domain entities.
	 *
	 * @param filename name of the text input file
	 * @throws ImportFileException
	 */
	public void importFile(String filename) throws ImportFileException {
		try {
			_spreadsheet = new Parser().parseFromFile(filename);
		} catch (IOException | UnrecognizedEntryException | NumberFormatException
				| IncorrectValueTypeException | PositionOutOfRangeException e) {
			throw new ImportFileException(filename, e);
		}
	}
}
