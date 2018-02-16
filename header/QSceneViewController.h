#ifndef Q_SCENE_VIEW_CONTROLLER
#define Q_SCENE_VIEW_CONTROLLER


#include <QLabel>
#include <QPainter>
#include "QSceneModel.h"


//!
//!	\brief QSceneViewController est l'équivalent de la vue et du controlleur de la simulation.
//!	
//!	QSceneViewController permet l'affichage mais aussi les interactions avec la vue pendant la simulation. D'autres interactions restent possibles avec le modèle via d'autres vues ou d'autres contrôlleurs.
//!	
//!	La fonction sceneModel() permet l'association de cette classe au modèle attendu.
//!	
//!	Il est possible de centrer la vue au centre du rectangle défini dans le modèle (voir QSceneModel::rect()) ou sur une navette en particulier. Les fonctions resetTarget() et targetShuttle() permettent cette fonctionnalité.
//!	
class QSceneViewController : public QLabel
{
	friend QSceneModel;

public:
	QSceneViewController();
	~QSceneViewController();

	QSceneModel * sceneModel() { return mSceneModel; }
	void setSceneModel(QSceneModel * sceneModel);

	void resetTarget();
	void targetShuttle(int index = 0);

protected:
	QPhysicalBody * mBodyToTarget{ nullptr };
	QSceneModel * mSceneModel{ nullptr };

	virtual void keyPressEvent(QKeyEvent *event) override;
	virtual void keyReleaseEvent(QKeyEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;
	virtual void paintEvent(QPaintEvent *event) override;

private:
	struct Star {
		QPointF position;
		QColor color;
		qreal size;
		void paint(QPainter & painter) const;
		void randomize(qreal positionSpan, qreal sizeMin, qreal sizeMax);
	};
	QVector<Star> mStars;
};


#endif // Q_SCENE_VIEW_CONTROLLER