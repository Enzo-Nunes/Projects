package xxl.app.edit;

import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.app.exception.InvalidCellRangeException;
import xxl.core.Span;
import xxl.core.Spreadsheet;
import xxl.core.exception.InvalidSpanException;
import xxl.core.exception.ParsingException;
import xxl.core.exception.PositionOutOfRangeException;

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
			_receiver.pasteCutBuffer(Span.parse(stringField("span"), _receiver));
			_receiver.setDirty(true);
		} catch (ParsingException | InvalidSpanException | PositionOutOfRangeException e) {
			throw new InvalidCellRangeException(stringField("span"));
		}
	}
}
