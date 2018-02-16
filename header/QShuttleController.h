#ifndef Q_SHUTTLE_CONTROLLER_H
#define Q_SHUTTLE_CONTROLLER_H


class QShuttle;


class QShuttleController
{
public:
	QShuttleController(QShuttle * shuttle = nullptr);
	~QShuttleController();

	QShuttle const * shuttle() const;
	void setShuttle(QShuttle * shuttle);

	virtual void stepSimulation(double deltaT) = 0;
	virtual void reset() = 0;

protected:
	QShuttle * mShuttle{ nullptr };
};



#endif // Q_SHUTTLE_CONTROLLER_H
