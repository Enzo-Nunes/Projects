package xxl.app.edit;

import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.core.Spreadsheet;
import xxl.core.exception.InvalidSpanException;
import xxl.core.exception.ParsingException;
import xxl.core.Span;
import xxl.app.exception.InvalidCellRangeException;
import xxl.core.Cell;

/**
 * Show span command.
 */
class DoShow extends Command<Spreadsheet> {

	DoShow(Spreadsheet receiver) {
		super(Label.SHOW, receiver);
		addStringField("range", Message.address());
	}

	@Override
	protected final void execute() throws CommandException {
		String range = stringField("range");
		Span span;
		try {
			span = Span.parse(range, _receiver);
		} catch (ParsingException | InvalidSpanException e) {
			throw new InvalidCellRangeException(range);
		}

		for (Cell cell : span) {
			if (cell != null)
				_display.addNewLine(cell.visualize(), false);
		}
	}
}
