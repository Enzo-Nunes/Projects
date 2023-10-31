package xxl.app.search;

import pt.tecnico.uilib.menus.Command;
import xxl.core.Spreadsheet;

/**
 * Command for searching function names.
 */
class DoShowFunctions extends Command<Spreadsheet> {

	DoShowFunctions(Spreadsheet receiver) {
		super(Label.SEARCH_FUNCTIONS, receiver);
		addStringField("pattern", Message.searchFunction());
	}

	@Override
	protected final void execute() {
		String result = _receiver.searchCellFunctions(stringField("pattern"));
		_display.addNewLine(result, false);
	}
}
