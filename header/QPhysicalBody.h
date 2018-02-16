#ifndef Q_PHYSICAL_BODY_H
#define Q_PHYSICAL_BODY_H


#include <QRectF>
#include <QList>
#include "MathUtil.h"
class QPainter;
class QShapeBody;


//!	
//!	\brief QPhysicalBody est une classe représentant un corps physique et ses interactions dans un contexte de simulation 2d.
//!	
//!	\section qpb_physics Physique de la simulation
//!	
//!	QPhysicalBody est la classe fondamentale de tous les corps de la simulation. Ses interactions sont principalement liées aux déplacements du corps dans un espace **2d**.
//!	On y retrouve les informations de translation et de rotation dans l'espace avec :
//!	 - les positions linéaire et angulaire
//!		- linearPosition() et setLinearPosition()
//!		- angularPosition() et setAngularPosition()
//!	 - les vitesses linéaire et angulaire
//!		- linearSpeed() et setLinearSpeed()
//!		- angularSpeed() et setAngularSpeed()
//!	 - les accélérations linéaire et angulaire
//!		- accelerationSpeed() et setLinearAcceleration()
//!		- accelerationSpeed() et setAngularAcceleration()
//!	
//!	La collision entre les corps n'est pas considérée par la simulation.
//!
//! La simulation de physique se résume à résoudre les équations de mouvement déterminées par Newton en considérant :
//!  - la préservation du moment cinétique linéaire et angulaire
//!  - l'inertie linéaire et angulaire
//!	 - l'application de forces (ou d'accélérations)
//!	 - les forces peuvent être appliquées selon le référentiel local ou le référentiel monde
//!
//!	Pendant un pas de simulation, il est possible d'appliquer une série de forces ou d'accélérations qui seront utilisées pour résoudre les vitesses et positions linéaires et angulaires. À la fin du pas de simulation, les forces et accélérations sont remises à 0.0. Les fonctions suivantes permenttent d'appliquer les forces et accélérations sont :
//!  - addExtLinearAcceleration(), addIntLinearAcceleration() et addAngularAcceleration()
//!	 - addExtForce(), addIntForce() et addTorque()
//!	
//!	\section qpb_shape Représentation physique et visuelle
//!	
//!	Tout corps **doit** posséder un objet de la classe QShapeBody afin d'en déterminer plusieurs éléments. Par cette approche flexible, le corps peut avoir n'importe quelle forme et même changer de forme pendant la simulation.
//!	Cet objet rend possible : 
//!	 - la définition géométrique de la forme
//!	 - la définition cosmétique (couleur et autre)
//!	 - l'affichage de la forme
//!	 - les calculs :
//!		- d'inertie :
//!			- linéaire (masse)
//!			- angulaire (moment d'inertie)
//!		- de centroïde
//!	
//!	Attention, le corps doit posséder une forme en tout temps. Aucune validation n'est faite pendant la simulation. Une forme invalide entraîne un comportement indéterminé. QPhysicalBody est responsable de la forme associée et s'occupe de la gestion de la mémoire pour sa désallocation. Par conséquent, lorsqu'une forme est assigné à un corps, il est impossible de la récupérer.
//!	
//!	La gestion de la forme s'effectue par ces fonctions :
//!	 - QPhysicalBody()
//!	 - QPhysicalBody(QShapeBody * shape)
//!	 - ~QPhysicalBody()
//!	 - setShape(QShapeBody * shape)
//!	 - shape()
//!	
//!	\section qpb_referential Parent, enfants et référentiel
//!	
//!	Chaque corps peut être défini selon le référentiel monde ou selon le référentiel d'un corps parent. Par conséquent :
//!	 - chaque corps peut être relié à [0, 1] corps parent
//!	 - chaque corps peut posséder de [0, n] corps enfants
//!	
//!	Un corps parent est toujours responsable de libérer la mémoire de ses enfants. Un corps sans parent doit êter assigné à la classe QSceneModel, c'est cette dernière qui est responsable de sa gestion de la mémoire. Si la classe n'est ni assigné à un parent ou à QSceneModel, alors c'est l'usager qui doit gérer la mémoire. 
//!	
//!	Il existe deux listes d'enfants possibles, les listes _under_ et _over_. Ces listes permettent d'afficher les corps enfants au-dessous du corps parent ou au-dessus du corps parent respectivement.
//!	
//!	Les fonctions suivantes permettent la gestion de ces conditions :
//!	 - parent()
//!	 - attachBodyUnder() et attachBodyOver()
//!	 - detachBodyUnder(), detachBodyOver() et detachBody()
//!	 - eraseBodyUnder(), eraseBodyOver() et eraseBody()
//!	 - attachedBodiesUnder() et attachedBodiesOver()
//!	 - mapFromScene() et mapToScene()
//! 
//!	Considérant les paramètres définis, il est possible d'appliquer des forces (linéaires et angulaires) selon les coordonnées 
//!	On y retrouve les caractéristiques communes :
//!	 - une forme quelconque représentée par la classe QShapeBody
//!	 - optionnellement, un parent duquel les coordonnées de l'objet courant sont relatives et deux fonctions permettant de passer des coordonnées mondes aux coordonnées corps (mapToScene() et mapFromScene())
//!	 - optionnellemnet, une liste d'enfants dont leurs coordonnées sont relatives à l'objet courant
//!	 - les variables nécessaires à une représentation **linéaire** du corps dans l'espace 2d :
//!		- position linéaire
//!		- vitesse linéaire
//!		- accélération linéaire
//!	 - les variables nécessaires à une représentation **angulaire** du corps dans l'espace 2d :
//!		- position angulaire (orientation)
//!		- vitesse angulaire
//!		- accélération angulaire
//!	 - un ensemble de fonction permettant d'appliquer une force ou une accélération (centrées au centroïde ou non) selon :
//!		- forces et accélérations internes (référentiel local)
//!		- forces et accélérations externes (référentiel monde)
//!	
//!	 ... 
//!	
class QPhysicalBody
{
public:
	//! \brief Constructeur par défaut. 
	//!
	//! Assigne automatiquement une forme ciculaire de rayon 1.0. 
	QPhysicalBody(QPhysicalBody * parent = nullptr);
	//! \brief Constructeur incluant une forme. 
	//!
	//! Assigne la forme _shape_. 
	QPhysicalBody(QShapeBody * shape, QPhysicalBody * parent = nullptr);
	//! \brief Destructeur. 
	//!
	//! Détruit la forme assignée. 
	virtual ~QPhysicalBody();

