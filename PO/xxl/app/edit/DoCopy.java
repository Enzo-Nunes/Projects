package xxl.app.edit;

import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.app.exception.InvalidCellRangeException;
import xxl.core.Span;
import xxl.core.Spreadsheet;
import xxl.core.exception.InvalidSpanException;
import xxl.core.exception.ParsingException;

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
			Span span = Span.parse(stringField("span"), _receiver);
			_receiver.updateCutBuffer(span);
			_receiver.clearSpan(span);
		} catch (ParsingException | InvalidSpanException e) {
			throw new InvalidCellRangeException(Message.address());
		}
	}
}
