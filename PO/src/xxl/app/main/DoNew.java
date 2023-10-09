package xxl.app.main;

import pt.tecnico.uilib.forms.Form;
import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.core.Calculator;
import xxl.core.Spreadsheet;

/**
 * Open a new file.
 */
class DoNew extends Command<Calculator> {

	DoNew(Calculator receiver) {
		super(Label.NEW, receiver);
	}

	@Override
	protected final void execute() throws CommandException {
		int numRows = Form.requestInteger(Message.lines());
		int numCols = Form.requestInteger(Message.columns());

		_receiver.setSpreadsheet(new Spreadsheet(numRows, numCols));
	}
}
