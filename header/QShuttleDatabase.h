#ifndef Q_SHUTTLE_DATABASE_H
#define Q_SHUTTLE_DATABASE_H


#include <QSqlDatabase>
#include <QStringList>
class QShuttle;
class QSceneModel;

//!
//!	+---------------------------------------------------------------------------+
//!	| Légende : 																|
//!	|																			|
//!	| 	Contraintes						Types									|
//!	| 	----------------------			-------------------------------------	|
//!	| 	[pk] = primary key				{ s } = SERIAL		{ . } = POINT		|
//!	| 	[fk] = foreign key				{ v } = VARCHAR		{ p } = POLYGON		|
//!	| 	[*] = NOT NULL					{ i } = INTEGER							|
//!	| 	[U] = UNIQUE					{ r } = REAL							|
//!	| 	[C] = CHECK																|
//!	+---------------------------------------------------------------------------+
//!
//!
//!
//!				+-------------------------------+									+-------------------------------+
//!				|            Shuttle			|									|             Shape				|
//!				+-------------------------------+									+-------------------------------+
//!		+----->	[pk	]{ s } Id					| <-------------+			+----->	[pk ] { s } Id					| <-----+
//!		|		[*U	]{ v } Name					|				|			|		[*  ] { i } BrushColor			|		|
//!		|		[*fk]{ i } Shape				| ------------- I ----------+		[*  ] { i } PenColor			|		|
//!		|		[*	]{ r } Surfacemass			|				|					[*  ] { r } PenWidth			|		|
//!		|		[*	]{ . } Linearposition		|				|					[*  ] { p } PolygonalShape		|		|
//!		|		[*	]{ . } Linearspeed			|				|					+-------------------------------+		|
//!		|		[*	]{ r } Angularposition		|				|															|
//!		|		[*	]{ r } Angularspeed			|				|															|
//!		|		+-------------------------------+				|															|
//!		|														|															|
//!		|														|															|
//!		|														|															|
//!		|														|															|
//!		|														|															|
//!		|														|															|
//!		|		+-------------------------------+				|					+-------------------------------+		|
//!		|		|            FuelTank			| <-----+		|					|            Thruster			|		|
//!		|		+-------------------------------+		|		|					+-------------------------------+		|
//!		|		[pk	]{ s } Id					|		|		|					[pk ] { s } Id					|		|
//!		+------	[*fk]{ i } Shuttle				|		|		+------------------	[*fk] { i } Shuttle				|		|
//!				[*  ]{ r } Capacity				|		|							[*fk] { i } Shape				| ------+
//!				[*C ]{ r } FuelLevel			|		+--------------------------	[*fk] { i } FuelTank			|
//!				[*	]{ r } TankWidth			|									[*  ] { r } MassFlowRate		|
//!				[*	]{ r } TankHeight			|									[*  ] { r } MassEjectedSpeed	|
//!				[*	]{ . } LinearPosition		|									[*  ] { . } LinearPosition		|
//!				[*	]{ r } AngularPosition		|									[*  ] { r } AngularPosition		|
//!				[*	]{ i } FuelColor			|									[*  ] { v } KeySequence			|
//!				+-------------------------------+									+-------------------------------+
//!
//!
//!
class QShuttleDatabase
{
public:
	QShuttleDatabase();
	~QShuttleDatabase();

	virtual bool connect(QString const & hostName, size_t port, QString const & userName, QString const & password, QString const & databaseName) = 0;
	virtual bool isConnected() const;

	virtual QStringList availableShuttles() const = 0;
	virtual bool isShuttleExists(QString const & name) = 0;

	virtual bool retrieveShuttle(QShuttle * shuttle, QString const & name, QSceneModel & sceneModel) = 0;
	virtual bool insertShuttle(QShuttle * shuttle) = 0;
	virtual bool updateShuttle(QShuttle * shuttle, QString const & name) = 0;
	virtual bool deleteShuttle(QString const & name) = 0;
	virtual bool deleteShuttleProf(QString const & name) = 0;

protected:
	QSqlDatabase mDatabase;

	// Debugging tools
	static void dbOutputDebugAvaliableDrivers();
	void dbOutputDebugFeaturesSupport() const;
	void dbOutputDebugError() const;
	void dbOutputDebugError(QSqlQuery const & query) const;
};


#endif // Q_SHUTTLE_DATABASE_H
