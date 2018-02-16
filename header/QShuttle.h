#ifndef Q_SHUTTLE_H
#define Q_SHUTTLE_H


#include "QPhysicalBody.h"
class QShuttleFuelTank;
class QShuttleThruster;
class QShuttleController;


//!
//!	\brief QShuttle est une classe décrivant une navette naviguant dans l'espace 2d.
//!	
//!	Cette classe est un corps physique interagissant avec tous les constitants de la simulation. Chaque navette est paramétrable pour plusieurs caractéristiques :
//!	 - sa forme via la classe QShapeBody
//!	 - une liste de réservoir de carburant - voir fuelTanks()
//!	 - une liste de propulseur - voir thrusters()
//!	 - un contrôleur - voir controller()
//!	
//! La classe QShuttle est responsable de toutes les assignations :
//!	 - forme (QShapeBody)
//!	 - réservoirs (QShuttleFuelTank)
//!	 - propulseurs (QShuttleThruster)
//!	 - contrôleur (QShuttleController)
//!	 
//!	En fait, pour que la navette soit complète, il est essentiel qu'elle possède au moins un élément de chaque item énuméré plus haut. 
//!	 - les propulseurs sevent à propulser et diriger la navette (selon leur nombre et disposition);
//!	 - les propulseurs doivent être reliés à un réservoir qui contient et distribue le carburant;
//!	 - finalement, un contrôleur est essentiel afin de pouvoir interagir avec la navette.
//!	
//!	La disposition des propulseurs sur la navette permettra de la diriger avec ou sans rotation.
//!
//! Les propulseurs sont ajoutés sous la navette alors que les réservoirs sont ajoutés sur la navette.
//!
//! Il existe deux types de contrôleur, un pour les navettes et un pour les propulseurs. Selon le type de contrôleur, c'est le contrôleur de la navette qui gère les contrôleurs de propulseur. Il ne suffit qu'à assigner les types de contrôleurs correspondant.
//!
class QShuttle : public QPhysicalBody
{
public:
	QShuttle();
	QShuttle(QShapeBody * shape);
	virtual ~QShuttle();

	//! \brief Retourne la liste de réservoirs.
	QList<QShuttleFuelTank*> const & fuelTanks() const { return mFuelTanks; }
	//! \brief Ajoute un nouveau réservoir.
	//!
	//!	Le réservoir ajouté devient un enfant de la navette. Il est automatiquement géré par celle-ci.
	void addFuelTank(QShuttleFuelTank * fuelTank, QPointF const & position, qreal orientation, bool addOver = true);
	//! \brief Détruit le réservoir _fuelTank_.
	//!
	//!	Si _fuelTank_ est valide, le réservoir est retiré de la liste et détruit. La fonction retourne _true_ si _fuelTank_ existe sinon _false_. Le pointeur n'est plus valide si la fonction est un succès.
	bool removeFuelTank(QShuttleFuelTank * fuelTank);

	//! \brief Retourne la liste de propulseurs.
	QList<QShuttleThruster*> const & thrusters() const;
	//! \brief Ajoute un nouveau propulseur.
	//!
	//!	Le propulseur ajouté devient un enfant de la navette. Il est automatiquement géré par celle-ci. 
	void addThruster(QShuttleThruster * thruster, QPointF const & position, qreal orientation, bool addUnder = true);
	//! \brief Détruit le propulseur _thruster_.
	//!
	//!	Si _thruster_ est valide, le propulseur est retiré de la liste et détruit. La fonction retourne _true_ si _thruster_ existe sinon _false_. Le pointeur n'est plus valide si la fonction est un succès.
	bool removeThruster(QShuttleThruster * thruster);
	//! \brief Retire et détruit tous les le propulseurs_.
	void removeThrusters();
	////! \brief Ajoute un nouveau propulseur.
	////!
	////!	Le propulseur ajouté devient un enfant de la navette. Il est automatiquement géré par celle-ci. 
	//void setThrusterQuantity(size_t quantity);

	//! \brief Retourne le contrôleur.
	QShuttleController * controller();
	//! \brief Déternime le contrôleur.
	//!
	//!	Si aucun contrôleur n'est assigné, _controller_ est assigné. Si _controller_ est celui déjà assigné, rien ne se passe. Si _controller_ est égal à _nullptr_, alors le contrôleur existant est détruit et aucun contrôleur n'est assigné à la navette. S'il existe un contrôleur, alors ce dernier est détruit et substitué par _controller_.
	//! Si l'assignation réussi, la navette est automatiquement assignée au contrôleur.
	void setController(QShuttleController * controller);
	//! \brief Détruit le contrôleur existant.
	void removeController();
	
	//!	\brief Effectue un pas de simulation.
	virtual void stepSimulation(double deltaT) override;

protected:
	QList<QShuttleFuelTank*> mFuelTanks;
	QList<QShuttleThruster*> mThrusters;
	// QList<QPhysicalBody*> mPayLoad; // to do
	QShuttleController * mController;

private:
	using QPhysicalBody::attachBodyUnder; // cannot attach other bodies
	using QPhysicalBody::attachBodyOver; // cannot attach other bodies
	//void detachBody(QPhysicalBody * body);
};

#endif // Q_SHUTTLE_H