	//! \brief Copie constructeur retiré. [to do]
	QPhysicalBody(QPhysicalBody const & body) = delete;
	//! \brief Opérateur d'assignation retiré. [to do]
	QPhysicalBody& operator=(QPhysicalBody const & body) = delete;
	//! \brief Copie constructeur de déplacement retiré. [to do]
	QPhysicalBody(QPhysicalBody const && body) = delete;
	//! \brief Opérateur d'assignation de déplacement retiré. [to do]
	QPhysicalBody& operator=(QPhysicalBody const && body) = delete;

	//! \brief Assigne une forme au corps courant. 
	//!
	//! La forme assignée possède toutes les informations cosmétiques et de répartition de la masse. 
	void setShape(QShapeBody * shape);
	//! \brief Forme associée au corps.
	QShapeBody* shape() const;

	//! \brief Parent du corps courant.
	QPhysicalBody * parent();
	//! \brief Ajoute un corps enfant.
	//! 
	//! Le corps courant est automatiquement assigné en tant que parent à body. Lors de l'affichage, le corps ajouté est considéré en dessous du corps courant.
	void attachBodyOver(QPhysicalBody * body);
	//! \brief Ajoute un corps enfant.
	//! 
	//! Le corps courant est automatiquement assigné en tant que parent à body. Lors de l'affichage, le corps ajouté est considéré au dessus du corps courant.
	void attachBodyUnder(QPhysicalBody * body);
	//! \brief Détache un corps enfant.
	//! 
	//! Le corps spécifié est retiré en parcourant les deux listes _under_ et _over_. Retourne vrai si un tel objet existe sinon retourne faux. La mémoire associée n'est pas libérée. Celui qui appel cette fonction est responsable du corps détaché.
	bool detachBody(QPhysicalBody * body);
	//! \brief Détache un corps enfant de la liste _under_.
	//! 
	//! Le corps spécifié est retiré de la liste _under_. Retourne vrai si un tel objet existe sinon retourne faux. La mémoire associée n'est pas libérée. Celui qui appel cette fonction est responsable du corps détaché.
	bool detachBodyUnder(QPhysicalBody * body);
	//! \brief Détache un corps enfant de la liste _over_.
	//! 
	//! Le corps spécifié est retiré de la liste _over_. Retourne vrai si un tel objet existe sinon retourne faux. La mémoire associée n'est pas libérée. Celui qui appel cette fonction est responsable du corps détaché.
	bool detachBodyOver(QPhysicalBody * body);

