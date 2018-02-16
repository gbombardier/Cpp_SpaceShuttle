#ifndef Q_THRUSTER_KEYBOARD_CONTROLLER_H
#define Q_THRUSTER_KEYBOARD_CONTROLLER_H


#include "QThrusterController.h"
#include <QKeySequence>
class QKeyEvent;


class QThrusterKeyboardController : public QThrusterController
{
public:
	QThrusterKeyboardController(QShuttleThruster * thruster = nullptr);
	QThrusterKeyboardController(QKeySequence const & keySequence, QShuttleThruster * thruster = nullptr);

	QKeySequence linkedKey() const;
	void linkKey(QKeySequence const & keySequence);

	virtual void stepSimulation(double deltaT) override;
	virtual void reset() override;

	virtual void keyPressed(QKeyEvent * event);
	virtual void keyReleased(QKeyEvent * event);

protected:
	QKeySequence mKeySequence;
};


#endif // Q_THRUSTER_KEYBOARD_CONTROLLER_H
