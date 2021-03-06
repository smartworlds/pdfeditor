/*
 * PDFedit - free program for PDF document manipulation.
 * Copyright (C) 2006-2009  PDFedit team: Michal Hocko,
 *                                        Jozef Misutka,
 *                                        Martin Petricek
 *                   Former team members: Miroslav Jahoda
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in doc/LICENSE.GPL); if not, write to the 
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
 * MA  02111-1307  USA
 *
 * Project is hosted on http://sourceforge.net/projects/pdfedit
 */
// vim:tabstop=4:shiftwidth=4:noexpandtab:textwidth=80

// static
#include "kernel/static.h"
#include "kernel/pdfoperators.h"
//
#include "kernel/iproperty.h"
#include "kernel/cinlineimage.h"

#include "kernel/ccontentstream.h"
#include "kernel/stateupdater.h"
#include "kernel/factories.h"

//==========================================================
namespace pdfobjects {
//==========================================================

using namespace std;
using namespace boost;
using namespace debug;
using namespace utils;


//==========================================================
// Concrete implementations of PdfOperator
//==========================================================

//
//
//
SimpleGenericOperator::SimpleGenericOperator (const char* opTxt, 
											  const size_t numOper, 
											  Operands& opers) : _opText (opTxt)
{
		//utilsPrintDbg (debug::DBG_DBG, "Operator [" << opTxt << "] Operand size: " << numOper << " got " << opers.size());
		assert (numOper >= opers.size());
		if (numOper < opers.size())
			throw MalformedFormatExeption ("Operator operand size mismatch.");

	//
	// Store the operands and remove it from the stack
	// REMARK: the op count can vary ("scn" operator takes arbitrary number of
	// parameters)
	//
	for (size_t i = 0; (i < numOper) && !opers.empty(); ++i)
	{
		Operands::value_type val = opers.back ();
		// Store the last element of input parameter
		_operands.push_front (val);
		// Remove the element from input parameter
		opers.pop_back ();
	}
}

//
//
//
SimpleGenericOperator::SimpleGenericOperator (const std::string& opTxt, 
											  Operands& opers): _opText (opTxt)
{
		utilsPrintDbg (debug::DBG_DBG, opTxt);
	//
	// Store the operands and remove it from opers
	//
	while (!opers.empty())
	{
		// Store the last element of input parameter
		_operands.push_front ( opers.back() );
		// Remove the element from input parameter
		opers.pop_back ();
	}
}

//
//
//
SimpleGenericOperator::~SimpleGenericOperator() 
{
		// can happen when used as a temporary object
		if (0 < _operands.size() && !(_operandobserver)) 
			return;
	for (Operands::iterator it = _operands.begin(); it != _operands.end(); ++it) {
		UNREGISTER_SHAREDPTR_OBSERVER ((*it), _operandobserver);
	}
}


//
//
//
void 
SimpleGenericOperator::getStringRepresentation (std::string& str) const
{
	std::string tmp;
	for (Operands::const_iterator it = _operands.begin(); it != _operands.end (); ++it)
	{
		tmp.clear ();
		(*it)->getStringRepresentation (tmp);
		str += tmp + " ";
	}

	// Add operator string
	str += _opText;
}
	

//
//
//
shared_ptr<PdfOperator> 
SimpleGenericOperator::clone ()
{
	// Clone operands
	Operands ops;
	for (Operands::iterator it = _operands.begin (); it != _operands.end(); ++it)
		ops.push_back ((*it)->clone());
	assert (ops.size () == _operands.size());

	// Create clone
	return createOperator (_opText,ops);
}


void 
SimpleGenericOperator::init_operands (shared_ptr<observer::IObserver<IProperty> > observer, 
									  boost::weak_ptr<CPdf> pdf, 
									  IndiRef* rf)
{ 
	// store observer
	_operandobserver = observer; 
	//
	for (Operands::iterator oper = _operands.begin (); oper != _operands.end (); ++oper)
	{
		if (hasValidPdf(*oper))
		{ // We do not support adding operators from another stream
			if ( ((*oper)->getPdf().lock() != pdf.lock()) || !((*oper)->getIndiRef() == *rf) )
			{
				kernelPrintDbg (debug::DBG_ERR, "Pdf or indiref do not match: want " << *rf <<  " op has" <<(*oper)->getIndiRef());
				throw CObjInvalidObject ();
			}
			
		}else
		{
			(*oper)->setPdf (pdf);
			(*oper)->setIndiRef (*rf);
			REGISTER_SHAREDPTR_OBSERVER((*oper), observer);
			(*oper)->lockChange ();
		}
	} // for
}

namespace utils {
static std::string transformToCodeString(const std::string& what, const GfxFont *font)
{
	std::string out;
	for (unsigned i=0; i < what.length(); ++i) {
		int ch = what[i];
		CharCode code = ch;
		if (font)
			code = font->getCodeFromUnicode((const Unicode *)&ch, 1);
		out += (char)code;
	}
	return out;

}

}

void TextSimpleOperator::getRawText(std::string& str)const
{
using namespace utils;
	utilsPrintDbg(debug::DBG_DBG, "");
	std::string name, rawStr;
	getOperatorName(name);
	Operands ops;
	getParameters(ops);
	if(name == "'" || name == "Tj")
	{
		if(ops.size() != 1 || !isString(ops[0]))
		{
			utilsPrintDbg(debug::DBG_WARN, "Bad operands for operator "
					<<name<<" count="<<ops.size()
					<<" ops[0] type="<< ops[0]->getType());
			return;
		}
		rawStr = getStringFromIProperty(ops[0]);
	}
	else if (name == "\"")
	{
		if(ops.size() != 3 || !isArray(ops[2]))
		{
			utilsPrintDbg(debug::DBG_WARN, "Bad operands for operator "
					<<name<<" count="<<ops.size()
					<<" ops[2] type="<< ops[2]->getType());
			return;
		}
		rawStr = getStringFromIProperty(ops[2]);
	}
	else if (name == "TJ")
	{
		shared_ptr<IProperty> op = ops[0];
		if (!isArray(op) || ops.size() != 1)
		{
			utilsPrintDbg(debug::DBG_WARN, "Bad operands for TJ operator: ops[type="
					<< op->getType() <<" size="<<ops.size()<<"]");
			return;
		}
		shared_ptr<CArray> opArray = IProperty::getSmartCObjectPtr<CArray>(op);
		std::vector<shared_ptr<IProperty> > props;
		opArray->_getAllChildObjects(props);
		std::vector<shared_ptr<IProperty> >::iterator i;
		for(i=props.begin(); i!=props.end(); ++i)
		{
			shared_ptr<IProperty> p = *i;

			// TODO consider spacing coming from values
			if(!(isString(p)))
				continue;
			rawStr += getStringFromIProperty(p);
		}

	}else
	{
		utilsPrintDbg(debug::DBG_WARN, "Bad operator name="<<name);
		return;
	}

	str = rawStr;
}

void 
TextSimpleOperator::setFontText (const std::string& str)
{
	utilsPrintDbg(debug::DBG_DBG, "");
	std::string codeStr = utils::transformToCodeString(str, getCurrentFont());

	setRawText(codeStr);
}


/** Simple class for font data encapsulation.
 */
class TextSimpleOperator::FontData 
{
	string fontName;
	string fontTag;
public:
	FontData(GfxFont* font)
	{
		if (font->getName() && font->getName()->getCString())
			fontName = font->getName()->getCString();
		if (font->getTag() && font->getTag()->getCString())
			fontTag = font->getTag()->getCString();
	}

