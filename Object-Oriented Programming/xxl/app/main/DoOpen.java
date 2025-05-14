package xxl.app.main;

import java.io.IOException;

import pt.tecnico.uilib.forms.Form;
import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.app.exception.FileOpenFailedException;
import xxl.core.exception.UnavailableFileException;
import xxl.core.Calculator;

/**
 * Open existing file.
 */
class DoOpen extends Command<Calculator> {

	DoOpen(Calculator receiver) {
		super(Label.OPEN, receiver);
		addStringField("filename", Message.openFile());
	}

	@Override
	protected final void execute() throws CommandException {
		try {
			if (_receiver.getSpreadsheet() != null && _receiver.getSpreadsheet().isDirty()) {
				if (Form.confirm(Message.saveBeforeExit())) {
					Command<Calculator> doSave = new DoSave(_receiver);
					doSave.performCommand();
				}
			}
			_receiver.load(stringField("filename"));
		} catch (UnavailableFileException | IOException | ClassNotFoundException e) {
			throw new FileOpenFailedException(e);
		}

	}
}
