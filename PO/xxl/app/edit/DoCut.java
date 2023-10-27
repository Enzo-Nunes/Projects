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
 * Cut command.
 */
class DoCut extends Command<Spreadsheet> {

	DoCut(Spreadsheet receiver) {
		super(Label.CUT, receiver);
		addStringField("span", Message.address());
	}

	@Override
	protected final void execute() throws CommandException {
		try {
			Span span = Span.parse(stringField("span"), _receiver);
			_receiver.updateCutBuffer(span);
			_receiver.clearSpan(span);
		} catch (ParsingException | PositionOutOfRangeException | InvalidSpanException e) {
			throw new InvalidCellRangeException(stringField("span"));
		}
	}
}
