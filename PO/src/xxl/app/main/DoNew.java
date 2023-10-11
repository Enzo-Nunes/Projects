package xxl.app.main;

import pt.tecnico.uilib.forms.Form;
import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.core.Calculator;
import xxl.core.Spreadsheet;
import xxl.core.User;

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
		_receiver.createNewSpreadsheet(integerField("numRows"), integerField("numColumns"));
	}
}
