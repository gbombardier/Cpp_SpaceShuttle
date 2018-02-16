#include "QShuttlePostgresqlDatabase.h"


#include <QSqlQuery>
#include <QDebug>

#include <QUtilities.h>

#include <QShuttle.h>
#include <QPolygonalBody.h>
#include <QShuttleFuelTank.h>
#include <QShuttleThruster.h>
#include <QThrusterKeyboardController.h>


QShuttlePostgresqlDatabase::QShuttlePostgresqlDatabase(){

}

QShuttlePostgresqlDatabase::~QShuttlePostgresqlDatabase(){
}

bool QShuttlePostgresqlDatabase::connect(QString const & hostName, size_t port, QString const & userName, QString const & password, QString const & databaseName){
	mDatabase = QSqlDatabase::addDatabase("QPSQL");
	mDatabase.setHostName(hostName);
	mDatabase.setPort(port);
	mDatabase.setUserName(userName);
	mDatabase.setPassword(password);
	mDatabase.setDatabaseName(databaseName);

	bool dbOpen{ mDatabase.open() };

	if (!dbOpen) {
		qDebug() << "Database connection error : " << mDatabase.lastError().text();
	}
	qDebug() << "Database connection successful!";

	initializePreparedQueries();

	return dbOpen;
}

QStringList QShuttlePostgresqlDatabase::availableShuttles() const{
	QStringList shuttles;
	QSqlQuery query;
	bool success{ query.exec("SELECT name FROM shuttle;") };

	if (!success) {
		dbOutputDebugError(query);
	} else {
		while (query.next()) {
			shuttles << query.value(0).toString();
		}
	}

	return shuttles;
}

bool QShuttlePostgresqlDatabase::isShuttleExists(QString const & name){
	QSqlQuery query;
	query.prepare("SELECT count(*) FROM shuttle WHERE name = ?;");
	query.bindValue(0, name);

	bool success{ query.exec() };
	if (!query.exec()) {
		return false;
	}

	query.next();
	return query.value(0).toInt() == 1;
}

bool QShuttlePostgresqlDatabase::retrieveShuttle(QShuttle * shuttle, QString const & name, QSceneModel & sceneModel){
	//mDatabase.transaction();

	modifShuttle(shuttle, name, sceneModel);
	modifFuelTank(shuttle, name, sceneModel);
	modifThruster(shuttle, name, sceneModel);

	//mDatabase.commit();

	return true;
}

QList<QVariant> QShuttlePostgresqlDatabase::retourneShape(int shapeId) {
	mDatabase.transaction();

	QList<QVariant> infosShape;

	QSqlQuery mQuery(mDatabase);
	mQuery.prepare("SELECT * FROM Shape WHERE id =?;");
	mQuery.bindValue(0, shapeId);
	mQuery.exec();
	qDebug() << "Select : " << mQuery.size();

	if (mQuery.size() == -1) {
		mDatabase.rollback();
	}
	else {
		while (mQuery.next()) {
			int brushColor = mQuery.value(1).toInt();
			int penColor = mQuery.value(2).toInt();
			qreal penWidth = mQuery.value(3).toReal();
			QString polygonalShape = mQuery.value(4).toString();
			infosShape << shapeId << brushColor << penColor << penWidth << polygonalShape;

		}
		qDebug() << shuttleShapeList;
	}

	mDatabase.commit();
	return infosShape;
}

