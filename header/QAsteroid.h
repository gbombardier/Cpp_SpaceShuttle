#ifndef Q_ASTEROID_H
#define Q_ASTEROID_H


#include "QPhysicalBody.h"
class QAsteroidShape;


class QAsteroid : public QPhysicalBody
{
public:
	QAsteroid();
	QAsteroid(size_t n, qreal radius, qreal deltaRadius);
	~QAsteroid();

	void generate(size_t n, qreal radius, qreal deltaRadius);
};


#endif // Q_ASTEROID_H
