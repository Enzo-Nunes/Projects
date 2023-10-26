package xxl.app.edit;

import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.app.exception.InvalidCellRangeException;
import xxl.app.exception.UnknownFunctionException;
import xxl.core.Spreadsheet;
import xxl.core.exception.InvalidSpanException;
import xxl.core.exception.ParsingException;
import xxl.core.exception.UnrecognizedEntryException;

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
		try {
			_receiver.insertSpan(stringField("span"), stringField("content"));
		} catch (ParsingException | InvalidSpanException e) {
			throw new InvalidCellRangeException(stringField("span")); //TODO: Review
		} catch (UnrecognizedEntryException e) {
			throw new UnknownFunctionException(stringField("content")); //TODO: Review
		}
	}
}
