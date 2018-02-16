#ifndef Q_NO_SHAPE_H
#define Q_NO_SHAPE_H


#include "QShapeBody.h"


//!	\brief QNoShape est une classe de forme sans contenu utilisable par QPhysicalBody.
//!	
//!	Cette forme est sans contenu visuel ni d'inertie. 
class QNoShape : public QShapeBody
{
public:
	QNoShape();

	virtual QShapeBody * clone() const override;
	virtual void draw(QPainter & painter) const override;

protected:
	virtual void processInertia() override;
};


#endif // Q_NO_SHAPE_H
