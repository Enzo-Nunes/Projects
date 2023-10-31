package xxl.app.main;

import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import pt.tecnico.uilib.forms.Form;
import xxl.app.exception.InvalidCellRangeException;
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
	protected final void execute() throws CommandException, InvalidCellRangeException{
		int linhas = integerField("numRows");
		int colunas = integerField("numColumns");

		if (linhas <= 2 * colunas) {
			throw new InvalidCellRangeException(linhas + ";" + colunas);
		} 

		if (_receiver.getSpreadsheet() != null && _receiver.getSpreadsheet().isDirty()) {
			if (Form.confirm(Message.saveBeforeExit())) {
				Command<Calculator> doSave = new DoSave(_receiver);
				doSave.performCommand();
			}
		}
		_receiver.createNewSpreadsheet(linhas, colunas);
	}
}
