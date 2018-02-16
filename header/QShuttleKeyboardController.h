#ifndef Q_SHUTTLE_KEYBOARD_CONTROLLER_H
#define Q_SHUTTLE_KEYBOARD_CONTROLLER_H


#include "QShuttleController.h"
class QKeyEvent;


class QShuttleKeyboardController : public QShuttleController
{
public:
public:
	QShuttleKeyboardController(QShuttle * shuttle = nullptr);
	~QShuttleKeyboardController();

	virtual void stepSimulation(double deltaT) override;
	virtual void reset() override;

	virtual void keyPressed(QKeyEvent * event);
	virtual void keyReleased(QKeyEvent * event);
};


#endif // Q_SHUTTLE_KEYBOARD_CONTROLLER_H
