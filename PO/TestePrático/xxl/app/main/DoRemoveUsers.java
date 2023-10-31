package xxl.app.main;

import java.util.ArrayList;

import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.core.Calculator;
import xxl.core.User;

/**
 * Remove Users.
 */
class DoRemoveUsers extends Command<Calculator> {

	DoRemoveUsers(Calculator receiver) {
		super("Remover Utilizadores", receiver);
		addStringField("username", "Insira o nome dos utilizadores a remover: ");
	}

	@Override
	protected final void execute() throws CommandException {
		// int numMatches = _receiver.getUsers("username");
		// _display.addNewLine("Foram removidos" + numMatches + "utilizadores.", false);
		ArrayList<User> matches = _receiver.removeUsers("username");
		_display.addNewLine("Foram removidos" + matches.size() + "utilizadores", false);
		_display.addNewLine("Foram removidos estes utilizadores: ", false);
		for (User user: matches) {
			_display.addNewLine(user.getName(), false);
		}
	}
}