bool QShuttlePostgresqlDatabase::modifShuttle(QShuttle * shuttle, QString const & name, QSceneModel & sceneModel) {
	// Retrieving the shuttle
	mDatabase.transaction();

	QSqlQuery mQuery(mDatabase);
	mQuery.prepare("SELECT * FROM Shuttle WHERE name =?;");
	mQuery.bindValue(0, name);

	mQuery.exec();
	qDebug() << "Select : " << mQuery.size();

	if (mQuery.size() == -1) {
		mDatabase.rollback();
		return false;
	}
	else {
		while (mQuery.next()) {
			int serial = mQuery.value(0).toInt();
			QString name = mQuery.value(1).toString();

			QList<QVariant> infosShape = retourneShape(mQuery.value(2).toInt());
			QPolygonF shape = QUtilities::polygonFromString(infosShape[4].toString());

			qreal surfaceMass = mQuery.value(3).toReal();
			QPointF linearPosition = QUtilities::pointFromString(mQuery.value(4).toString());
			QPointF linearSpeed = QUtilities::pointFromString(mQuery.value(5).toString());
			qreal angularPosition = mQuery.value(6).toReal();
			qreal angularSpeed = mQuery.value(7).toReal();
			
			shuttleList << serial << name << infosShape << shape << surfaceMass << linearPosition << linearSpeed << angularPosition << angularSpeed;

			static_cast<QPolygonalBody*>(shuttle->shape())->setPolygon(shape);
			shuttle->shape()->setBrush(QUtilities::colorFromInt(infosShape[1].toInt()));
			shuttle->shape()->setPen(QUtilities::colorFromInt(infosShape[2].toInt()));
			shuttle->setSurfaceMass(surfaceMass);
			shuttle->setLinearPosition(linearPosition);
			shuttle->setLinearSpeed(linearSpeed);
			shuttle->setAngularPosition(angularPosition);
			shuttle->setAngularSpeed(angularSpeed);

		}
		qDebug() << shuttleList;
	}



	mDatabase.commit();
	return true;
}

bool QShuttlePostgresqlDatabase::modifFuelTank(QShuttle * shuttle, QString const & name, QSceneModel & sceneModel) {
	mDatabase.transaction();
	QSqlQuery mQuery(mDatabase);

	// Retrieving the fueltank
	int shuttleId;
	mQuery.prepare("SELECT Id FROM Shuttle WHERE name =?;");
	mQuery.bindValue(0, name);
	mQuery.exec();
	mQuery.next();
	shuttleId = mQuery.value(0).toInt();


	mQuery.prepare("SELECT * FROM fueltank WHERE shuttle =?;");
	mQuery.bindValue(0, shuttleId);
	mQuery.exec();
	qDebug() << "Select : " << mQuery.size();

	if (mQuery.size() == -1) {
		mDatabase.rollback();
		return false;
	}
	else {
		while (mQuery.next()) {
			int serial = mQuery.value(0).toInt();
			mShuttleID = mQuery.value(1).toInt();
			qreal capacity = mQuery.value(2).toReal();
			qreal fuelLevel = mQuery.value(3).toReal();
			qreal tankWidth = mQuery.value(4).toReal();
			qreal tankHeight = mQuery.value(5).toReal();
			QPointF linearPosition = QUtilities::pointFromString(mQuery.value(6).toString());
			qreal angularPosition = mQuery.value(7).toReal();
			QColor fuelColor = QUtilities::colorFromInt(mQuery.value(8).toInt());

			fueltankList << serial << mShuttleID << capacity << fuelLevel << linearPosition << tankWidth << linearPosition << angularPosition << fuelColor;

			shuttle->fuelTanks()[0]->setCapacity(capacity);
			shuttle->fuelTanks()[0]->fill();
			shuttle->fuelTanks()[0]->takeFuel((1 - fuelLevel)*shuttle->fuelTanks()[0]->capacity());
			shuttle->fuelTanks()[0]->setTankSize(tankWidth, tankHeight);
			shuttle->fuelTanks()[0]->setLinearPosition(linearPosition);
			shuttle->fuelTanks()[0]->setAngularPosition(angularPosition);
			shuttle->fuelTanks()[0]->setFuelColor(fuelColor);
		}
		qDebug() << fueltankList;
	}

	

	mDatabase.commit();
	return true;
}

