#include "operatorData.h"
#include "typedefs.h"
#include "kernel\pdfoperators.h"
#include "kernel\cobjectsimple.h"
#include "kernel\ccontentstream.h"

using namespace pdfobjects;

OperatorData::OperatorData(PdfOp op) : _begin(0), _end(0), _ymin(0), _ymax(0), _charSpace(0.0f), _op(op), _origX(0), _origX2(0), _text("")
{
	std::wstring test;
	boost::shared_ptr<pdfobjects::TextSimpleOperator> txt = boost::dynamic_pointer_cast<TextSimpleOperator>(op);
	txt->getFontText(test);
	
	_text.assign(test.begin(),test.end());
	libs::Rectangle r = _op->getBBox();
	_ymin = min<double>(r.yleft, r.yright);
	_ymax = max<double>(r.yleft, r.yright);
	_begin = min<float>(r.xleft, r.xright);
	_end = max<float>(r.xleft, r.xright);
	_origX = _begin;
	_origX2 = _end; 
	_charSpace = _end - _begin;
	for ( size_t i =0; i< _text.size(); i++)
		_charSpace -= txt->getWidth(_text[i]);
	_charSpace/=_text.size();
}
float OperatorData::GetNextStop()
{
	int l = letters(_end);
	if (l>=_text.size())
		return _origX2;
	return position(l);
}
void OperatorData::change(bool from_beg)
{
	double b = _begin;
	double e = _end;
	clear();
	double subs = fabs(b - _begin)<1e-5? e:b;
	if (from_beg)
		_end = subs;
	else
		_begin = subs;
}
void OperatorData::clear()
{
	_begin = _origX;
	_end = _origX2;
}
void OperatorData::restoreBegin()
{
	_begin = _origX;
}
void OperatorData::restoreEnd()
{
	_end = _origX2;
}
void OperatorData::set(int x,double &place)
{
	place = x;
}
int OperatorData::letters(double x)
{
	double t = _origX;
	x = min(x,_origX2);
	int i =0;
	boost::shared_ptr<TextSimpleOperator> txt= boost::dynamic_pointer_cast<TextSimpleOperator>(_op);
	while (t<x)
	{
		t+= txt->getWidth(_text[i]);
		t+= this->_charSpace;
		i++;
	}
	return i;
}
double OperatorData::position(int letters)
{
	double place = _origX;
	boost::shared_ptr<TextSimpleOperator> txt= boost::dynamic_pointer_cast<TextSimpleOperator>(_op);
	for(int i = 0; i< letters; i++)
		place += txt->getWidth(_text[i]) + _charSpace;
	assert(place < _origX2+0.5f);
	return place;
}
void OperatorData::setMark(int x, bool beg)
{
	if (beg)
		setBegin(x);
	else
		setEnd(x);
}

void OperatorData::setBegin(int x)
{
	set(x,_begin);
}
void OperatorData::setEnd(int x)
{
	set(x,_end);
}
bool OperatorData::operator<(const OperatorData & oper) //zoradime podla y-osi
{
	BBox b = oper._op->getBBox();
	//cim vyssie je y, tym vyssie je na obrazovke, t.j. ty to bude prvsie
	//ak je rozdiel moc maly v y osi, si na jednej lajne
	return forward( b.xleft, b.yleft);
}
bool OperatorData::forward(double x, double y)const
{
	BBox a = _op->getBBox();
	float maxy = max(a.yleft, a.yright); //najvyssia hodnota -> najnizie

	if (fabs(maxy - y) > 0.5f) //rozhodni podla y osi, cim mensi, tym blizsie
	{
		return maxy - y < 0;//pojde dopredy ak toto je vyssie ako y, ktore sme dostali
	}
	maxy = min(a.xleft, a.xright);
	return maxy < x;
}
//split odla toho, ako sme to vysvietili
void OperatorData::split(QString & split1, QString& split2, QString& split3)
{
	std::wstring w;
	boost::shared_ptr<TextSimpleOperator> txt= boost::dynamic_pointer_cast<TextSimpleOperator>(_op);
	txt->getFontText(w);
	QString s  = QString::fromStdWString(w);
	BBox a = _op->getBBox();
	int part1 = letters(_begin);
	int part2 = letters(_end);
	split1= s.mid(0,part1);
	split2= s.mid(part1, part2-part1);
	split3= s.mid(part2);
}
void OperatorData::replaceAllText(QString s)
{
	PdfOperator::Operands ops;
	ops.push_back(boost::shared_ptr<IProperty>(new CString(s.toStdString())));
	PdfOp p = createOperator("Tj",ops);
	_op->getContentStream()->replaceOperator(_op,p);
	_op = p;
	_text = s.toStdString();
	clear();
}