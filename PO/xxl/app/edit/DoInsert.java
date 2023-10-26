package xxl.app.edit;

import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.core.Spreadsheet;

/**
 * Class for inserting data.
 */
class DoInsert extends Command<Spreadsheet> {

	DoInsert(Spreadsheet receiver) {
		super(Label.INSERT, receiver);
		addStringField("span", Message.address());
		addStringField("content", Message.contents());
	}

	@Override
	protected final void execute() throws CommandException {
		_receiver.insertSpan(stringField("span"), stringField("content"));
	}
}
