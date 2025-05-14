package xxl.app.main;

import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import pt.tecnico.uilib.forms.Form;
import xxl.core.Calculator;

/**
 * Open a new file.
 */
class DoNew extends Command<Calculator> {

	DoNew(Calculator receiver) {
		super(Label.NEW, receiver);
		addIntegerField("numRows", Message.lines());
		addIntegerField("numColumns", Message.columns());
	}

	@Override
	protected final void execute() throws CommandException {
		if (_receiver.getSpreadsheet() != null && _receiver.getSpreadsheet().isDirty()) {
			if (Form.confirm(Message.saveBeforeExit())) {
				Command<Calculator> doSave = new DoSave(_receiver);
				doSave.performCommand();
			}
		}
		_receiver.createNewSpreadsheet(integerField("numRows"), integerField("numColumns"));
	}
}
