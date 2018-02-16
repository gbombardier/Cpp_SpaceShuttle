#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SpaceShuttle.h"
#include "QSceneModel.h"
#include "QSceneViewController.h"
#include "QShuttle.h"
#include "QPolygonalBody.h"
#include "QShuttleKeyboardController.h"
#include "QShuttleFuelTank.h"
#include "QShuttleThruster.h"
#include "QPolygonFactory.h"
#include "QPolygonEditor.h"
#include "QThrusterKeyboardController.h"
#include "QTimer.h"
#include <QLayout.h>
#include <qcombobox.h>
#include "QRealValueBox.h"
#include "QIntValueBox.h"
#include <qgroupbox.h>
#include <qlineedit.h>
#include <QScrollBar.h>
#include <QPointF>
#include <string>
#include <QShuttleDatabase.h>
#include <QShuttlePostgresqlDatabase.h>
#include <QRectangularBody.h>

using namespace std;


class SpaceShuttle : public QMainWindow
{
	Q_OBJECT

public:
	SpaceShuttle(QWidget *parent = Q_NULLPTR);
	void creerWidgetNavette();
	void creerWidgetVehicule();
	void creerWidgetReservoir();
	void creerWidgetPropulseurs();
	void creerWidgetSimulation();
	void creerWidgetDatabase();
	void afficherReservoir();
	void modifierEditors();

private:
	Ui::SpaceShuttleClass ui;
	void addNewThruster(QSceneModel * model,
		QShuttleFuelTank * fuelTank,
		qreal thrusterSize,
		qreal massFlowRate,
		qreal massEjectSpeed,
		QPointF const & position,
		qreal orientationDegrees,
		QBrush const & brush,
		QPen const & pen,
		QKeySequence const & key);
	void SpaceShuttle::move();