bool QShuttlePostgresqlDatabase::modifThruster(QShuttle * shuttle, QString const & name, QSceneModel & sceneModel) {
	mDatabase.transaction();
	QSqlQuery mQuery(mDatabase);

	int shuttleId;
	mQuery.prepare("SELECT Id FROM Shuttle WHERE name =?;");
	mQuery.bindValue(0, name);
	mQuery.exec();
	mQuery.next();
	shuttleId = mQuery.value(0).toInt();
	

	//// Retrieving the thrusters
	mQuery.prepare("SELECT * FROM thruster WHERE shuttle =?;");
	mQuery.bindValue(0, shuttleId);
	mQuery.exec();
	qDebug() << "Select : " << mQuery.size();

	if (mQuery.size() == -1) {
		mDatabase.rollback();
		return false;
	}
	else {
		shuttle->removeThrusters();
		int index = 0;
		while (mQuery.next()) {
			int serial = mQuery.value(0).toInt();
			int shuttleId = mQuery.value(1).toInt();

			QList<QVariant> infosShape = retourneShape(mQuery.value(2).toInt());
			QPolygonF shape = QUtilities::polygonFromString(infosShape[4].toString());

			int fuelTank = mQuery.value(3).toInt();
			qreal massFlowRate = mQuery.value(4).toReal();
			qreal massEjectedSpeed = mQuery.value(5).toReal();
			QPointF linearPosition = QUtilities::pointFromString(mQuery.value(6).toString());
			qreal angularPosition = mQuery.value(7).toReal();
			QString keySequence = mQuery.value(8).toString();

			//thrusterList[index] << serial << shuttleId << shape << fuelTank << massFlowRate << massEjectedSpeed << linearPosition << angularPosition << keySequence;

			// Step 1 - Build thruster with a polygonal shape (instead of the default circular shape) 
			QShuttleThruster * thruster = new QShuttleThruster(new QPolygonalBody);
			// Step 2 - Assign the polygonal shape - in this case, we use an equilateral polygon
			static_cast<QPolygonalBody*>(thruster->shape())->setPolygon(shape);
			// Step 3 - Assign brush and pen to the shape
			thruster->shape()->setBrush(QUtilities::colorFromInt(infosShape[1].toInt()));
			thruster->shape()->setPen(QUtilities::colorFromInt(infosShape[2].toInt()));
			// Step 4 - Assign model to the thruster
			thruster->setSceneModel(&sceneModel);
			// Step 5 - Assign the fuel tank
			thruster->linkToFuelTank(shuttle->fuelTanks()[0]);
			// Step 6 - Set flow rate and eject speed
			thruster->setThrusterEfficiency(massFlowRate, massEjectedSpeed);
			// Step 7 - Create and assign the keyboard controller
			thruster->setController(new QThrusterKeyboardController(keySequence));
			// Step 8 - Assign the thruster to the shuttle
			shuttle->addThruster(thruster, linearPosition, Trigo<>::deg2rad(angularPosition));
			index++;
		}

		qDebug() << thrusterList;
	}

	mDatabase.commit();
	return true;
}

bool QShuttlePostgresqlDatabase::insertShuttle(QShuttle * shuttle){
	mDatabase.transaction();

	// Inserting shuttle's shape
	int insertedShapeId;
	if (!insertShape(static_cast<QPolygonalBody*>(shuttle->shape()), insertedShapeId)) {
		mDatabase.rollback();
		return false;
	}

	// Inserting shuttle's info
	int insertedShuttleId;
	mInsertShuttleQuery.bind(	shuttle->name(),
								insertedShapeId,
								shuttle->surfaceMass(),
								QUtilities::pointToString(shuttle->linearPosition()),
								QUtilities::pointToString(shuttle->linearSpeed()),
								shuttle->angularPosition(),
								shuttle->angularSpeed());
	if (!mInsertShuttleQuery.execute(insertedShuttleId)) {
		mDatabase.rollback();
		return false;
	}

	// Inserting fuel tank's info
	int insertedFuelTankId;
	QShuttleFuelTank * fuelTank{ shuttle->fuelTanks()[0] };
	mInsertFuelTankQuery.bind(	insertedShuttleId,
								fuelTank->capacity(),
								fuelTank->fuelLevelRatio(),
								fuelTank->tankSize().width(),
								fuelTank->tankSize().height(),
								QUtilities::pointToString(fuelTank->linearPosition()),
								fuelTank->angularPosition(),
								QUtilities::colorToInt(fuelTank->fuelColor()));
	if (!mInsertFuelTankQuery.execute(insertedFuelTankId)) {
		mDatabase.rollback();
		return false;
	}

	// Inserting all thrusters
	for (auto const & thruster : shuttle->thrusters()) {
		// Inserting thruster's shape
		if (!insertShape(static_cast<QPolygonalBody*>(thruster->shape()), insertedShapeId)) {
			mDatabase.rollback();
			return false;
		}

		// Inserting thruster's info
		int insertedThrusterId;
		mInsertThrusterQuery.bind(	insertedShuttleId,
									insertedShapeId,
									insertedFuelTankId,
									thruster->massFlowRate(),
									thruster->massEjectedSpeed(),
									QUtilities::pointToString(thruster->linearPosition()),
									thruster->angularPosition(),
									QUtilities::keySequenceToString(static_cast<QThrusterKeyboardController*>(thruster->controller())->linkedKey()));
		if (!mInsertThrusterQuery.execute(insertedThrusterId)) {
			mDatabase.rollback();
			return false;
		}
	}

	mDatabase.commit();
	return true;
}

