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
 * Copy command.
 */
class DoCopy extends Command<Spreadsheet> {

	DoCopy(Spreadsheet receiver) {
		super(Label.COPY, receiver);
		addStringField("span", Message.address());
	}

	@Override
	protected final void execute() throws CommandException {
		try {
			_receiver.updateCutBuffer(Span.parse(stringField("span"), _receiver));
		} catch (ParsingException | PositionOutOfRangeException | InvalidSpanException e) {
			throw new InvalidCellRangeException(stringField("span"));
		}
	}
}
