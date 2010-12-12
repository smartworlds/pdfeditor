#ifndef __FONTE_WIDGET__
#define __FONTE_WIDGET__

#include <QWidget>
#include <vector>


//PDF
#include <kernel/pdfedit-core-dev.h>
#include <kernel/factories.h>
#include <kernel/pdfoperators.h>
#include "typedefs.h"
//misc
//kvoli xpdf, kde je to definovane...pch!:)
#undef fontItalic 
#include "ui_properties.h"

class TextFont
{
	std::string fontId;
public:
	TextFont(std::string id) : fontId(id) {}
	//vrati pdfoperator TF, s nastavenim fontu a velkosti
	boost::shared_ptr<pdfobjects::PdfOperator> getFontOper(int size)
	{	
		pdfobjects::PdfOperator::Operands fontOperands;//TODO check poradie
		fontOperands.push_back(boost::shared_ptr<pdfobjects::IProperty>(new pdfobjects::CName (fontId)) );
		fontOperands.push_back(boost::shared_ptr<pdfobjects::IProperty>(pdfobjects::CRealFactory::getInstance(size)));//velkost pismeno
		return createOperator("Tf", fontOperands);
	}
};

class FontWidget : public QWidget
{
	Q_OBJECT

	Ui::Properties ui;
	std::vector<TextFont> _fonts;
public:
	FontWidget(QWidget * parent);
	FontWidget(const FontWidget & font);
	~FontWidget();
	// set
	void addFont(std::string name, std::string value);
	//gets
	int getRotation();
	int getScale();
	QColor getColor();
	int getX();
	int getY();
signals:
	void text(PdfOp op);
public slots:
	void apply(); //on clicked
};
#endif