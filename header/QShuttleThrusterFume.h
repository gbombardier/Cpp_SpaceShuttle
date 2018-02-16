#ifndef Q_SHUTTLE_THRUSTER_FUME_H
#define Q_SHUTTLE_THRUSTER_FUME_H


#include "QPhysicalBody.h"
#include <random>


class QShuttleThrusterFume : public QPhysicalBody
{
public:
	QShuttleThrusterFume(qreal radius = 5.0);

	virtual void stepSimulation(double deltaT) override;
	virtual bool toBeRemoved() override;

	qreal alphaDecreaseRate() const { return mAlphaDecreaseRate; }
	qreal radiusIncreaseRate() const { return mRadiusIncreaseRate; }
	void setAlphaDecreaseRate(qreal alphaDecreaseRate) { mAlphaDecreaseRate = alphaDecreaseRate; }
	void setRadiusIncreaseRate(qreal radiusIncreaseRate) { mRadiusIncreaseRate = radiusIncreaseRate; }

protected:
	qreal mAlphaDecreaseRate;
	qreal mRadiusIncreaseRate;
};



#endif // Q_SHUTTLE_THRUSTER_FUME_H
