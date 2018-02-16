#ifndef Q_MATH_UTIL_H
#define Q_MATH_UTIL_H


#include <QtMath>
#include <QPointF>
#include <QLineF>
#include <QPolygonF>


class QMathUtil
{
public:
	QMathUtil() = delete;
	~QMathUtil() = delete;
	QMathUtil(QMathUtil const & m) = delete;
	QMathUtil& operator=(QMathUtil const & m) = delete;
	QMathUtil(QMathUtil const && m) = delete;
	QMathUtil& operator=(QMathUtil const && m) = delete;

	static qreal norm2(QPointF const & p) { return p.x() * p.x() + p.y() * p.y(); }
	static qreal norm(QPointF const & p) { return qSqrt(norm2(p)); }
	static qreal dist2(QPointF const & p1, QPointF const & p2) { return norm2(p2 - p1); }
	static qreal dist(QPointF const & p1, QPointF const & p2) { return norm(p2 - p1); }

	static qreal angleFromVector(QPointF const & v) { return qAtan2(v.y(), v.x()); }
	static QPointF vectorFromAngle(qreal angle) { return QPointF(qCos(angle), qSin(angle)); }

	static QPointF rotate(QPointF const & p, qreal angle) {
		qreal c{ qCos(angle) };
		qreal s{ qSin(angle) };
		return QPointF(c * p.x() - s * p.y(), s * p.x() + c * p.y());
	}

	static bool isSelfIntersecting(QPolygonF const & polygon) {
		int iLast{ polygon.size() - 2 };
		int jLast{ polygon.size() - 1 };
		for (int i{ 0 }; i < iLast; ++i) {
			for (int j{ i + 2 }; j < jLast; ++j) {
				if (i == 0 && j == jLast - 1 && polygon.isClosed()) {
					continue;
				}
				if (QLineF(polygon[i], polygon[i + 1]).intersect(QLineF(polygon[j], polygon[j + 1]), nullptr) == QLineF::BoundedIntersection) {
					return true;
				}
			}
		}
		return false;
	}

	static bool isDefinedClockwise(QPolygonF const & polygon) {
		int iLast{ polygon.size() - (polygon.isClosed() ? 1 : 0) };
		QPointF p1{ polygon[0] };
		qreal sum{ 0.0 };
		for (int i{ 0 }; i <= iLast; ++i) {
			QPointF p2{ polygon[(i + 1) % polygon.size()] };
			sum += (p2.x() - p1.x()) * (p2.y() + p1.y());
			p1 = p2;
		}
		return sum >= 0.0;
	}


};


#endif // Q_MATH_UTIL_H
