#ifndef Q_CIRCULAR_BODY_H
#define Q_CIRCULAR_BODY_H


#include "QShapeBody.h"


//!	\brief QCircularBody est une classe de forme circulaire utilisable par QPhysicalBody.
//!	
//!	
class QCircularBody : public QShapeBody
{
public:
	QCircularBody() = default;
	QCircularBody(qreal radius);

	virtual QShapeBody * clone() const override;

	qreal radius() const { return mRadius; }
	void setRadius(qreal radius);

	virtual void draw(QPainter & painter) const override;

protected:
	qreal mRadius{ 0.0 };

	virtual void processInertia() override;
};


#endif // Q_CIRCULAR_BODY_H