	bool mSimul = true, mCentered = true;
	qreal mSimulSpeed = 30;
	int mNbThrusters = 0;
	char mKeySequences[10] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};

	QSceneViewController *mSceneVC;
	QSceneModel *mSceneModel;
	QSizeF const &mSceneSize = QSizeF(800.0, 800.0);

	QShuttle *mShuttle;
	QShuttleKeyboardController *mShuttleController;
	QShuttleFuelTank *mShuttleFuelTank;
	QShuttleThruster *mShuttleThruster;
	QPolygonEditor *mShuttleEditor, *mPropulseurEditor;

	QTimer mTimer;
	qreal elapsedTime = 0;
	int mScaleX=100, mScaleY=30;

	QHBoxLayout *mLayoutInterface, *mLayoutCaracHaut, *mLayoutCaracBas, *mLayoutShuttle;
	QVBoxLayout *mLayoutTotal, *mLayoutVerticalVehicule, *mLayoutVerticalCarac;
	QTabWidget *mTabInterface;
	QLabel *mLabelNom, *mLabelMasse, *mLabelKg;
	QLineEdit *mEditNom;
	QRealValueBox *mScrollBarCarac;
	QSpinBox *mSpinBoxcarac;
	QWidget *mWidgetVehicule, *mWidgetCarac;
	QGroupBox *mGroupBoxCarac, *mGroupBoxShuttle;

	//Pour les propulseurs
	QHBoxLayout *mLayoutHInterface, *mLayoutHPropulsion, *mLayoutHCaracHaut, *mLayoutHCaracBas, *mLayoutHShuttle, *mLayoutHSelectionProp, *mLayoutHDimension, *mLayoutHMasseProp,
		*mLayoutHDebit, *mLayoutHVitesseEjec, *mLayoutHPosH, *mLayoutHPosV, *mLayoutHOri, *mLayoutHCtrl;
	QVBoxLayout *mLayoutVTotal, *mLayoutVVehicule, *mLayoutVCarac, *mLayoutVPropulseur, *mLayoutVFormePropulseurs, *mLayoutPropulseursFinal;
	QLabel *mLabelNbPropulseurs, *mLabelSelectionProp, *mLabelDimension, *mLabelRien, *mLabelMasseProp, *mLabelKgM2, *mLabelDebit, *mLabelKg2s,
		*mLabelVitesseEjec, *mLabelMs, *mLabelPosH, *mLabelMH, *mLabelPosV, *mLabelMV, *mLabelOri, *mLabelDegrees, *mLabelToucheCtrl;
	QLineEdit *mEditShortcutProp;
	QRealValueBox
		*mScrollBarDimension, *mScrollBarMasseProp, *mScrollBarDebit, *mScrollBarVitesseEjec, *mScrollBarPosH, *mScrollBarPosV, *mScrollBarOri;
	QIntValueBox *mScrollBarNbPropulseur;
	QGroupBox
		*mGroupBoxPropulsion, *mGroupBoxPropulseur, *mGroupBoxFormeProp;
	QComboBox *mComboSelectionProp;

	//Pour le réservoir
	qreal mSize{ 0 }, mShuttleWidth{ 0 }, mShuttleHeight{ 0 };
	qreal mCapacite{ 10000000 }, mCapaciteMax{ 10000000 };
	QWidget *mTabReservoir;
	QVBoxLayout *mVerticalReservoir;
	QHBoxLayout *mHorCapaLayout, *mHorNiveauLayout, *mHorLargeurLayout, *mHorHauteurLayout, *mHorPosHLayout, *mHorPosVLayout, *mHorOriLayout, *mHorColResLayout, *mHorColConLayout, *mHorColCarLayout;
	QLabel *mTxtCapa, *mTxtKg, *mTxtNiveau, *mTxtNiv, *mTxtLargeur, *mTxtM, *mTxtHauteur, *mTxtM2, *mTxtPosH, *mTxtM3, *mTxtPosV, *mTxtM4, *mTxtOri, *mTxtDeg, *mTxtColRes, *mTxtColCon, *mTxtColCar;
	QIntValueBox *mValeurCapacite;
	QRealValueBox *mValeurNiveau, *mValeurLargeur, *mValeurHauteur, *mValeurPosH, *mValeurPosV, *mValeurOri;
	QColorBox *mValeurColRes, *mValeurColCon, *mValeurColCar;

	//Pour la simulation
	QWidget *mTabSimulation;
	QVBoxLayout *mVerticalSimul;
	QPushButton *mBoutonDemarrer, *mBoutonArreter, *mBoutonPas, *mBoutonReinit;
	QCheckBox *mBoxDemAuto, *mBoxCentrer;

	//Pour la création de navettes
	QGroupBox *mGroupeWidget;
	QVBoxLayout *mVerticalNavette;
	QComboBox *mComboNavette;
	QHBoxLayout *mHorTailleLayout;
	QLabel *mTxtTaille, *mTxtPixel;
	QRealValueBox *mValeurTaille;
	QPushButton *mBoutonCreer;

	//Pour le database
	QGroupBox *mGroupBoxInsert, *mGroupBoxDelete, *mGroupBoxUpdate, *mGroupBoxSelect;
	QHBoxLayout *mHLayoutNom, *mHLayoutBouton1, *mHLayoutBouton2, *mHLayoutBouton3, *mHLayoutBouton4;
	QVBoxLayout *mLayoutInsert, *mLayoutDelete, *mLayoutUpdate, *mLayoutSelect, *mLayoutVerticalDatabase;
	QWidget *mWidgetDatabase;
	QPushButton *mBtnInsert, *mBtnDelete, *mBtnUpdate, *mBtnSelect;
	QLabel *mLabelNomShuttle;
	QLineEdit *mEditNomShuttle;
	QShuttlePostgresqlDatabase *mDatabase;
	QComboBox *mListeVaisseaux;

	private slots:
		//Pour simulation
		void demarrerSimul();
		void arreterSimul();
		void unPas();
		void reinitialiserVaisseau();
		void changerCentrage();

		//Pour réservoir
		void modifierReservoir();
		void modifNiveau();
		void modifPosition();
		void modifCapacite();
		void modifOrientation();

		//Pour le shuttle
		void genererVaisseauBase(qreal s);
		void generate_Horizon_6t_k(qreal s);
		void modifForme();
		void creerVaisseau();

		//Pour les thrusters
		void creerThrusters(int nombre);
		void nombrePropulseurs();
		void afficherPropulseur();
		void modifPropulseur();
		void modifPropulseurX();
		void modifPropulseurY();
		void modifFormeProp();
		void modifTouche();

		//Pour le database
		void databaseInsert();
		void databaseUpdate();
		void databaseSelect();
		void databaseDelete();
};
