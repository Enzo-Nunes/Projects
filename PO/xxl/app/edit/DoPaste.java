package xxl.app.edit;

import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.app.exception.InvalidCellRangeException;
import xxl.core.Span;
import xxl.core.Spreadsheet;
import xxl.core.exception.InvalidSpanException;
import xxl.core.exception.ParsingException;

/**
 * Paste command.
 */
class DoPaste extends Command<Spreadsheet> {

	DoPaste(Spreadsheet receiver) {
		super(Label.PASTE, receiver);
		addStringField("span", Message.address());
	}

	@Override
	protected final void execute() throws CommandException {
		try {
			_receiver.pasteCutbuffer(Span.parse(stringField("span"), _receiver));
		} catch (ParsingException | InvalidSpanException e) {
			throw new InvalidCellRangeException(Message.address());
		}
	}
}