bool QShuttlePostgresqlDatabase::updateShuttle(QShuttle * shuttle, QString const & name){
	mDatabase.transaction();
	
	if (deleteShuttle(name)) {
		if (insertShuttle(shuttle)) {
			mDatabase.commit();
		}else {
			mDatabase.rollback();
			return false;
		}
	}else {
		mDatabase.rollback();
		return false;
	}

	return true;
}

bool QShuttlePostgresqlDatabase::deleteShuttle(QString const & name) {
	mDatabase.transaction();

	QSqlQuery mQuery(mDatabase);
	mQuery.prepare("SELECT * FROM Shuttle WHERE name = '" + name + "';");

	mQuery.exec();
	qDebug() << "Select : " << mQuery.size();

	int idShuttleDelete = 0;
	while (mQuery.next()) {
		idShuttleDelete = mQuery.value(0).toInt();
	}

	removeConstraints();


	//Delete Thruster
	QSqlQuery mDeleteThrusterQuery(mDatabase);
	mDeleteThrusterQuery.prepare("DELETE FROM Thruster WHERE Shuttle =? ;");
	mDeleteThrusterQuery.bindValue(0, idShuttleDelete);

	bool successThruster{ mDeleteThrusterQuery.exec() };
	if (successThruster) {
		qDebug() << "Delete done!";
	}
	else {
		qDebug() << "Delete error : " << mDeleteThrusterQuery.lastError().text();
		mDatabase.rollback();
		return false;
	}

	//Delete Fuel Tank
	QSqlQuery mDeleteFuelTankQuery(mDatabase);
	mDeleteFuelTankQuery.prepare("DELETE FROM FuelTank WHERE Shuttle =? ;");
	mDeleteFuelTankQuery.bindValue(0, idShuttleDelete);

	bool successFuelTank{ mDeleteFuelTankQuery.exec() };
	if (successFuelTank) {
		qDebug() << "Delete done!";
	}
	else {
		qDebug() << "Delete error : " << mDeleteFuelTankQuery.lastError().text();
		mDatabase.rollback();
		return false;
	}
	//Delete Shuttle
	QSqlQuery mDeleteShuttleQuery(mDatabase);
	mDeleteShuttleQuery.prepare("DELETE FROM Shuttle WHERE Id =? ;");
	mDeleteShuttleQuery.bindValue(0, idShuttleDelete);

	bool successShuttle{ mDeleteShuttleQuery.exec() };
	if (successShuttle) {
		qDebug() << "Delete done!";
		addConstraints();
	}
	else {
		qDebug() << "Delete error : " << mDeleteShuttleQuery.lastError().text();
		mDatabase.rollback();
		return false;
	}

	mDatabase.commit();
	return true;
}

void QShuttlePostgresqlDatabase::initializePreparedQueries(){
	mInsertShuttleQuery.prepare(mDatabase, "Shuttle", "Name", "Shape", "Surfacemass", "Linearposition", "Linearspeed", "Angularposition", "Angularspeed");
	mInsertShapeQuery.prepare(mDatabase, "Shape", "BrushColor", "PenColor", "PenWidth", "PolygonalShape");
	mInsertFuelTankQuery.prepare(mDatabase, "FuelTank", "Shuttle", "Capacity", "FuelLevel", "TankWidth", "TankHeight", "LinearPosition", "AngularPosition", "FuelColor");
	mInsertThrusterQuery.prepare(mDatabase, "Thruster", "Shuttle", "Shape", "FuelTank", "MassFlowRate", "MassEjectedSpeed", "LinearPosition", "AngularPosition", "KeySequence");


}

void QShuttlePostgresqlDatabase::removeConstraints() {
	mDatabase.transaction();
	QSqlQuery alterTable;

	QString query1("ALTER TABLE Shuttle DROP CONSTRAINT fk_shuttle_shape;");
	QString query2("ALTER TABLE FuelTank DROP CONSTRAINT fk_tank_shuttle;");
	QString query3("ALTER TABLE Thruster DROP CONSTRAINT fk_thruster_shuttle;");
	QString query4("ALTER TABLE Thruster DROP CONSTRAINT fk_thruster_shape;");
	QString query5("ALTER TABLE Thruster DROP CONSTRAINT fk_thruster_tank;");

	alterTable.exec(query1);
	alterTable.exec(query2);
	alterTable.exec(query3);
	alterTable.exec(query4);
	alterTable.exec(query5);

	mDatabase.commit();
}

