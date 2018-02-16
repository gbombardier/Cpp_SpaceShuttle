#include "QShuttleDatabase.h"


#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>


QShuttleDatabase::QShuttleDatabase(){
}


QShuttleDatabase::~QShuttleDatabase(){
}

bool QShuttleDatabase::isConnected() const{
	return mDatabase.isOpen();
}

void QShuttleDatabase::dbOutputDebugAvaliableDrivers(){
	qDebug() << QSqlDatabase::drivers();
}


#define Q_DATABASE_TEST_FEATURE(driver, feature) \
		qDebug() << "\t" << #feature << " : " << driver->hasFeature(feature);

void QShuttleDatabase::dbOutputDebugFeaturesSupport() const{
	if (mDatabase.isOpen()) {
		QSqlDriver * driver{ mDatabase.driver() };

		qDebug() << "Database supported features :";
		Q_DATABASE_TEST_FEATURE(driver, QSqlDriver::Transactions)
		Q_DATABASE_TEST_FEATURE(driver, QSqlDriver::QuerySize)
		Q_DATABASE_TEST_FEATURE(driver, QSqlDriver::BLOB)
		Q_DATABASE_TEST_FEATURE(driver, QSqlDriver::Unicode)
		Q_DATABASE_TEST_FEATURE(driver, QSqlDriver::PreparedQueries)
		Q_DATABASE_TEST_FEATURE(driver, QSqlDriver::NamedPlaceholders)
		Q_DATABASE_TEST_FEATURE(driver, QSqlDriver::PositionalPlaceholders)
		Q_DATABASE_TEST_FEATURE(driver, QSqlDriver::LastInsertId)
		Q_DATABASE_TEST_FEATURE(driver, QSqlDriver::BatchOperations)
		Q_DATABASE_TEST_FEATURE(driver, QSqlDriver::SimpleLocking)
		Q_DATABASE_TEST_FEATURE(driver, QSqlDriver::LowPrecisionNumbers)
		Q_DATABASE_TEST_FEATURE(driver, QSqlDriver::EventNotifications)
		Q_DATABASE_TEST_FEATURE(driver, QSqlDriver::FinishQuery)
		Q_DATABASE_TEST_FEATURE(driver, QSqlDriver::MultipleResultSets)
		Q_DATABASE_TEST_FEATURE(driver, QSqlDriver::CancelQuery)
	} else {
		qDebug() << "Database not connected.";
	}
}

#undef Q_DATABASE_TEST_FEATURE


void QShuttleDatabase::dbOutputDebugError() const{
	qDebug() << "Database error : " << mDatabase.lastError().text();
}

void QShuttleDatabase::dbOutputDebugError(QSqlQuery const & query) const{
	qDebug() << "Query error : " << query.lastError().text();
}

