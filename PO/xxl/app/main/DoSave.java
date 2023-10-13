package xxl.app.main;

import java.io.IOException;

import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.core.Calculator;
import xxl.core.exception.MissingFileAssociationException;

/**
 * Save to file under current name (if unnamed, query for name).
 */
class DoSave extends Command<Calculator> {

	DoSave(Calculator receiver) {
		super(Label.SAVE, receiver, xxl -> xxl.getSpreadsheet() != null);
		addStringField("filename", Message.newSaveAs());
	}

	@Override
	protected final void execute() throws CommandException {
		try {
			if (_receiver.getSpreadsheet().getFilename() == null) {
				_receiver.saveAs(stringField("filename"));
			} else {
				_receiver.save();
			}
		} catch (IOException | MissingFileAssociationException e) {
			System.err.println(e.getMessage());
		}

	}
}