void QShuttlePostgresqlDatabase::addConstraints() {
	mDatabase.transaction();
	QSqlQuery alterTable;

	QString query1("ALTER TABLE Shuttle ADD CONSTRAINT fk_shuttle_shape FOREIGN KEY(Shape) REFERENCES Shape(Id);");
	QString query2("ALTER TABLE FuelTank ADD CONSTRAINT fk_tank_shuttle FOREIGN KEY(Shuttle) REFERENCES Shuttle(Id);");
	QString query3("ALTER TABLE Thruster ADD CONSTRAINT fk_thruster_shuttle FOREIGN KEY(Shuttle) REFERENCES Shuttle(Id);");
	QString query4("ALTER TABLE Thruster ADD CONSTRAINT fk_thruster_shape FOREIGN KEY(Shape) REFERENCES Shape(Id);");
	QString query5("ALTER TABLE Thruster ADD CONSTRAINT fk_thruster_tank FOREIGN KEY(FuelTank) REFERENCES FuelTank(Id);");

	alterTable.exec(query1);
	alterTable.exec(query2);
	alterTable.exec(query3);
	alterTable.exec(query4);
	alterTable.exec(query5);

	mDatabase.commit();
}

bool QShuttlePostgresqlDatabase::insertShape(QPolygonalBody * shape, int & newShapeId){
	mInsertShapeQuery.bind(	QUtilities::colorToInt(shape->brush().color()),
							QUtilities::colorToInt(shape->pen().color()),
							shape->pen().width(),
							QUtilities::polygonToString(shape->polygon()));
	return mInsertShapeQuery.execute(newShapeId);
}

bool QShuttlePostgresqlDatabase::deleteShuttleProf(QString const & name)
{
	mDatabase.transaction();

	//QList<QVariant> shapeIds;
	QString shapeIds;
	mAllShapesFromShuttle.bindValue(0, name);
	mAllShapesFromShuttle.bindValue(1, name);
	//if (!QUtilities::sqlGetIds(mAllShapesFromShuttle, shapeIds)) {
	//	return false;
	//}

	// Delete all thrusters
	mDeleteThrusterFromShuttle.bindValue(0, name);
	if (!mDeleteThrusterFromShuttle.exec()) {
		qDebug() << "QSqlQuery error : " << mDeleteThrusterFromShuttle.lastError().text();
		mDatabase.rollback();
		return false;
	}

	// Delete the fuel tank
	mDeleteFuelTankFromShuttle.bindValue(0, name);
	if (!mDeleteFuelTankFromShuttle.exec()) {
		qDebug() << "QSqlQuery error : " << mDeleteFuelTankFromShuttle.lastError().text();
		mDatabase.rollback();
		return false;
	}

	// Delete the shuttle
	QSqlQuery query4;
	mDeleteShuttle.bindValue(0, name);
	if (!mDeleteShuttle.exec()) {
		qDebug() << "QSqlQuery error : " << mDeleteShuttle.lastError().text();
		mDatabase.rollback();
		return false;
	}

	// Delete all the shapes (including shuttle + thrusters)
	// ... first approach
	//for (auto id : shapeIds) {
	//	mDeleteShapeFromId.bindValue(0, id);
	//	if (!mDeleteShapeFromId.exec()) {
	//		qDebug() << "QSqlQuery error : " << mDeleteShapeFromId.lastError().text();
	//		mDatabase.rollback();
	//		return false;
	//	}
	//}
	// ... second approach
	QString sqlDeleteShapeFromId(QString("DELETE FROM shape WHERE id IN (%1);").arg(shapeIds));
	QSqlQuery queryDeleteShapeFromId;
	if (!queryDeleteShapeFromId.exec(sqlDeleteShapeFromId)) {
		qDebug() << "QSqlQuery error : " << mDeleteShapeFromId.lastError().text();
		mDatabase.rollback();
		return false;
	}

	mDatabase.commit();
	return true;
}