	//! \brief Détache un corps enfant et détruit _body_.
	//! 
	//! Le corps spécifié est retiré en parcourant les deux listes _under_ et _over_. Retourne vrai si un tel objet existe sinon retourne faux. L'objet est détruit et la mémoire associée est libérée.
	bool eraseBody(QPhysicalBody * body);
	//! \brief Détache un corps enfant de la liste _under_ et détruit _body_.
	//! 
	//! Le corps spécifié est retiré de la liste _under_. Retourne vrai si un tel objet existe sinon retourne faux. L'objet est détruit et la mémoire associée est libérée.
	bool eraseBodyUnder(QPhysicalBody * body);
	//! \brief Détache un corps enfant de la liste _over_ et détruit _body_.
	//! 
	//! Le corps spécifié est retiré de la liste _over_. Retourne vrai si un tel objet existe sinon retourne faux. L'objet est détruit et la mémoire associée est libérée.
	bool eraseBodyOver(QPhysicalBody * body);

	//! \brief Détache tous les corps enfants et les détruit.
	void eraseBodies();

	//! \brief Détache tous les enfants devant être retirés.
	//!
	//! Cette fonction détache tous les enfants qui doivent être retirée (_under_ et _over_). Tous les corps détachés sont retournés dans la liste et deviennent la responsabilité de la fonction appelante.
	QList<QPhysicalBody*> detachChildToBeRemoved();
	//! \brief Détache et détruit tous les enfants devant être retirés.
	//!
	//! Cette fonction détache et détruit tous les enfants qui doivent être retirée (_under_ et _over_).
	void eraseChildToBeRemoved();

	//! \brief Retourne la liste de tous les corps attachés (au dessous).
	QList<QPhysicalBody*> const & attachedBodiesUnder() const;
	//! \brief Retourne la liste de tous les corps attachés (au dessus).
	QList<QPhysicalBody*> const & attachedBodiesOver() const;

