#ifndef Q_ASTEROID_SHAPE_H
#define Q_ASTEROID_SHAPE_H


#include "QPolygonalBody.h"


class QAsteroidShape : public QPolygonalBody
{
public:
	QAsteroidShape();
	QAsteroidShape(size_t n, qreal radius, qreal deltaRadius);

	void generate(size_t n, qreal radius, qreal deltaRadius);
};

#endif // Q_ASTEROID_SHAPE_H
