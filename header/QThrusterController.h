#ifndef Q_THRUSTER_CONTROLLER_H
#define Q_THRUSTER_CONTROLLER_H


class QShuttleThruster;


class QThrusterController
{
public:
	QThrusterController(QShuttleThruster * thruster = nullptr);

	void linkThruster(QShuttleThruster * thruster);

	virtual void stepSimulation(double deltaT) = 0;
	virtual void reset() = 0;

protected:
	QShuttleThruster * mLinkedThruster;
};


#endif // Q_THRUSTER_CONTROLLER_H
