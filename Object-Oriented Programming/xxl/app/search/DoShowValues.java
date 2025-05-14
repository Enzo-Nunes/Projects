package xxl.app.search;

import pt.tecnico.uilib.menus.Command;
import xxl.core.Spreadsheet;

/**
 * Command for searching content values.
 */
class DoShowValues extends Command<Spreadsheet> {

	DoShowValues(Spreadsheet receiver) {
		super(Label.SEARCH_VALUES, receiver);
		addStringField("pattern", Message.searchValue());
	}

	@Override
	protected final void execute() {
		String result = _receiver.searchCellValues(stringField("pattern"));
		_display.addNewLine(result, false);
	}
}
