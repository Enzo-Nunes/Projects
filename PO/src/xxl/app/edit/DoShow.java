package xxl.app.edit;

import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.core.Spreadsheet;
import xxl.core.exception.ParsingException;
import xxl.core.Span;
import xxl.app.exception.InvalidCellRangeException;
import xxl.core.Cell;

/**
 * Class for searching functions.
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
    } catch (ParsingException e) {
      throw new InvalidCellRangeException(range);
    }
    
    for (Cell cell : span)
      _display.addLine(cell.visualize());

    _display.display();
  }
}
