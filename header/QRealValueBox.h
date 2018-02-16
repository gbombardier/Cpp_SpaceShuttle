#ifndef Q_REAL_VALUE_BOX_H
#define Q_REAL_VALUE_BOX_H


#include <QWidget>

class QScrollBar;
class QDoubleSpinBox;


//!	
//!	\brief QRealValueBox est un widget composé de QScrollBar et QDoubleSpinBox facilitant l'intéraction avec l'usager pour la détermination d'un nombre à point flottant.
//!	
//!	QRealValueBox dispose horizontalement un QScrollBar ainsi qu'un QDoubleSpinBox. Les deux widgets sont synchronisés et donnent un outil puissant pour déterminer un nombre à point flottant.
//!	
//!	Il est possible de déterminer la résolution du QScrollBar et la précision du QDoubleSpinBox par les fonctions decimals() et setDecimals().
//! 
//!	Optionnellement, il est possible d'ajouter à gauche un titre et à droite un champ quelconque principalement utilisé pour des unités. Voir les fonctions addTitle() et addUnit().
//!	
//!	La largeur du QDoubleSpinBox peut être fixé afin de laisser toute la place au QScrollBar. Voir la fonction setSpinFixedWidth().
//!
//! Les fonctions value(), setValue() (_slot_), setValueQuiet() et setRange() (_slot_) permettent l'interaction avec le nombre. La fonction setValueQuiet() est identique à setValue() sans émettre de signal de changement de valeur.
//!
//! Les signaux rangeChanged() et valueChanged() sont émit lorsque ces valeurs sont modifiées.
//!
//!	Les fonctions setPageStep() et setSingleStep() sont disponibles et synchronisés.
//!	
class QRealValueBox : public QWidget
{
	Q_OBJECT

public:
	QRealValueBox(int precision = 3, QWidget * parent = nullptr);
	~QRealValueBox();

	qreal value() const;

	int decimals() const;
	void setDecimals(int precision);

	void setSpinFixedWidth(size_t spinBoxWidth = 0);
	void addTitle(QString const & title, size_t width = 0, Qt::Alignment alignment = Qt::AlignLeft | Qt::AlignVCenter);
	void addUnit(QString const & unit, size_t width = 0, Qt::Alignment alignment = Qt::AlignCenter);

	void setPageStep(qreal step);
	void setSingleStep(qreal step);

	void setValueQuiet(qreal value);

public slots:
	void setRange(qreal min, qreal max);
	void setValue(qreal value);

signals:
	void rangeChanged(qreal min, qreal max);
	void valueChanged(qreal value);

protected:
	int mDecimals;
	QScrollBar * mScrollBar;
	QDoubleSpinBox * mSpinBox;

	qreal valueFromScrollBar(int value);
	int scrollBarFromValue(qreal value);

protected slots:
	void setValueFromSpinBox(qreal value);
	void setValueFromScrollBar(int value);
};


#endif // Q_REAL_VALUE_BOX_H
