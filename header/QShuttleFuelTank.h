#ifndef Q_SHUTTLE_FUEL_TANK_H
#define Q_SHUTTLE_FUEL_TANK_H


#include "QPhysicalBody.h"
#include <QColor>

//!
//!	\brief QShuttleFuelTank est une classe représetant un réservoir d'une navette.
//!	
//!	Ce réservoir possède le carburant nécessaire aux propulseurs qui propulsent la navette. Il est ajouté sur la navette.
//!	
//!	Même si la classe QShuttleFuelTank hérite de la classe QPhysicalBody, il est impossible de déterminer sa forme. 
//! La forme QShuttleFuelTankShape est créée à l'appel du constructeur. Cette forme rectangulaire a la caractéristique d'afficher la quantité de carburant restant. 
//!
//!	Un réservoir possède plusieurs caractéristiques :
//!	 - la capcité de carburant (en kg) - voir les fonctions capacity() et setCapacity()
//!	 - le niveau de carburant (en %) - voir les fonctions fuelLevel(), fuelLevelRatio(), isEmpty(), fill() et takeFuel()
//!	
//! Quelques fonctions utilitaires permettent d'interagire directement avec la forme : 
//!	 - fuelColor() et setFuelColor()
//!	 - setTankSize()
//!	
class QShuttleFuelTank : public QPhysicalBody
{
public:
	QShuttleFuelTank();
	QShuttleFuelTank(qreal tankWidth, qreal tankHeight, QColor const & fuelColor = QColor(196, 255, 18));

	qreal capacity() const;
	qreal fuelLevel() const;
	qreal fuelLevelRatio() const;
	bool isEmpty() const;

	void setCapacity(qreal capacity);

	void fill(qreal level = 1.0);			// to do -> must update the mass
	qreal takeFuel(qreal fuelAmount);		// to do -> must update the mass

	QSizeF tankSize() const;
	QColor fuelColor() const;
	void setTankSize(qreal width, qreal height);
	void setFuelColor(QColor const & color);

protected:
	qreal mCapacity{ 1.0 };					// kg
	qreal mFuelLevel{ 0.0 };				// kg  -  mFuelLevel in [0.0, mCapacity]

private:
	using QPhysicalBody::setShape;
	using QPhysicalBody::attachBodyUnder;		// cannot attach other bodies
	using QPhysicalBody::attachBodyOver;		// cannot attach other bodies
	//void detachBody(QPhysicalBody * body);
};



#endif // Q_SHUTTLE_FUEL_TANK_H
