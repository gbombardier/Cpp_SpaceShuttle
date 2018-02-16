#ifndef Q_POLYGON_FACTORY_H
#define Q_POLYGON_FACTORY_H


#include <QPolygonF>


class QPolygonFactory
{
public:
	QPolygonFactory() = delete;
	~QPolygonFactory() = delete;
	QPolygonFactory(QPolygonFactory const & f) = delete;
	QPolygonFactory& operator=(QPolygonFactory const & f) = delete;
	QPolygonFactory(QPolygonFactory const && f) = delete;
	QPolygonFactory& operator=(QPolygonFactory const && f) = delete;

	static QPolygonF isoscelesTriangle(qreal base, qreal height, bool tipInsteadOfBase = true);
	static QPolygonF equilateralPolygon(size_t n, qreal sideLength, qreal thetaDegrees);
	static QPolygonF arrow4Points(qreal length = 1.0, qreal relativeWidth = 1.0, qreal relativeRecess = 0.15, bool tipInsteadOfBase = true);

	//void transform(QPolygonF & polygon, QPointF const & translation, qreal rotation = 1.0, qreal scaling = 1.0);

};


#endif // Q_POLYGON_FACTORY_H
