#ifndef Q_SQL_QUERY_INSERTION_H
#define Q_SQL_QUERY_INSERTION_H


#include <QSqlQuery>
#include <QSqlError>
#include <type_traits>
#include <stdexcept>
#include <QDebug>

//!	
//!	\brief QSqlQueryInsertion est une classe facilitant l'exécution de requête d'insertion.
//!	
//!	Seulement trois fonctions sont définies et s'utilisent de la façon suivante :
//!	 -# on utilise la fonction _prepare_ pour préparer la requête en définissant le nom de la table ainsi que tous les champs à insérer
//!	 -# on utilise la fonction _bind_ pour déterminer toutes les valeurs à insérer dans la requête - se sont les paramètres variables
//!	 -# on utilise la fonction _execute_ pour exécuter la requête
//!
//!	Tous les champs passés à la fonction _prepared_ doivent être de type chaîne de caractères tels que : `char const *`, `std::string` ou `QString`.
//!
//! Il est essentiel que le nombre de champs utilisés dans la fonction _prepare_ (expluant le paramètre _tableName_) soit le même que le nombre de valeurs passés à la fonction _bind_.
//!
//! Voici un exemple de code :
//!	\code{.cpp}
//!
//!	QBrush myBrush(Qt::white);
//!	QPen myPen(Qt::black, 1.0);
//!	QPolygonF polygon;
//!	polygon << QPointF(0.0, 0.0) << QPointF(10.0, 10.0) << QPointF(10.0, -10.0) << QPointF(0.0, 0.0);
//!
//!	int newId;
//!	QSqlQueryInsertion insertShapeQuery;
//!
//!	insertShapeQuery.prepare("Shape", "BrushColor", "PenColor", "PenWidth", "PolygonalShape");
//!	insertShapeQuery.bind(	QUtilities::colorToInt(myBrush.color()),	// as brush color
//!							QUtilities::colorToInt(myPen.color()),		// as pen color
//!							myPen.width(),								// as pen width
//!							QUtilities::polygonToString(polygon));		// as polygonal shape
//!	if (!insertShapeQuery.execute(newId)) {
//!		// do something
//!	}
//!
//!	\endcode
//!
class QSqlQueryInsertion
{
public:
	//! \brief Fonction initialisant la requête en tant que requête préparée (_prepared statement_).
	//!
	//! Tous les _field_ passés doivent être de type chaîne de caractères tels que : `char const *`, `std::string` ou `QString`. Cette fonction doit être appelé avant l'appel de la fonction bind.
	template <typename FirstType, typename ... RemainingTypes> void prepare(QString const & tableName, FirstType field1, RemainingTypes ... otherFields);
	//! \brief Fonction surchargée utilitaire appelant _setDatabase_ et _prepare_.
	template <typename FirstType, typename ... RemainingTypes> void prepare(QSqlDatabase const & database, QString const & tableName, FirstType field1, RemainingTypes ... otherFields);

	//! \brief Fonction faisant la liaison entre un paramètre un champ défini précédemment avec la fonction _prepare_.
	//!
	//! Chacun des champs peut être de n'importe quel type mais ils doivent correspondre aux champs destinés dans la table. L'ordre de déclaration dans la fonction _bind_ doit être exactement le même que celui utilisé dans la fonction _prepare_. Conséquemment, les deux fonctions doit avoir le même nombre de champs. 
	template <typename FirstType, typename ... RemainingTypes> void bind(FirstType field1, RemainingTypes ... otherFields);

	//! \brief Fonction exécutant la requête.
	//!
	//! Les fonctions _prepare_ et _bind_ doivent être appelées d'abord. La requête retourne _true_ si l'exécution de la requête a réussi sinon _false_. Si la requête ne s'exécute pas correctement, un message d'erreur est affiché dans la console. Finalement, le dernier identifiant est retourné avec le paramètre _lastInsertId_.
	bool execute(int & lastInsertId);
	
	//! \brief Fonction assignant la base de données.
	void setDatabase(QSqlDatabase const & database);

private:
	QSqlQuery mQuery;
	size_t mQueryNParamNBound{ 0 };
	template <typename FirstType, typename ... RemainingTypes> void prepare(QString & field, QString & values, FirstType field1, RemainingTypes ... otherFields);
	template <typename FirstType> void prepare(QString & fields, QString & values, FirstType field1);
	template <typename FirstType, typename ... RemainingTypes> void bind(FirstType field1);
};




template <typename FirstType, typename ... RemainingTypes> 
inline void QSqlQueryInsertion::prepare(QSqlDatabase const & database, QString const & tableName, FirstType field1, RemainingTypes ... otherFields) {
	setDatabase(database);
	prepare(tableName, field1, otherFields...);
}

template <typename FirstType, typename ... RemainingTypes> 
inline void QSqlQueryInsertion::prepare(QString const & tableName, FirstType field1, RemainingTypes ... otherFields) {
	QString fields;
	QString values;
	prepare(fields, values, field1, otherFields...);

	QString statement(QString("INSERT INTO %1(%2) VALUES(%3);").arg(tableName).arg(fields).arg(values));
	mQuery.prepare(statement);
	mQueryNParamNBound = sizeof...(RemainingTypes)+1;
}

template <typename FirstType, typename ... RemainingTypes> 
inline void QSqlQueryInsertion::prepare(QString & fields, QString & values, FirstType field1, RemainingTypes ... otherFields) {
	static_assert(	std::is_same<FirstType, char *>::value ||
					std::is_same<FirstType, char const *>::value ||
					std::is_same<FirstType, std::string>::value ||
					std::is_same<FirstType, QString>::value, "Invalid template  argument in QSqlQueryPrepared::prepare : T must be of string type (char *, char const *, std::string or QString");
	fields += QString(field1) + ", ";
	values += "?, ";
	prepare(fields, values, otherFields ...);
}

template <typename FirstType> 
inline void QSqlQueryInsertion::prepare(QString & fields, QString & values, FirstType field1) {
	static_assert(	std::is_same<FirstType, char *>::value ||
					std::is_same<FirstType, char const *>::value ||
					std::is_same<FirstType, std::string>::value ||
					std::is_same<FirstType, QString>::value, "Invalid template  argument in QSqlQueryPrepared::prepare : T must be of string type (char *, char const *, std::string or QString");
	fields += field1;
	values += "?";
}

template <typename FirstType, typename ... RemainingTypes> 
inline void QSqlQueryInsertion::bind(FirstType field1, RemainingTypes ... otherFields) {
	// to do : add an intermediate function to call this condition!
	// 
	//if (sizeof...(RemainingTypes) + 1 != mQueryNParamNBound) {
	//	throw std::invalid_argument("Invalid template argument in QSqlQueryPrepared::bind : the number of argument passed to bind must be the same previously used for each field of prepare.");
	//} 
	//

	mQuery.addBindValue(field1);
	bind(otherFields...);
}

template <typename FirstType, typename ... RemainingTypes> 
inline void QSqlQueryInsertion::bind(FirstType field1) {
	mQuery.addBindValue(field1);
}

inline bool QSqlQueryInsertion::execute(int & lastInsertId) {
	bool querySucceed{ mQuery.exec() };

	if (!querySucceed) {
		qDebug() << "QSqlQuery error : " << mQuery.lastError().text() << " with [" << mQuery.lastQuery() << "]";
	}

	lastInsertId = mQuery.lastInsertId().toInt();

	return querySucceed;
}

inline void QSqlQueryInsertion::setDatabase(QSqlDatabase const & database) {
	mQuery = QSqlQuery(database);
}

#endif // Q_SQL_QUERY_INSERTION_H
