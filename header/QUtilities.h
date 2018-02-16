#ifndef Q_UTILITIES_H
#define Q_UTILITIES_H


#include <QString>
#include <QStringList>
#include <QVariant>

#include <QPointF>
#include <QPolygonF>

#include <QBrush>
#include <QPen>
#include <QColor>

#include <QKeySequence>

#include <QRegularExpression>

#include <QSqlQuery>


class QUtilities final
{
public:
	QUtilities() = delete;
	~QUtilities() = delete;
	QUtilities(QUtilities const & utilities) = delete;
	QUtilities& operator=(QUtilities const & utilities) = delete;
	QUtilities(QUtilities const && utilities) = delete;
	QUtilities& operator=(QUtilities const && utilities) = delete;

	static QString pointToString(QPointF const & point, size_t decimals = 3);
	static QPointF pointFromString(QString const & pointString);

	static QString polygonToString(QPolygonF const & polygon, size_t decimals = 3);
	static QPolygonF polygonFromString(QString const & polygonString);
	
	static int colorToInt(QColor const & color);
	static QColor colorFromInt(int color);
	
	static QString keySequenceToString(QKeySequence const & keySequence);
	static QKeySequence keySequenceFromString(QString const & keySequenceString);

	static bool sqlGetId(QString const & tableName, QString const & idColumnName, QString const & fieldColumnName, QVariant const & fieldValue, QVariant & id);
	static bool sqlGetIds(QString const & tableName, QString const & idColumnName, QString const & fieldColumnName, QVariant const & fieldValue, QList<QVariant> & ids);
	static bool sqlGetIds(QSqlQuery & query, QList<QVariant> & ids);
	static bool sqlGetIds(QSqlQuery & query, QString & ids);

private:
	static QRegularExpression sRegExpCapturePoint;

};



#endif // Q_UTILITIES_H
