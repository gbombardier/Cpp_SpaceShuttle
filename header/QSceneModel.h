#ifndef Q_SCENE_MODEL_H
#define Q_SCENE_MODEL_H


#include <QList>
#include <QRectF>
//#include <QRadialGradient>
#include "QShuttle.h"
#include "QShuttleThrusterFume.h"

class QSceneViewController;

//!
//!	\brief QSceneModel est le modèle de la simulation.
//!	
//!	QSceneModel possède tous les corps de la simulation :
//!	 - corps
//!	 - navette
//!	 - fumée
//!	
//!	Tous les corps sauf la fumée subissent une attraction gravitationnelle les uns par rapport aux autres.
//!	
//!	Une taille doit être déterminée pour l'affichage. Néanmoins, cette taille ne sert qu'à l'affichage. Les objets à l'extérieur de la zone d'affichage existent toujours et sont toujours traités.
//!	
//!	La fonction setOneShuttle permet de déterminer la navette de la simulation. La fonction addThrustersFume() permet l'ajout de la fumée provenant d'un propulseur.
//!
//! La fonction tic() est le noyau de la simulation en s'assurant que tous les éléments de la simulation fassent un pas et que leur interactions sont adéquates (incluant leur retrait si nécessaire). 
//! Toutes les navettes sont indépendantes les unes par rapport aux autres lors de la simulation. Elles n'interagissent pas ensemble.
//!	
class QSceneModel
{
	friend QSceneViewController;

public:
	QSceneModel(QSizeF const & size = QSizeF(100.0, 100.0));
	QSceneModel(QRectF const & rect);
	~QSceneModel();

	void setSize(qreal width, qreal height);
	void setSize(QSizeF size);
	void setRect(QRectF const & rect);
	QRectF rect() const;

	//void setBackgroundColor(QColor const & centerColor, QColor const & edgeColor);

	void tic(qreal elapsedTime);

	void addBody(QPhysicalBody * body);
	void addThrustersFume(QShuttleThrusterFume * body);
	void setOneShuttle(QShuttle * shuttle);

protected:
	QSceneViewController * mSceneViewController;
	QRectF mRect;
	//QColor mBackgroundCenterColor, mBackgroundEdgeColor;
	//QRadialGradient mBackgroundGradient;
	//void updateBackground();

	QList<QPhysicalBody*> mBodies;
	QList<QShuttle*> mShuttles;
	QList<QShuttleThrusterFume*> mThrustersFume;
};


#endif // Q_SCENE_MODEL_H