	//! \brief Ajoute une accélération linéaire appliquée au centroïde selon le référentiel monde.
	void addExtLinearAcceleration(QPointF const & linAcceleration);								// Acceleration applied uniformly on the body	=> using center of mass
	//! \brief Ajoute une accélération linéaire appliquée au centroïde selon le référentiel local.
	void addIntLinearAcceleration(QPointF const & linAcceleration);								// Acceleration applied uniformly on the body	=> using center of mass
	//! \brief Ajoute une accélération angulaire appliquée au centroïde.
	void addAngularAcceleration(qreal angAcceleration);											// Acceleration applied uniformly on the body	=> using center of mass
	//! \brief Ajoute une force (force linéaire) appliquée au centroïde selon le référentiel monde.
	void addExtForce(QPointF const & force);													// Force applied uniformly on the body			=> using center of mass
	//! \brief Ajoute une force (force linéaire) appliquée au centroïde selon le référentiel local.
	void addIntForce(QPointF const & force);													// Force applied uniformly on the body			=> using center of mass
	//! \brief Ajoute un couple (force angulaire) appliquée au centroïde.
	void addTorque(qreal torque);																// Torque applied uniformly on the body			=> using center of mass
	//! \brief Ajoute une accélération linéaire appliquée au point _from_ selon le référentiel monde.
	void addExtLinearAcceleration(QPointF const & from, QPointF const & linAcceleration);		// Acceleration applied from a specific point	=> linear acceleration + angular acceleration applied at center of mass 
	//! \brief Ajoute une accélération linéaire appliquée au point _from_ selon le référentiel local.
	void addIntLinearAcceleration(QPointF const & from, QPointF const & linAcceleration);		// Acceleration applied from a specific point	=> linear acceleration + angular acceleration applied at center of mass 
	//  not considered yet - to do : void addExtAngularAcceleration(QPointF const & from, qreal angAcceleration);	// currently, rotations are applied from the center of mass
	//! \brief Ajoute une force (force linéaire) appliquée au point _from_ selon le référentiel monde.
	void addExtForce(QPointF const & from, QPointF const & force);								// Force applied from a specific point			=> linear acceleration + angular acceleration applied at center of mass 
	//! \brief Ajoute une force (force linéaire) appliquée au point _from_ selon le référentiel local.
	void addIntForce(QPointF const & from, QPointF const & force);								// Force applied from a specific point			=> linear acceleration + angular acceleration applied at center of mass 
	//  not considered yet - to do : void addTorque(QPointF const & from, qreal torque);		// currently, rotations are applied from the center of mass

	//! \brief Effectue la transformation des coordonnées d'un point.
	//!
	//! La transformation correspond au passage du référentiel monde vers le référentiel local.
	QPointF mapFromScene(QPointF const & point);
	//! \brief Effectue la transformation des coordonnées d'un point.
	//!
	//! La transformation correspond au passage du référentiel local vers le référentiel monde.
	QPointF mapToScene(QPointF const & point);
	//! \brief Effectue la transformation des coordonnées d'un point.
	//!
	//! La transformation correspond au passage du référentiel parent vers le référentiel local. S'il n'y a pas de parent assigné, le référentiel monde est alors considéré comme parent.
	QPointF mapFromParent(QPointF const & point);
	//! \brief Effectue la transformation des coordonnées d'un point.
	//!
	//! La transformation correspond au passage du référentiel local vers le référentiel parent. S'il n'y a pas de parent assigné, le référentiel monde est alors considéré comme parent.
	QPointF mapToParent(QPointF const & point);

	//! \brief Effectue la transformation d'une orientation.
	//!
	//! La transformation correspond au passage du référentiel monde vers le référentiel local.
	qreal mapFromScene(qreal orientation);
	//! \brief Effectue la transformation d'une orientation.
	//!
	//! La transformation correspond au passage du référentiel local vers le référentiel monde.
	qreal mapToScene(qreal orientation);
	//! \brief Effectue la transformation d'une orientation.
	//!
	//! La transformation correspond au passage du référentiel parent vers le référentiel local. S'il n'y a pas de parent assigné, le référentiel monde est alors considéré comme parent.
	qreal mapFromParent(qreal orientation);
	//! \brief Effectue la transformation d'une orientation.
	//!
	//! La transformation correspond au passage du référentiel local vers le référentiel parent. S'il n'y a pas de parent assigné, le référentiel monde est alors considéré comme parent.
	qreal mapToParent(qreal orientation);


	//! \brief Dessine la forme avec _painter_.
	void paint(QPainter & painter) const;

	//! \brief Effectue un pas de simulation.
	//!
	//! Applique les forces ajoutées pendant le pas de simulation et met à jour les positions, vitesses et accélérations.
	virtual void stepSimulation(double deltaT);

	//! \brief Indique si un corps doit être retiré de la simulation. 
	//!
	//! Cette fonction peut être surchargée et, selon certaines conditions, indiquer si l'objet courant peut être retiré de la simulation. Par défaut, cette fonction retourne _false_.
	virtual bool toBeRemoved();

