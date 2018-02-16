#ifndef Q_SHAPE_BODY_H
#define Q_SHAPE_BODY_H


#include <QPainter>

//!	
//!	\brief QShapeBody est une classe abstraite représentant les caractéristiques géométriques d'un corps physique de QPhysicalBody.
//!	
//!	Cette classe permet la définition des paramètres physiques et visuelle d'un corps par :
//!	 - la définition géométrique de la forme
//!	 - la définition cosmétique (couleur et autre)
//!	 - l'affichage de la forme
//!	 - les calculs :
//!		- d'inertie :
//!			- linéaire (masse)
//!			- angulaire (moment d'inertie)
//!		- de centroïde
//!	
//!	Cette classe est abstraite et ne peut être instanciée. Trois fonctions doivent être surchargée : 
//!	 - clone()
//!	 - draw()
//!	 - processInertia()
//!	
//!	\section qsb_physics Physique
//!	
//!	La fonction à surcharger processInertia() doit calculer et assigner adéquatement les variables suivantes :
//!	 - mCentroid - le centroïde (ou le centre de masse) de la forme 
//!	 - mLinearInertia - le moment d'inertie linéaire ou plus communément appelé la masse
//!	 - mAngularInertia - le moment d'inertie angulaire ou plus communément appelé le moment d'inertie tout simplement
//!	
//!	Le moment d'inertie angulaire doit être calculée par rapport au centroïde et l'axe Z. 
//!
//! \section qsb_display
//!
//!	La classe permet son affichage par les paramètres communs de :
//!	 - mBrush
//!	 - mPen
//!	 - draw()
//!
class QShapeBody
{
public:
	//! \brief Constructeur par défaut.
	QShapeBody();
	//! \brief Constructeur de copie par défaut.
	QShapeBody(QShapeBody const & body) = default;
	//! \brief Opérateur d'assignation par défaut.
	QShapeBody& operator=(QShapeBody const & body) = default;
	//! \brief Destructeur.
	virtual ~QShapeBody() = default;

	//! \brief Fonction virtuelle pure devant retourner une nouvelle instance identique à l'objet courant.
	virtual QShapeBody * clone() const = 0;

	//! \brief Fonction permettant de recalculer les données d'inertie. 
	//!
	//! **Important** : Cette fonction doit être rappelée dès que la forme change où est modifiée. 
	//!	Par exemple, si un cercle est déterminé selon un certain rayon lors de la contruction, c'est la responsabilité du développeur d'appeler la fonction updateInertia(). 
	//! Si le cercle est modifé en cours de simulation et que le cercle change de rayon, c'est la responsabilité du développeur d'appeler la fonction updateInertia(). 
	virtual void updateInertia();
	//! \brief Dessine la forme selon les coordonnées locales.
	//!
	//! Cette fonction doit afficher la forme selon les coordonnées locales. Cette fonction est virtuelle pure et doit être surchargée.
	virtual void draw(QPainter & painter) const = 0;
	//! \brief Dessine le centroïde de la forme.
	virtual void drawCentroid(QPainter & painter) const;
	//! \brief Dessine la forme.
	//!
	//! C'est cette fonction qui est appelée par QSceneViewController et qui appel la fonction draw().
	void paint(QPainter & painter) const;

	//! \brief Retourne le centroïde de la forme.
	QPointF centroid() const;
	//! \brief Retourne le moment d'inertie linéaire (la masse).
	qreal linearInertia() const;
	//! \brief Retourne le moment d'inertie angulaire.
	qreal angularInertia() const;

	//! \brief Retourne si le centroïde est affiché ou non.
	bool isShowingCentroid() const;
	//! \brief Détermine l'affichage du centroïde.
	void showCentroid(bool show);

	//! \brief Retourne la brosse utilisée pour dessiner la forme.
	QBrush brush() const;
	//! \brief Retourne la crayon utilisé pour dessiner la forme.
	QPen pen() const;
	//! \brief Détermine la brosse utilisée pour dessiner la forme.
	void setBrush(QBrush const & brush);
	//! \brief Détermine le crayon utilisé pour dessiner la forme.
	void setPen(QPen const & pen);
	//! \brief Détermine aléatoirement la couleur de la brosse.
	void randomizeBrushColor(qreal minHue = 0.0, qreal maxHue = 1.0, qreal minSaturation = 0.0, qreal maxSaturation = 1.0, qreal minLightness = 0.0, qreal maxLightness = 1.0);
	//! \brief Détermine aléatoirement la couleur du crayon.
	void randomizePenColor(qreal minHue = 0.0, qreal maxHue = 1.0, qreal minSaturation = 0.0, qreal maxSaturation = 1.0, qreal minLightness = 0.0, qreal maxLightness = 1.0);

	//! \brief Retourne la taille du centroïde à dessiner. La taille du centroïde est la même pour toutes les instances de la classe.
	static qreal centroidRadius();
	//! \brief Retourne la brosse du centroïde à dessiner. La brosse du centroïde est la même pour toutes les instances de la classe.
	static QBrush centroidBrush();
	//! \brief Retourne le crayon du centroïde à dessiner. Le crayon du centroïde est la même pour toutes les instances de la classe.
	static QPen centroidPen();
	//! \brief Détermien la taille du centroïde à dessiner. La taille du centroïde est la même pour toutes les instances de la classe.
	static void setCentroidRadius(qreal centroidRadius);
	//! \brief Détermien la brosse du centroïde à dessiner. La brosse du centroïde est la même pour toutes les instances de la classe.
	static void setCentroidBrush(QBrush const & centroidBrush);
	//! \brief Détermien le crayon du centroïde à dessiner. Le crayon du centroïde est la même pour toutes les instances de la classe.
	static void setCentroidPen(QPen const & centroidPen);

protected:
	//! \brief Brosse de la forme.
	QBrush mBrush;
	//! \brief Crayon de la forme.
	QPen mPen;

	//! \brief Centroïde de la forme. 
	//!
	//!	Cette variable doit être déterminée par la fonction processInertia(). Ses unités sont en _m_.
	QPointF mCentroid;					// m		center of mass
	//! \brief Moment d'inertie linéaire de la forme. 
	//!
	//!	Cette variable doit être déterminée par la fonction processInertia(). Ses unités sont en _kg_.
	qreal mLinearInertia;				// kg		mass
	//! \brief Moment d'inertie angulaire. 
	//!
	//!	Cette variable doit être déterminée par la fonction processInertia(). Ses unités sont en _kg m²_.
	qreal mAngularInertia;				// kg m²	moment of inertia at centroid : Icm
	//! \brief Fonction devant calculer les variables mCentroid, mLinearInertia et mAngularInertia. 
	//!
	//!	Cette fonction est virtuelle pure et doit être surchargée.
	virtual void processInertia() = 0;

private:
	//! \brief Variable déterminant si le centroïde est affiché.
	bool mShowCentroid;
	static qreal sCentroidRadius;
	static QBrush sCentroidBrush;
	static QPen sCentroidPen;
};



#endif // Q_SHAPE_BODY_H
