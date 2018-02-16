#ifndef Q_COLOR_BOX_H
#define Q_COLOR_BOX_H


#include <QLabel>


//!	
//!	\brief QColorBox est un widget permettant l'interaction d'une couleur.
//!	
//!	QColorBox affiche la couleur spécifiée et permet son édition via la fenêtre de dialogue standard par un simple click.
//!	
//!	Les fonctions color() (slot) et setColor() permettent au programmeur d'interagir avec la couleur.
//!	
//!	QColorBox offre aussi une zone de génération aléatoire de la couleur. Il est possible d'activer ou non cette fonctionnalité avec enableRandomGenerator(). 
//!	La fonction setRandomRanges() détermine les étendues des paramètres teinte, saturation et luminosité pour la génération aléatoire. 
//!	Ces valeurs doivent être déterminée entre [0.0, 1.0].
//!
//! Le signal colorChanged() est émit lorsque la couleur est changée.
//!	
class QColorBox : public QLabel
{
	Q_OBJECT

public:
	QColorBox(QWidget * parent = nullptr);
	QColorBox(QColor const & color, bool randomGeneratorEnabled = true, QWidget * parent = nullptr);
	~QColorBox();

	QColor color() const { return mColor; }

	bool isRandomGeneratorEnabled() const { return mRandomGeneratorEnabled; }
	void enableRandomGenerator(bool enabled) { mRandomGeneratorEnabled = enabled; }
	void setRandomRanges(qreal hueMin, qreal hueMax, qreal saturationMin, qreal saturationMax, qreal lightnessMin, qreal lightnessMax);

public slots:
	void setColor(QColor const & color);

signals:
	void colorChanged(QColor const & color);

protected:
	bool mRandomGeneratorEnabled{ true };
	QColor mColor;
	qreal mHueMin;
	qreal mHueMax;
	qreal mSaturationMin;
	qreal mSaturationMax;
	qreal mLightnessMin;
	qreal mLightnessMax;

	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void paintEvent(QPaintEvent *event) override;
	
};

#endif // Q_COLOR_BOX_H
