#ifndef Q_POLYGON_EDITOR_H
#define Q_POLYGON_EDITOR_H


#include <QWidget>
#include <QGraphicsScene>
#include <QCheckBox>
#include "QColorBox.h"
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QPushButton>

class QPolygonView;
class QPolygonItem;


//!	
//!	\brief QPolygonEditor est un widget permettant la définition interactive d'un polygone.
//!	
//!	L'éditeur présente une surface carrée uniatire où le polygone est défini. Puisque le polygone est centré, il est donc déterminé dans l'intervel [-0.5, 0.5].
//!	
//!	Un paramètre de taille relative permet de déterminer la taille d'entrée et de sortie du polygone. Les fonctions outputScale() et setOutputScale() permettent d'interagir avec cette valeur.
//!	Par exemple, si un polygone est déterminé interactivement dans l'interval [-0.5, 0.5] et qu'on désire une taille de sortie de [-100, 100], il faudra définir le facteur de taille relative à 200.0. 
//!	Inversement, lors de l'assignation d'un polygone à l'éditeur, ce dernier est redimensionné selon le facteur d'échelle relative. 
//!	
//!	Les fonctions polygon() et setPolygon() donnent accès au polygone en édition.
//!
//! L'éditeur offre aussi une visualisation du polygone avec plusieurs options. Les brosses et crayon utilisés pour l'affichage sont disponible via les fonctions brush(), setBrush(), pen() et setPen(). 
//!
//!	D'autres options sont aussi disponibles directement dans l'éditeur :
//!	 - couleur de la brosse et du crayon
//!	 - taille du crayon
//!	 - affichage ou non de l'arrière plan et des points de contrôle
//!	 - création d'un nouveau polygone avec un générateur de polygone régulier à _n_ côtés.
//!	 - information :
//!		- nombre de côtés du polygone
//!		- conformité du polygone (sans auto-croisement et défini dans le sens horaire)
//!		- position de la souris pendant l'édition
//!
//! Il est à noter que le polygone utilisé est fermé (le dernier point est égal au premier point).
//!	
class QPolygonEditor : public QWidget
{
	Q_OBJECT

public:
	QPolygonEditor(int editorScale = 500, QWidget * parent = nullptr);
	~QPolygonEditor();

	QPolygonF polygon() const;
	void setPolygon(QPolygonF polygon);

	qreal outputScale() const;
	void setOutputScale(qreal outputScale);
	//void setOutputScale(QPolygonF const & fromPolygon);

	QBrush brush() const;
	QPen pen() const;

	void setBrush(QBrush const & brush);
	void setPen(QPen const & pen);

signals:
	void polygonUpdated();

protected:
	int mEditorScale;
	QSize mEditorSize;
	QPolygonView * mPolygonView;
	QGraphicsScene mPolygonScene;
	QPolygonItem * mPolygonItem;
	QGraphicsRectItem * mUnitSquareItem;
	QGraphicsLineItem * mXAxisItem;
	QGraphicsLineItem * mYAxisItem;

	QCheckBox * mViewBackgroundCheckBox;
	QCheckBox * mViewPolygonVerticesBox;
	
	QDoubleSpinBox * mOutputScale;
	QColorBox * mBrushColorBox;
	QColorBox * mPenColorBox;
	QDoubleSpinBox * mPenWidthSpinBox;

	QSpinBox * mRegularPolygonSizeSpinBox;
	QSpinBox * mRegularPolygonOrientationSpinBox;
	QPushButton * mRegularPolygonSizeButton;

	QLabel * mPolygonSizeLabel;
	QLabel * mPolygonConformity;
	QLabel * mMousePositionLabel;

protected slots :
	void showBackground();
	void showVertices();
	void updateAppearance();
	void updateMousePosition(QPointF const & position);
	void updateMouseGone();
	void updatePolygonInfo();
	void createRegularPolygon();
};

#include <QObject>
#include <QGraphicsPolygonItem>
class QPolygonItem : public QObject, public QGraphicsPolygonItem
{
	Q_OBJECT

public:
	virtual QRectF boundingRect() const override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;

	size_t verticesCount() const { return polygon().size(); }
	qreal vertexSize() const { return mVertexSize; }

	void update();
	bool isDefinedClockwise() const { return mClockwiseDefined; }
	bool isSelfIntersecting() const { return mSelfIntersecting; }

	bool isShowingVertices() const { return mShowVertices; }

public slots:
	void setVertexSize(qreal vertexSize) { mVertexSize = vertexSize; }
	void showVertices(bool show = true) { mShowVertices = show; }

protected:
	bool mShowVertices;
	qreal mVertexSize;
	bool mClockwiseDefined;
	bool mSelfIntersecting;
};


#include <QGraphicsView>

class QPolygonView : public QGraphicsView
{
	Q_OBJECT

public:
	QPolygonView(QWidget * parent = nullptr);
	~QPolygonView() = default;

	void setPolygonItem(QPolygonItem * polygonItem) { mPolygonItem = polygonItem; }

signals:
	void mouseMoved(QPointF const & position);
	void mouseLeaved();
	void polygonEdited();

protected:
	QPolygonItem * mPolygonItem{ nullptr };
	int mVertexIndex{ -1 };
	virtual void leaveEvent(QEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;
};


#endif // Q_POLYGON_EDITOR_H
