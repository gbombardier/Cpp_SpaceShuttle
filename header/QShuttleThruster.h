#ifndef Q_SHUTTLE_THRUSTER_H
#define Q_SHUTTLE_THRUSTER_H


#include "QPhysicalBody.h"
#include <QColor>

class QSceneModel;
class QShuttleFuelTank;
class QThrusterController;

//!	
//!	\brief QShuttleThruster est une classe représentant le propulseur d'une navette.
//!	
//!	Cette classe est un corps physique destiné à être assigné à une navette.
//!	
//!	Un propulseur possède plusieurs caractéristiques :
//!	 - le modèle auquel il est assigné (nécessaire pour l'ajout de fumée)
//!	 - un contrôleur
//!	 - un lien vers le réservoir où se trouve son carburant
//!	 - le débit massique de matière projetée lors de sa pleine poussée
//!	 - la vitesse d'éjection de la matière lors de sa pleine poussée
//!	 - le niveau de poussé actuel
//!	
//!	La classe QShuttleThruster possède les responsabilités suivantes :
//!	 - elle n'est pas responsable du modèle lié QSceneModel - néanmoins, le modèle doit rester valide tout au long de l'utilisation du propulseur
//!	 - elle n'est pas responsable du réservoir lié QShuttleFuelTank - néanmoins, le réservoir doit rester valide tout au long de l'utilisation du propulseur
//!	 - elle est responsable du contrôleur assigné QThrusterController
//!	
//! Il existe deux types de contrôleur, un pour les navettes et un pour les propulseurs. Selon le type de contrôleur, c'est le contrôleur de la navette qui gère les contrôleurs de propulseur. Il ne suffit qu'à assigner les types de contrôleurs correspondant.
//!	
class QShuttleThruster : public QPhysicalBody
{
public:
	QShuttleThruster(QShapeBody * shape);
	~QShuttleThruster();

	QSceneModel* sceneModel();
	QThrusterController* controller();
	QShuttleFuelTank * fuelTank();
	void setSceneModel(QSceneModel * model);
	void setController(QThrusterController * controller);
	void linkToFuelTank(QShuttleFuelTank * fuelTank);
	
	qreal massFlowRate() const { return mMassFlowRate; }
	qreal massEjectedSpeed() const { return mMassEjectedSpeed; }
	qreal thrustLevel() const { return mThrustLevel; }

	void setThrusterEfficiency(qreal massFlowRate, qreal massEjectedSpeed);
	void setThrustLevel(qreal thrustLevel);

	qreal pushes();	// return resulting force from the thrust level and tank level

	qreal tankCapacity() const;
	qreal fuelLevel() const;
	qreal fuelLevelRatio() const;
	bool isFuelEmpty() const;
	QColor fuelColor() const;

protected:
	QSceneModel * mSceneModel{ nullptr };
	QShuttleFuelTank * mFuelTank{ nullptr };
	QThrusterController * mController{ nullptr };

	qreal mMassFlowRate{ 0.0 };			// kg / s
	qreal mMassEjectedSpeed{ 0.0 };		// m / s
	qreal mThrustLevel{ 0.0 };			// % -> [0.0, 1.0]

private:
	using QPhysicalBody::attachBodyUnder; // cannot attach other bodies
	using QPhysicalBody::attachBodyOver; // cannot attach other bodies
	//void detachBody(QPhysicalBody * body);
};






#endif // Q_SHUTTLE_THRUSTER_H
