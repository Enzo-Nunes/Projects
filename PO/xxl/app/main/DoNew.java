package xxl.app.main;

import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.core.Calculator;

/**
 * Open a new file.
 */
class DoNew extends Command<Calculator> {

	DoNew(Calculator receiver) {
		super(Label.NEW, receiver);
		addIntegerField("numRows", Message.lines());
		addIntegerField("numColumns", Message.columns());
	}

	@Override
	protected final void execute() throws CommandException {
		//TODO: ask teacher. Is this supposed to be verified this way? Where is readBoolean()?
		if (_receiver.getSpreadsheet() == null || !_receiver.getSpreadsheet().isDirty()) {
			_receiver.createNewSpreadsheet(integerField("numRows"), integerField("numColumns"));
		} else {
			addBooleanField("saveBeforeExit", Message.saveBeforeExit());
		}
	}
}
