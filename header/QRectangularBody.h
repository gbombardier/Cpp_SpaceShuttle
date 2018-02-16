#ifndef Q_RECTANGULAR_BODY_H
#define Q_RECTANGULAR_BODY_H


#include "QShapeBody.h"


//!	\brief QRectangularBody est une classe de forme rectangulaire utilisable par QPhysicalBody.
//!	
//!	
class QRectangularBody : public QShapeBody
{
public:
	QRectangularBody() = default;
	QRectangularBody(qreal width, qreal height);

	virtual QShapeBody * clone() const override;

	qreal width() const;
	qreal height() const;
	QSizeF size() const;
	void setWidth(qreal width);
	void setHeight(qreal height);
	void setSize(qreal width, qreal height);

	virtual void draw(QPainter & painter) const override;

protected:
	qreal mWidth{ 0.0 };
	qreal mHeight{ 0.0 };

	virtual void processInertia() override;
};


#endif // Q_RECTANGULAR_BODY_H