	const char * getFontName()const
	{
		return fontName.c_str();
	}

	const char * getFontTag()const
	{
		return fontTag.c_str();
	}
};

GfxFont* TextSimpleOperator::getCurrentFont()const
{
	assert(fontData);
	const char* tag = fontData->getFontTag();
	shared_ptr<GfxResources> res = getContentStream()->getResources(); 
	GfxFont* font = res->lookupFont(tag);
	if(!font)
		utilsPrintDbg(debug::DBG_ERR, "Unable to get font(name="
				<<fontData->getFontName()
				<<", tag="<<fontData->getFontTag()
				<<") for operator");
	return font;
}
float TextSimpleOperator::getFontHeight()const
{
	shared_ptr<PdfOperator> shr = this->_next().lock(); //looking for "Tc"
	FontOperatorIterator it = this->getIterator<FontOperatorIterator>(shr,false);
	boost::shared_ptr<PdfOperator> op = it.getCurrent();
	Operands operands;
	op->getParameters(operands);
	float fs = utils::getValueFromSimple<CReal>(operands[1]);
	return fs;
}
float TextSimpleOperator::getOper(const char * wanted, float def,int neg)
{
	std::string name;
	PdfOperator::Iterator it = this->_prev();
	int level=1;
	while (it.valid())
	{
		it.getCurrent()->getOperatorName(name);
		/*if (name == "BT")
			return 1;*/
		if (name == "Q" )//restore ->budeme ignrovat, pretoze hladame stav, v akom sme teraz
			level++;
		if(name == "q"&&level>0)
			level--;
		if (name == wanted && level==0) //-> musi to by ale v platnom Q,q
		{
			shared_ptr<PdfOperator> op = it.getCurrent();
			PdfOperator::Operands ops;
			op->getParameters(ops);
#if _DEBUG
			std::string m;
			op->getStringRepresentation(m);
#endif
			float f= utils::getValueFromSimple<CReal>(ops[0]);
			if (neg==0)
				return f; //we do not care about sign
			if (neg * f > 0)
				return f;//for horizontal writing
		}
		it.prev();
	}
	return def;
}

void TextSimpleOperator::getMatrix(float * values, boost::shared_ptr< PdfOperator>  beginOp)
{
	//memset(values,0, sizeof(float)*6);
	////find actual matrix
	//bool btProcessed = false;
	//PdfOperator::Iterator it = PdfOperator::getIterator(beginOp);
	//it.prev();
	//std::vector<float> vals;
	//std::string name;
	//while(it.valid())
	//{
	//	it.getCurrent()->getOperatorName(name);
	//	if (name == "Tm" && !btProcessed)
	//	{
	//		//apevt to Values
	//		for ( int i =0; i<6;i++);
	//	}
	//}
	//GfxState state;//reconstruct matrix
}
float TextSimpleOperator::getWidth(Unicode input)
{
	char chr = getCurrentFont()->getCodeFromUnicode(&input,1);
	CharCode c; Unicode u;int a; double dy,cc,d,dx;
	getCurrentFont()->getNextChar(&chr,1,&c,&u,(int)(sizeof(u) / sizeof(Unicode)),&a,&dx,&dy,&cc,&d);
	//najdi predosly operator s velkostou font
	shared_ptr<PdfOperator> shr = this->_prev().lock();
	//we need also all TM operator that operated with the width of glyph and also CM operators befor that
	FontOperatorIterator it = this->getIterator<FontOperatorIterator>(shr,false);
	boost::shared_ptr<PdfOperator> op = it.getCurrent();
	Operands operands;
	op->getParameters(operands);
#if _DEBUG
	std::string m;
	op->getStringRepresentation(m);
#endif
	float fs = utils::getValueFromSimple<CReal>(operands[1]);
	return dx*fs/actualTransform[0] + dy*fs/actualTransform[3]; //* displayparameter issue in x value
}

void TextSimpleOperator::getFontText(std::wstring& str)const
{
 	std::string rawStr;
	getRawText(rawStr);

	//rawStr = "Pr�lohy";
 	int len = rawStr.size();
 	GString raw(rawStr.c_str(), len);
 	GfxFont* font = getCurrentFont();
	if(!font)
		return;
	utilsPrintDbg(debug::DBG_INFO, "Textoperator uses font="<<fontData->getFontName());
 	CharCode code;
 	Unicode u;
 	int uLen;
 	double dx, dy, originX, originY;
 	char * p=raw.getCString();
 	while(len>0)
 	{
 		int n = font->getNextChar(p, len, &code, &u, (int)(sizeof(u) / sizeof(Unicode)), &uLen,
 			    &dx, &dy, &originX, &originY);
 		for (int i=0; i<uLen; ++i)
 			str += (&u)[i];
 		p += n;
 		len -= n;
  	}
}

TextSimpleOperator::~TextSimpleOperator()
{
	if(fontData)
		delete fontData;
}

const char* TextSimpleOperator::getFontName()const
{
	assert(fontData);
	return fontData->getFontName();
}

void TextSimpleOperator::setFontData(GfxFont* gfxFont)
{
	assert(gfxFont);
	if (!gfxFont)
	{
		utilsPrintDbg(debug::DBG_ERR, "Null font encountered");
		return;
	}
	if(fontData)
		delete fontData;
	fontData = new FontData(gfxFont);
}

void TextSimpleOperator::setTransformationMatrix( const double * param1 )
{
	memcpy(actualTransform,param1,sizeof(double)*6);
}

void TextSimpleOperator::concatTransformationMatrix( const double * param1 )
{
	double a1 = actualTransform[0];
	double b1 = actualTransform[1];
	double c1 = actualTransform[2];
	double d1 = actualTransform[3];
	int i;

	actualTransform[0] = param1[0] * a1 + param1[1] * c1;
	actualTransform[1] = param1[0] * b1 + param1[1] * d1;
	actualTransform[2] = param1[2] * a1 + param1[3] * c1;
	actualTransform[3] = param1[2] * b1 + param1[3] * d1;
	actualTransform[4] = param1[4] * a1 + param1[5] * c1 + actualTransform[4];
	actualTransform[5] = param1[4] * b1 + param1[5] * d1 + actualTransform[5];

	// avoid FP exceptions on badly messed up PDF files
	for (i = 0; i < 6; ++i) {
		if (actualTransform[i] > 1e10) {
			actualTransform[i] = 1e10;
		} else if (actualTransform[i] < -1e10) {
			actualTransform[i] = -1e10;
		}
	}
}

void TextSimpleOperator::clearPositions()
{
	_positions.clear();
}
libs::Point TextSimpleOperator::getPosition(int i, bool &ok)
{
	ok=true;
	if (i < _positions.size())
		return _positions[i];
	ok = false;
	return libs::Point();
}
void TextSimpleOperator::savePosition( double tdx, double tdy )
{
	_positions.push_back(libs::Point(tdx,tdy));
}

void TextSimpleOperator::setSubPartExclusive( int begin, int end )
{
	std::string rawStr;
	getRawText(rawStr);
	std::string res;
	
	int len = rawStr.size();
	GString raw(rawStr.c_str(), len);
	GfxFont* font = getCurrentFont();
	if(!font)
		return;
	utilsPrintDbg(debug::DBG_INFO, "Textoperator uses font="<<fontData->getFontName());
	CharCode code;
	Unicode u;
	int uLen;
	double dx, dy, originX, originY;
	char * p=raw.getCString();
	int cycles = 0;
	if (end <0)
		end = raw.getLength();
	while(len>0)
	{
		int n = font->getNextChar(p, len, &code, &u, (int)(sizeof(u) / sizeof(Unicode)), &uLen,
			&dx, &dy, &originX, &originY);
		if ( cycles < begin || cycles > end)
			for (int i=0; i<n; ++i)
				res += p[i];
		p += n;
		len -= n;
		cycles++;
	}
	PdfOperator::Operands ops;
	getParameters(ops);
	//change first operator
	//ops[0] = shared_ptr<IProperty>( CStringFactory::getInstance(res));
	setRawText(res);
}

void TextSimpleOperator::setRawText( std::string codeStr )
{
	std::string name;
	getOperatorName(name);

	Operands ops;
	getParameters(ops);
	if(name == "'" || name == "Tj")
	{
		if(ops.size() != 1 || !isString(ops[0]))
		{
			utilsPrintDbg(debug::DBG_WARN, "Bad operands for operator " <<name<<" count="<<ops.size()<<" ops[0] type="<< ops[0]->getType());
			return;
		}
		setValueToSimple<CString, pString>(ops[0], codeStr);
	}
	else if (name == "\"")
	{
		if(ops.size() != 3 || !isArray(ops[2]))
		{
			utilsPrintDbg(debug::DBG_WARN, "Bad operands for operator "<<name<<" count="<<ops.size()<<" ops[2] type="<< ops[2]->getType());
			return;
		}
		setValueToSimple<CString, pString>(ops[2], codeStr);
	}
	else if (name == "TJ")
	{
		shared_ptr<IProperty> op = ops[0];
		if (!isArray(op) || ops.size() != 1)
		{
			utilsPrintDbg(debug::DBG_WARN, "Bad operands for TJ operator: ops[type="<< op->getType() <<" size="<<ops.size()<<"]");
			return;
		}
		// We want to set a new text for this operator so let's 
		// forget about the original parameters along with the
		// formatting and add the given string as an only one
		// parameter in the array.
		if (isArray(op)) {
			shared_ptr<CArray> array = IProperty::getSmartCObjectPtr<CArray>(op);
			while (array->getPropertyCount() > 1)
				array->delProperty(array->getPropertyCount()-1);
			shared_ptr<IProperty> p = array->getProperty(0);
			setValueToSimple<CString, pString>(p, codeStr);
		}else
			setValueToSimple<CString, pString>(ops[0], codeStr);
		return;

	}else
	{
		utilsPrintDbg(debug::DBG_WARN, "Bad operator name="<<name);
		return;
	}
}

//==========================================================
// Concrete implementations of CompositePdfOperator
//==========================================================

//
// UnknownCompositePdfOperator
//

//
//
//
UnknownCompositePdfOperator::UnknownCompositePdfOperator 
	(const char* opBegin, const char* opEnd) : CompositePdfOperator (), _opBegin (opBegin), _opEnd (opEnd)
{
	utilsPrintDbg (DBG_DBG, "Unknown composite operator: " << _opBegin << " " << _opEnd);

}

//
//
//
void
UnknownCompositePdfOperator::getStringRepresentation (string& str) const
{
	// Header
	str += _opBegin; str += " ";
	
	// Delegate
	CompositePdfOperator::getStringRepresentation (str);	
}

//
//
//
shared_ptr<PdfOperator> 
UnknownCompositePdfOperator::clone ()
{
	shared_ptr<UnknownCompositePdfOperator> clone (new UnknownCompositePdfOperator(_opBegin,_opEnd));

	for (PdfOperators::iterator it = _children.begin(); it != _children.end(); ++it)
		clone->push_back ((*it)->clone(),getLastOperator(clone));
	
	// Create clone
	return clone;
}

//
// InlineImageCompositePdfOperator
//

size_t 
InlineImageCompositePdfOperator::getWidth() const 
{
  return _inlineimage->width();
}
size_t 
InlineImageCompositePdfOperator::getHeight() const
{
  return _inlineimage->height();
}


//
//
//
InlineImageCompositePdfOperator::InlineImageCompositePdfOperator 
	(boost::shared_ptr<CInlineImage> im, const char* opBegin, const char* opEnd) 
		: CompositePdfOperator (), _opBegin (opBegin), _opEnd (opEnd), _inlineimage (im)
{
	utilsPrintDbg (DBG_DBG, _opBegin << " " << _opEnd);
}

//
//
//
void
InlineImageCompositePdfOperator::getStringRepresentation (string& str) const
{
	// BI % Begin inline image object
	// /W 17 % Width in samples
	// /H 17 % Height in samples
	// /CS /RGB % Color space
	// /BPC 8 % Bits per component
	// /F [/A85 /LZW] % Filters
	// ID % Begin image data
	// J1/gKA>.]AN&J?]-<HW]aRVcg*bb.\eKAdVV%/PcZ
	// �Omitted data�
	// R.s(4KE3&d&7hb*7[%Ct2HCqC~>
	// EI

	// Header
	str += _opBegin; str += "\n";
	// 
	if (_inlineimage)
	{
		std::string tmp;
		_inlineimage->getStringRepresentation (tmp);	
		str += tmp;
	}else
	{
		assert (!"Bad inline image.");
		throw CObjInvalidObject ();
	}
	// Footer
	str += _opEnd; str += "\n";

}

//
//
//
void
InlineImageCompositePdfOperator::getParameters (Operands& opers) const
{
	boost::shared_ptr<IProperty> ip = _inlineimage;
	opers.push_back (ip);
}

//
//
//
shared_ptr<PdfOperator> 
InlineImageCompositePdfOperator::clone ()
{
	// Clone operands
	shared_ptr<CInlineImage> imgclone = IProperty::getSmartCObjectPtr<CInlineImage> (_inlineimage->clone());
	assert(imgclone->width() == _inlineimage->width());
	// Create clone
	return shared_ptr<PdfOperator> (new InlineImageCompositePdfOperator (imgclone, _opBegin, _opEnd));
}


//==========================================================
// Helper funcions
//==========================================================

boost::shared_ptr<PdfOperator> createOperator(const std::string& name, PdfOperator::Operands& operands)
{
	if (name == "BI")
		throw NotImplementedException("Inline images not implemented here");

	// Try to find the op by its name
	const StateUpdater::CheckTypes* chcktp = StateUpdater::findOp (name.c_str());
	// Operator not found, create unknown operator
	if (NULL == chcktp)
		return shared_ptr<PdfOperator> (new SimpleGenericOperator (name ,operands));
	
	assert (chcktp);
	utilsPrintDbg (DBG_DBG, "Operator found. " << chcktp->name);
	// Check the type against specification
	// 
	if (!checkAndFixOperator (*chcktp, operands))
	{
		//assert (!"Content stream bad operator type.");
		throw ElementBadTypeException ("Content stream operator has incorrect operand type.");
	}
	
	// Get operands count
	size_t argNum = static_cast<size_t> ((chcktp->argNum > 0) ? chcktp->argNum : -chcktp->argNum);

	//
	// If endTag is "" it is a simple operator, composite otherwise
	// 
	if (isTextOp(*chcktp))
		return shared_ptr<PdfOperator> (new TextSimpleOperator(chcktp->name, argNum, operands));

	if (isSimpleOp(*chcktp))
		return shared_ptr<PdfOperator> (new SimpleGenericOperator (chcktp->name, argNum, operands));
		
	// Composite operator
	return shared_ptr<PdfOperator> (new UnknownCompositePdfOperator (chcktp->name, chcktp->endTag));

}

boost::shared_ptr<PdfOperator> createOperator(const char *name, PdfOperator::Operands& operands)
{
	std::string n = name;
	return createOperator(n, operands);
}

boost::shared_ptr<PdfOperator> createOperatorTranslation (double x, double y) 
{
	PdfOperator::Operands ops;
	ops.push_back (boost::shared_ptr<IProperty>(new CReal (1)));
	ops.push_back (boost::shared_ptr<IProperty>(new CReal (0)));
	ops.push_back (boost::shared_ptr<IProperty>(new CReal (0)));
	ops.push_back (boost::shared_ptr<IProperty>(new CReal (1)));
	ops.push_back (boost::shared_ptr<IProperty>(new CReal (x)));
	ops.push_back (boost::shared_ptr<IProperty>(new CReal (y)));
	return createOperator("cm", ops);
}


boost::shared_ptr<PdfOperator> createOperatorScale (double width, double height) 
{
	PdfOperator::Operands ops;
	ops.push_back (boost::shared_ptr<IProperty>(new CReal (width)));
	ops.push_back (boost::shared_ptr<IProperty>(new CReal (0)));
	ops.push_back (boost::shared_ptr<IProperty>(new CReal (0)));
	ops.push_back (boost::shared_ptr<IProperty>(new CReal (height)));
	ops.push_back (boost::shared_ptr<IProperty>(new CReal (0)));
	ops.push_back (boost::shared_ptr<IProperty>(new CReal (0)));
	return createOperator("cm", ops);
}
boost::shared_ptr<PdfOperator> createOperatorRotation (double radians) 
{
	double cs = cos(radians);
	double sn = sin(radians);
	PdfOperator::Operands ops;
	ops.push_back (boost::shared_ptr<IProperty>(new CReal (cs)));
	ops.push_back (boost::shared_ptr<IProperty>(new CReal (sn)));
	ops.push_back (boost::shared_ptr<IProperty>(new CReal (-1*sn)));
	ops.push_back (boost::shared_ptr<IProperty>(new CReal (cs)));
	ops.push_back (boost::shared_ptr<IProperty>(new CReal (0)));
	ops.push_back (boost::shared_ptr<IProperty>(new CReal (0)));
	return createOperator("cm", ops);
}
boost::shared_ptr<PdfOperator> createOperatorText (boost::shared_ptr<CContentStream> &cc,
		const std::string &fontName, const std::string &op, const std::string &text)
{
	utilsPrintDbg(debug::DBG_DBG, "");

	boost::shared_ptr<GfxResources> res = cc->getResources();
	GfxFont *font = res->lookupFont(fontName.c_str());
	std::string encText;
	if (font) {
		encText = transformToCodeString(text, font);
	}
	else {
		encText = text;
		utilsPrintDbg(debug::DBG_WARN, "No font found for "+fontName);
	}

	PdfOperator::Operands ops;

	if (op == "'" || op == "Tj") {
		ops.push_back(boost::shared_ptr<IProperty>(CStringFactory::getInstance(encText)));
	} else if (op == "TJ") {
		boost::shared_ptr<CArray> array(CArrayFactory::getInstance());
		boost::shared_ptr<IProperty> str(CStringFactory::getInstance(encText));
		array->addProperty(*str);
		ops.push_back(array);
	} else if (op == "\"") {
		utilsPrintDbg(debug::DBG_WARN, op+" text operator is not supported");
		throw NotImplementedException(op +" text operator is not supported");

	}else {
		utilsPrintDbg(debug::DBG_ERR, "Unknown text operator "+op);
		throw ElementBadTypeException("Unknown text operator "+op);
	}
	return createOperator(op, ops);
}

//
//\todo improve performance
//
boost::shared_ptr<CompositePdfOperator>
findCompositeOfPdfOperator (PdfOperator::Iterator it, boost::shared_ptr<PdfOperator> oper)
{
	boost::shared_ptr<CompositePdfOperator> composite;
	typedef PdfOperator::PdfOperators Opers;
	Opers opers;


	while (!it.isEnd())
	{
		// Have we found what we were looking for
		if (isCompositeOp(it))
		{
			opers.clear ();
			it.getCurrent()->getChildren (opers);
			if (opers.end() != std::find (opers.begin(), opers.end(), oper))
				return boost::dynamic_pointer_cast<CompositePdfOperator, PdfOperator> (it.getCurrent());
		}else
		{
			// This can happen only in the "first level" but that should be
			// handled in caller
			if (it.getCurrent() == oper)
			{
				assert (!"Found highest level operator, that should be handled in the caller of this function.");
				throw CObjInvalidObject ();
			}
		}

		it.next ();
	}

	//
	// We should have found the operator
	// -- this can happen in an incorrect script
	// 		that remembers reference to a removed object
	// 
	//assert (!"Operator not found...");
	throw CObjInvalidOperation ();
}


	
//==========================================================
} // namespace pdfobjects
//==========================================================
