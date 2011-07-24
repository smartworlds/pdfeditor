#include "Search.h"
#include <string.h>

Search::Search(QWidget * parent) : QWidget(parent),_ignoreCase(false),_wholeWord(false),_regexp(false)
{
	ui.setupUi(this);
}
void Search::next()
{
	emit search(ui.text->text(),true);
}
void Search::prev()
{
	emit search(ui.text->text(),false);
}
void Search::replace()
{
//	emit replaceTextSignal(this->ui.text->toPlainText(),this->ui.replacetext->toPlainText());
}