	//! \brief Retourne le temps écoulé depuis l'insertion de l'objet dans la simulation. 
	qreal timeLived() const;

	//! \brief Retourne le temps pendant lequel le corps peut exister dans la simulation. Une valeur négative indique que le corps peut vivre indéfiniment.
	qreal mayLivedUntil() const;

	//! \brief Détermine le temps pendant lequel le corps peut exister dans la simulation. Une valeur négative indique que le corps peut vivre indéfiniment.
	void setMayLivedUntil(qreal mayLivedUntil);

	//! \brief Retourne le nom du corps.
	QString name() const;
	//! \brief Retourne la masse surfacique.
	qreal surfaceMass() const;
	//! \brief Retourne la position linéaire.
	QPointF linearPosition() const;
	//! \brief Retourne la vitesse linéaire.
	QPointF linearSpeed() const;
	//! \brief Retourne l'accélération linéaire.
	QPointF linearAcceleration() const;
	//! \brief Retourne la position angulaire.
	qreal angularPosition() const;
	//! \brief Retourne la vitesse angulaire.
	qreal angularSpeed() const;
	//! \brief Retourne l'accélération angulaire.
	qreal angularAcceleration() const;

	//! \brief Détermine le nom du corps.
	void setName(QString const & name);
	//! \brief Détermine la masse surfacique.
	void setSurfaceMass(qreal surfaceMass);
	//! \brief Détermine la position linéaire.
	void setLinearPosition(QPointF const & linearPosition);
	//! \brief Détermine la vitesse linéaire.
	void setLinearSpeed(QPointF const & linearSpeed);
	//! \brief Détermine l'accélération linéaire.
	void setLinearAcceleration(QPointF const & linearAcceleration);
	//! \brief Détermine tous les paramètres linéaires à la fois.
	void setLinear(QPointF const & linearPosition, QPointF const & linearSpeed = QPointF(0.0, 0.0), QPointF const & linearAcceleration = QPointF(0.0, 0.0));
	//! \brief Détermine la position angulaire.
	void setAngularPosition(qreal angularPosition);
	//! \brief Détermine la vitesse angulaire.
	void setAngularSpeed(qreal angularSpeed);
	//! \brief Détermine l'accélération angulaire.
	void setAngularAcceleration(qreal angularAcceleration);
	//! \brief Détermine tous les paramètres angulaires à la fois.
	void setAngular(qreal angularPosition, qreal angularSpeed = 0.0, qreal angularAcceleration = 0.0);

	// \brief Génère une position linéaire aléatoire bornée par le rect.
	void randomizeLinearPosition(QRectF const & rect);
	// \brief Génère une position angulaire aléatoire bornée par minOrientation et maxOrientation.
	void randomizeAngularPosition(qreal minOrientation = 0.0, qreal maxOrientation = Trigo<>::pi<2>());
	// \brief Génère une vitesse linéaire aléatoire selon une plage de vitesse [minLinearSpeed, maxLinearSpeed] et une plage d'orientation [minOrientation, maxOrientation].
	void randomizeLinearSpeed(qreal minLinearSpeed, qreal maxLinearSpeed, qreal minOrientation = 0.0, qreal maxOrientation = Trigo<>::pi<2>());
	// \brief Génère une vitesse angulaire aléatoire bornée par la plage de vitesse [minSpeed, maxSpeed].
	void randomizeAngularSpeed(qreal minAngularSpeed, qreal maxAngularSpeed);

protected:
	//! \brief Le nom du corps. 
	QString mName;

	//! \brief Lien vers la forme du corps. 
	//! 
	//! L'objet QPhysicalBody est propriétaire de l'objet QShapeBody et, par conséquent, responsable de la gestion dynamique de la mémoire. Il ne faut donc pas libérer la mémoire de la forme.
	//! Cette forme possède toutes les informations nécessaires pour :
	//!	 - la gestion pour les calculs de physique liés à l'inertie de la masse en translation et en rotation
	//!	 - l'apparence visuelle de la forme ainsi que sont affichage
	QShapeBody * mShape;

