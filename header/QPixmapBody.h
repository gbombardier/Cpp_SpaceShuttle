#ifndef Q_PIXMAP_BODY_H
#define Q_PIXMAP_BODY_H


#include "QRectangularBody.h"
#include <QPixmap>
#include <QImage>

//! \brief QPixmapBody est une classe de forme rectangulaire affichant une image utilisable par QPhysicalBody.
//!
//! IL N'Y A EU VRAIMENT AUCUN TEST
class QPixmapBody : public QRectangularBody
{
public:
	QPixmapBody();
	QPixmapBody(QPixmap const & pixmap, QPointF const & hotPoint = QPointF(0.0, 0.0));
	QPixmapBody(QImage const & image, QPointF const & hotPoint = QPointF(0.0, 0.0));
	~QPixmapBody();

	QPixmap pixmap() const;
	void setPixmap(QPixmap const & pixmap, QPointF const & hotPoint = QPointF(0.0, 0.0));
	void setPixmap(QImage const & image, QPointF const & hotPoint = QPointF(0.0, 0.0));

	QPointF hotPoint() const;
	void setHotPoint(QPointF const & hotPoint);

	virtual QShapeBody * clone() const override;

	virtual void draw(QPainter & painter) const override;

protected:
	QPixmap mPixmap;
	QPointF mHotPoint;

private:
	using QRectangularBody::width;
	using QRectangularBody::height;
	using QRectangularBody::size;
	using QRectangularBody::setWidth;
	using QRectangularBody::setHeight;
	using QRectangularBody::setSize;
	using QRectangularBody::mWidth;
	using QRectangularBody::mHeight;
};


#endif // Q_PIXMAP_BODY_H
