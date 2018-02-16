#ifndef Q_SHUTTLE_FUEL_TANK_SHAPE_H
#define Q_SHUTTLE_FUEL_TANK_SHAPE_H


#include "QRectangularBody.h"
#include "QShuttleFuelTank.h"


class QShuttleFuelTankShape : public QRectangularBody
{
public:
	QShuttleFuelTankShape(QShuttleFuelTank const & fuelTank);
	QShuttleFuelTankShape(QShuttleFuelTank const & fuelTank, qreal width, qreal height, QColor const & fuelColor = QColor(196, 255, 18));

	QColor fuelColor() const;
	bool tankOpaque() const;
	void setFuelColor(QColor const & fuelColor);
	void setTankOpaque(bool tankOpaque);

	virtual void draw(QPainter & painter) const override;

private:
	QShuttleFuelTank const & mFuelTank;
	QColor mFuelColor{ QColor(196, 255, 18) };
	bool mTankOpaque{ false };
};


#endif // Q_SHUTTLE_FUEL_TANK_SHAPE_H
