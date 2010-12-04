#include "page.h"
#include "globalfunctions.h"
#include "debug.h"
#include <QPainter>

DisplayPage::DisplayPage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

DisplayPage::~DisplayPage()
{

}
//slot
void DisplayPage::zoom(int zoomscale)//later with how much pages, if all or not
{
	this->ui.label->resize(zoomscale * _size);
	//TODO adjust scrollbar?
}

void DisplayPage::setImage( const QImage & image )
{
	_image = image.copy();
	unsetImg();
}
void DisplayPage::fillRect( QRegion region, const QColor color)
{
	QVector<QRect> r = region.rects();
	for ( int i= 0; i < r.size(); i++)
	{
		fillRect(r[i].x(), r[i].y(), r[i].width(), r[i].height(),color);
	}
}
	
void DisplayPage::fillRect( int x1, int y1, int x2, int y2, const QColor color)
{
	QPainter painter(&_copyImg);
	y1 = ui.label->size().height() - y1; //TODO pixmap size! for displaying
	y2 = ui.label->size().height() - y2;
	int sx = (x1 > x2) ? x1 - x2:x2 - x1;
	int sy = (y1 > y2) ? y1 - y2:y2 - y1;
	QRect r(min(x1,x2),min(y1,y2),sx,sy);
//	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(r, color);
	setImg();

	//needrepaint?
}
void DisplayPage::setImg() //again st from image, for removing highligh and so
{
	this->ui.label->setPixmap(QPixmap::fromImage(_copyImg));
	this->ui.label->adjustSize();
	_size = this->ui.label->pixmap()->size();
}
void DisplayPage::unsetImg() //against from image, for removing highligh and so
{
	_copyImg = _image.copy();
	setImg();
}
void DisplayPage::mousePressEvent(QMouseEvent * event)
{
	//pass parent the coordinates
	DEBUGLINE("clicked on" <<event->x() << " " <<event->y()); //TODO konvertovat na image, nie label
	//chceme suradnice vzhladom na label //TODO co ak budeme v continuous mode?
	
	QPoint point = this->ui.label->mapFromParent( event->pos());
	std::cout << point.x() << " "<< point.y() << std::endl;
	emit MouseClicked(point.x(), this->ui.label->size().height() - point.y()); //opacne kvoli pdfku
}
void DisplayPage::mouseMoveEvent(QMouseEvent * event)
{
	emit highlightText(event->x(), this->ui.label->size().height() - event->y());
}


void DisplayPage::mouseReleased(QMouseEvent * event)
{
	event->ignore(); //posun to parentovi
}
