#ifndef Q_INT_VALUE_BOX_H
#define Q_INT_VALUE_BOX_H


#include <QWidget>
class QScrollBar;
class QSpinBox;
class QLabel;


//!	
//!	\brief QIntValueBox est un widget composé de QScrollBar et QSpinBox facilitant l'intéraction avec l'usager pour la détermination d'un nombre entier.
//!	
//!	QIntValueBox dispose horizontalement un QScrollBar ainsi qu'un QSpinBox. Les deux widgets sont synchronisés et donnent un outil puissant pour déterminer un nombre entier.
//!	
//!	Optionnellement, il est possible d'ajouter à gauche un titre et à droite un champ quelconque principalement utilisé pour des unités. Voir les fonctions addTitle() et addUnit().
//!	
//!	La largeur du QSpinBox peut être fixé afin de laisser toute la place au QScrollBar. Voir la fonction setSpinFixedWidth().
//!
//! Les fonctions value(), setValue() (_slot_), setValueQuiet() et setRange() (_slot_) permettent l'interaction avec le nombre. La fonction setValueQuiet() est identique à setValue() sans émettre de signal de changement de valeur.
//!
//! Les signaux rangeChanged() et valueChanged() sont émit lorsque ces valeurs sont modifiées.
//!
//!	Les fonctions setPageStep() et setSingleStep() sont disponibles et synchronisés.
//!	
class QIntValueBox : public QWidget
{
	Q_OBJECT

public:
	QIntValueBox(QWidget * parent = nullptr);
	~QIntValueBox();

	int value() const;

	void setSpinFixedWidth(size_t spinBoxWidth = 0);
	void addTitle(QString const & title, size_t width = 0, Qt::Alignment alignment = Qt::AlignLeft | Qt::AlignVCenter);
	void addUnit(QString const & unit, size_t width = 0, Qt::Alignment alignment = Qt::AlignCenter);

	void setPageStep(int step);
	void setSingleStep(int step);

	void setValueQuiet(int value);

public slots:
	void setRange(int min, int max);
	void setValue(int value);

signals:
	void rangeChanged(int min, int max);
	void valueChanged(int value);

protected:
	QScrollBar * mScrollBar;
	QSpinBox * mSpinBox;

protected slots:
	void setValueFromSpinBox(int value);
	void setValueFromScrollBar(int value);
};

#endif // Q_INT_VALUE_BOX_H