	//! \brief Masse surfacique. 
	//!
	//! Cet indicateur physique n'existe pas mais correspond à la répartition de masse dans un esapce 2d. 
	//! C'est l'équivalent de la masse volumique dans le monde réel. Selon la forme déterminé par \link #mShape, on considère que la répartition de masse est uniforme sur la surface.
	//!
	//! La masse surfacique est exprimée en kilogramme par mètre carré : \f$\text{kg}\cdot\text{m}^{-2}\f$
	qreal mSurfaceMass{ 1.0 };	// to do... actually unused!

	//! \brief Position linéaire 2d du corps dans l'espace. 
	//!
	//! La position est exprimée en pixel : \f$\text{pix}\f$
	QPointF mLinearPosition;

	//! \brief Vitesse linéaire 2d du corps dans l'espace. 
	//!
	//! La vitesse est exprimée en pixel par seconde : \f$\text{pix}\cdot\text{sec}^{-1}\f$
	QPointF mLinearSpeed;

	//! \brief Accélération linéaire 2d du corps dans l'espace. 
	//!
	//! L'accélération est exprimée en pixel par seconde carrée : \f$\text{pix}\cdot\text{sec}^{-2}\f$
	QPointF mLinearAcceleration;

	//! \brief Position angulaire du corps dans l'espace 2d. 
	//!
	//! La position est exprimée en radians : \f$\text{rad}\f$
	qreal mAngularPosition{ 0.0 };

	//! \brief Vitesse angulaire du corps dans l'espace 2d. 
	//!
	//! La vitesse est exprimée en radians par seconde : \f$\text{rad}\cdot\text{sec}^{-1}\f$
	qreal mAngularSpeed{ 0.0 };

	//! \brief Accélération angulaire du corps dans l'espace 2d. 
	//!
	//! L'accélération est exprimée en radians par seconde carrée : \f$\text{rad}\cdot\text{sec}^{-2}\f$
	qreal mAngularAcceleration{ 0.0 };
	
	//! \brief Le corps parent duquel le corps présent dépend. 
	//!
	//! Le parent a un impact important pour deux aspects. S'il est déterminé :
	//!  - Les coordonnées exprimées par un enfant sont toutes relatives au parent (position et orientation).
	//!  - Le parent est responsable de l'enfant. C'est donc lui qui est responsable de la gestion dynamique de la mémoire et c'est lui qui a la responsabilité de libérer la mémoire du présent corps.
	//! 
	//! Si le parent est nul :
	//!  - Les coordonnées sont relatives au monde.
	QPhysicalBody * mParent;

	//! \brief Liste des enfants qui dépendent et sont relatif au corps courant. 
	//!
	//! Tous ces corps sont dessinés avant le corps courant. Ils sont donc affiché à l'arrière plan selon leur ordre d'insertion dans la liste.
	QList<QPhysicalBody*> mAttachedBodiesUnder;

	//! \brief Liste des enfants qui dépendent et sont relatif au corps courant. 
	//!
	//! Tous ces corps sont dessinés après le corps courant. Ils sont donc affiché à l'avant plan selon leur ordre d'insertion dans la liste.
	QList<QPhysicalBody*> mAttachedBodiesOver;

	//! \brief Le temps écoulé depuis l'insertion de l'objet dans la simulation. 
	//!
	//! Correspond à la somme de tous les _elapsedTime_ accumulé par les appels de _stepSimulation_.
	qreal mTimeLived;

	//! \brief Le temps pendant lequel le corps existe dans la simulation. 
	//!
	//! Une valeur négative indique que le corps peut vivre indéfiniment (ce qui est la valeur par défaut).
	qreal mMayLivedUntil;

private:
	QTransform transformToParent();
	QTransform transformToScene();
};










#endif // Q_PHYSICAL_BODY_H