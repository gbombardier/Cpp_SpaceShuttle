#ifndef Q_SHUTTLE_POSTGRESQL_DATABASE_H
#define Q_SHUTTLE_POSTGRESQL_DATABASE_H


#include "QShuttleDatabase.h"
#include <QSqlQueryInsertion.h>
#include <qvariant.h>



class QPolygonalBody;


class QShuttlePostgresqlDatabase : public QShuttleDatabase
{
public:
	QShuttlePostgresqlDatabase();
	~QShuttlePostgresqlDatabase();

	virtual bool connect(QString const & hostName, size_t port, QString const & userName, QString const & password, QString const & databaseName) override;

	virtual QStringList availableShuttles() const override;
	virtual bool isShuttleExists(QString const & name) override;

	virtual bool retrieveShuttle(QShuttle * shuttle, QString const & name, QSceneModel & sceneModel) override;
	virtual bool insertShuttle(QShuttle * shuttle) override;
	virtual bool updateShuttle(QShuttle * shuttle, QString const & name) override;
	virtual bool deleteShuttle(QString const & name) override;
	virtual bool deleteShuttleProf(QString const & name) override;

	void removeConstraints();
	void addConstraints();

	bool modifShuttle(QShuttle * shuttle, QString const & name, QSceneModel & sceneModel);
	bool modifFuelTank(QShuttle * shuttle, QString const & name, QSceneModel & sceneModel);
	bool modifThruster(QShuttle * shuttle, QString const & name, QSceneModel & sceneModel);

	QList<QVariant> retourneShape(int shapeId);

protected:
	bool insertShape(QPolygonalBody * shape, int & newShapeId);
	
private:	
	QSqlQueryInsertion mInsertShuttleQuery;
	QSqlQueryInsertion mInsertShapeQuery;
	QSqlQueryInsertion mInsertFuelTankQuery;
	QSqlQueryInsertion mInsertThrusterQuery;

	QList<QVariant> shuttleList, fueltankList, shuttleShapeList;
	QList<QList<QVariant>> thrusterList;

	QSqlQuery mAllShapesFromShuttle;
	QSqlQuery mDeleteThrusterFromShuttle;
	QSqlQuery mDeleteFuelTankFromShuttle;
	QSqlQuery mDeleteShuttle;
	QSqlQuery mDeleteShapeFromId;

	void initializePreparedQueries();
	int mShuttleID;

};


#endif // Q_SHUTTLE_POSTGRESQL_DATABASE_H
