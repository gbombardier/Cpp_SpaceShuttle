#include "SpaceShuttle.h"

SpaceShuttle::SpaceShuttle(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//Créer le modèle et le controlleur
	mSceneModel = new QSceneModel(mSceneSize);
	mSceneVC = new QSceneViewController;
	mShuttleEditor = new QPolygonEditor(200);
	mPropulseurEditor = new QPolygonEditor(200);
	mSceneVC->setSceneModel(mSceneModel);

	mDatabase = new QShuttlePostgresqlDatabase;
	mDatabase->connect("localhost", 5432, "postgres", "AAAaaa111", "postgres");


	QSize newSize(1400, 900);
	setFixedSize(newSize);

	mSize=mShuttleEditor->outputScale();
	mSize *= 2.0;

	mShuttleWidth = mSize / 12.5;
	mShuttleHeight = mSize/ 2.0;

	mTabInterface = new QTabWidget;

	//Générer le premier vaisseau
	//generate_Horizon_6t_k(mShuttleEditor->outputScale());
	genererVaisseauBase(mShuttleEditor->outputScale());

	//Création des tabs pour l'interface graphique
	creerWidgetDatabase();
	creerWidgetVehicule();
	creerWidgetReservoir();
	creerWidgetPropulseurs();
	creerWidgetSimulation();
	creerWidgetNavette();
	mListeVaisseaux->setCurrentIndex(mListeVaisseaux->findText(mEditNom->text()));
	modifForme();
	

	//Layout horizontal pour l'interface
	mLayoutInterface = new QHBoxLayout;
	mLayoutInterface->addWidget(mSceneVC);
	mLayoutInterface->addWidget(mTabInterface);

	//Placer l'interface dans le central widget
	QWidget *  mainWidget = new QWidget;
	mainWidget->setLayout(mLayoutInterface);
	setCentralWidget(mainWidget); 

	//Connexions
	connect(&mTimer, &QTimer::timeout, this, &SpaceShuttle::move);
	mTimer.start(mSimulSpeed);

	mShuttleFuelTank->fill();
}

//Fonctions pour créer les différentes pages du tabwidget
void SpaceShuttle::creerWidgetDatabase() {
	//Box pour enregistrer
	mLayoutInsert = new QVBoxLayout;
	mHLayoutBouton1 = new QHBoxLayout;
	mBtnInsert = new QPushButton("Enregistrer un nouveau shuttle");
	mBtnInsert->setFixedWidth(200);
	mHLayoutBouton1->addWidget(mBtnInsert);
	mHLayoutBouton1->setAlignment(Qt::AlignHCenter);

	mHLayoutNom = new QHBoxLayout;
	mLabelNomShuttle = new QLabel;
	mLabelNomShuttle->setFixedWidth(30);
	mLabelNomShuttle->setText("Nom:  ");
	mEditNomShuttle = new QLineEdit;
	mEditNomShuttle->setFixedWidth(200);
	mHLayoutNom->addWidget(mLabelNomShuttle);
	mHLayoutNom->addWidget(mEditNomShuttle);

	mLayoutInsert->addLayout(mHLayoutNom);
	mLayoutInsert->addLayout(mHLayoutBouton1);

	connect(mBtnInsert, &QPushButton::clicked, this, &SpaceShuttle::databaseInsert);

	//Box pour Supprimer
	mLayoutDelete = new QVBoxLayout;
	mBtnDelete = new QPushButton("Supprimer le shuttle");
	mBtnDelete->setFixedWidth(200);
	mHLayoutBouton2 = new QHBoxLayout;
	mHLayoutBouton2->addWidget(mBtnDelete);
	mHLayoutBouton2->setAlignment(Qt::AlignHCenter);
	mLayoutDelete->addLayout(mHLayoutBouton2);

	connect(mBtnDelete, &QPushButton::clicked, this, &SpaceShuttle::databaseDelete);

	//Box pour update
	mLayoutUpdate = new QVBoxLayout;
	mBtnUpdate = new QPushButton("Mettre a jour le shuttle");
	mBtnUpdate->setFixedWidth(200);
	mHLayoutBouton3 = new QHBoxLayout;
	mHLayoutBouton3->addWidget(mBtnUpdate);
	mHLayoutBouton3->setAlignment(Qt::AlignHCenter);
	mLayoutUpdate->addLayout(mHLayoutBouton3);

	connect(mBtnUpdate, &QPushButton::clicked, this, &SpaceShuttle::databaseUpdate);

	//Box pour charger
	mLayoutSelect = new QVBoxLayout;
	mBtnSelect = new QPushButton("Charger un shuttle");
	mBtnSelect->setFixedWidth(200);

	mListeVaisseaux = new QComboBox;
	mListeVaisseaux->setFixedWidth(150);
	QStringList listeTemp = mDatabase->availableShuttles();
	mListeVaisseaux->addItems(listeTemp);
	mEditNomShuttle->setText("Le basique 2017");

	mHLayoutBouton4 = new QHBoxLayout;
	mHLayoutBouton4->addWidget(mListeVaisseaux);
	mHLayoutBouton4->addWidget(mBtnSelect);
	mHLayoutBouton4->setAlignment(Qt::AlignHCenter);
	mLayoutSelect->addLayout(mHLayoutBouton4);
	connect(mBtnSelect, &QPushButton::clicked, this, &SpaceShuttle::databaseSelect);


	//Ajouter les groupBox
	mGroupBoxInsert = new QGroupBox("Enregistrer Shuttle");
	mGroupBoxInsert->setLayout(mLayoutInsert);
	mGroupBoxInsert->setFixedHeight(100);
	mGroupBoxInsert->setFixedWidth(400);

	mGroupBoxDelete = new QGroupBox("Supprimer Shuttle");
	mGroupBoxDelete->setLayout(mLayoutDelete);
	mGroupBoxDelete->setFixedHeight(100);
	mGroupBoxDelete->setFixedWidth(400);

	mGroupBoxUpdate = new QGroupBox("Mettre a jour Shuttle");
	mGroupBoxUpdate->setLayout(mLayoutUpdate);
	mGroupBoxUpdate->setFixedHeight(100);
	mGroupBoxUpdate->setFixedWidth(400);

	mGroupBoxSelect = new QGroupBox("Charger Shuttle");
	mGroupBoxSelect->setLayout(mLayoutSelect);
	mGroupBoxSelect->setFixedHeight(100);
	mGroupBoxSelect->setFixedWidth(400);

	mLayoutVerticalDatabase = new QVBoxLayout;
	mLayoutVerticalDatabase->addWidget(mGroupBoxInsert);
	mLayoutVerticalDatabase->addWidget(mGroupBoxDelete);
	mLayoutVerticalDatabase->addWidget(mGroupBoxUpdate);
	mLayoutVerticalDatabase->addWidget(mGroupBoxSelect);

	mWidgetDatabase = new QWidget;
	mWidgetDatabase->setLayout(mLayoutVerticalDatabase);

	mTabInterface->addTab(mWidgetDatabase, tr("Database"));
}

void SpaceShuttle::creerWidgetVehicule() {
	//Widget pour le véhicule
	mLabelNom = new QLabel;
	mLabelNom->setText("Nom: ");
	mLabelNom->setFixedWidth(50);

	mEditNom = new QLineEdit;
	mEditNom->setText("Le basique 2017");

	mLabelMasse = new QLabel;
	mLabelMasse->setText("Masse surfacique");
	mLabelMasse->setFixedWidth(100);

	mScrollBarCarac = new QRealValueBox;
	mScrollBarCarac->setSpinFixedWidth(50);

	mLabelKg = new QLabel("kg");
	mLabelKg->setText("kg");
	mLabelKg->setFixedWidth(50);
	mLabelKg->setFixedHeight(15);
	mLabelKg->setAlignment(Qt::AlignRight);

	mLayoutCaracHaut = new QHBoxLayout;
	mLayoutCaracHaut->addWidget(mLabelNom);
	mLayoutCaracHaut->addWidget(mEditNom);

	mLayoutCaracBas = new QHBoxLayout;
	mLayoutCaracBas->addWidget(mLabelMasse);
	mLayoutCaracBas->addWidget(mScrollBarCarac);
	mLayoutCaracBas->addWidget(mLabelKg);

	mLayoutVerticalCarac = new QVBoxLayout;
	mLayoutVerticalCarac->addLayout(mLayoutCaracHaut);
	mLayoutVerticalCarac->addLayout(mLayoutCaracBas);

	mLayoutShuttle = new QHBoxLayout;
	mLayoutShuttle->addWidget(mShuttleEditor);

	mWidgetCarac = new QWidget;
	mWidgetCarac->setLayout(mLayoutVerticalCarac);

	mGroupBoxCarac = new QGroupBox("Caracteristique");
	mGroupBoxCarac->setLayout(mLayoutVerticalCarac);
	mGroupBoxCarac->setFixedHeight(100);

	mGroupBoxShuttle = new QGroupBox("Forme");
	mGroupBoxShuttle->setLayout(mLayoutShuttle);
	mGroupBoxShuttle->setFixedHeight(350);

	mLayoutVerticalVehicule = new QVBoxLayout;
	mLayoutVerticalVehicule->addWidget(mGroupBoxCarac);
	mLayoutVerticalVehicule->addWidget(mGroupBoxShuttle);

	mWidgetVehicule = new QWidget;
	mWidgetVehicule->setLayout(mLayoutVerticalVehicule);

	mTabInterface->addTab(mWidgetVehicule, tr("Vehicule"));
}

void SpaceShuttle::creerWidgetReservoir() {
	//Widget pour le réservoir
	mTabReservoir = new QWidget;
	mTabReservoir->setFixedHeight(400);
	mVerticalReservoir = new QVBoxLayout;

	mHorCapaLayout = new QHBoxLayout;
	mTxtCapa = new QLabel;
	mTxtCapa->setText("Capacite");
	mTxtCapa->setFixedWidth(100);
	mValeurCapacite = new QIntValueBox;
	mValeurCapacite->setSpinFixedWidth(70);
	mValeurCapacite->setRange(0, mCapaciteMax);
	mValeurCapacite->setValue(mCapacite);
	mTxtKg = new QLabel;
	mTxtKg->setText("kg");
	mTxtKg->setAlignment(Qt::AlignRight);
	mTxtKg->setFixedHeight(15);
	mTxtKg->setFixedWidth(60);
	mHorCapaLayout->addWidget(mTxtCapa);
	mHorCapaLayout->addWidget(mValeurCapacite);
	mHorCapaLayout->addWidget(mTxtKg);
	mVerticalReservoir->addItem(mHorCapaLayout);
	connect(mValeurCapacite, &QIntValueBox::valueChanged, this, &SpaceShuttle::modifierReservoir);

	mHorNiveauLayout = new QHBoxLayout;
	mTxtNiveau = new QLabel;
	mTxtNiveau->setText("Niveau de carburant");
	mTxtNiveau->setFixedWidth(100);
	mValeurNiveau = new QRealValueBox;
	mValeurNiveau->setSpinFixedWidth(70);
	mValeurNiveau->setRange(0, 1.0);
	mValeurNiveau->setValue(1.0);
	mTxtNiv = new QLabel;
	mTxtNiv->setText("-");
	mTxtNiv->setAlignment(Qt::AlignRight);
	mTxtNiv->setFixedHeight(12);
	mTxtNiv->setFixedWidth(60);
	mHorNiveauLayout->addWidget(mTxtNiveau);
	mHorNiveauLayout->addWidget(mValeurNiveau);
	mHorNiveauLayout->addWidget(mTxtNiv);
	mVerticalReservoir->addItem(mHorNiveauLayout);
	connect(mValeurNiveau, &QRealValueBox::valueChanged, this, &SpaceShuttle::modifNiveau);

	mHorLargeurLayout = new QHBoxLayout;
	mTxtLargeur = new QLabel;
	mTxtLargeur->setText("Largeur");
	mTxtLargeur->setFixedWidth(100);
	mValeurLargeur = new QRealValueBox;
	mValeurLargeur->setSpinFixedWidth(70);
	mValeurLargeur->setRange(0, 1.0);
	mValeurLargeur->setValue(1.0);
	mTxtM = new QLabel;
	mTxtM->setText("m");
	mTxtM->setAlignment(Qt::AlignRight);
	mTxtM->setFixedHeight(12);
	mTxtM->setFixedWidth(60);
	mHorLargeurLayout->addWidget(mTxtLargeur);
	mHorLargeurLayout->addWidget(mValeurLargeur);
	mHorLargeurLayout->addWidget(mTxtM);
	mVerticalReservoir->addItem(mHorLargeurLayout);
	connect(mValeurLargeur, &QRealValueBox::valueChanged, this, &SpaceShuttle::modifierReservoir);

	mHorHauteurLayout = new QHBoxLayout;
	mTxtHauteur = new QLabel;
	mTxtHauteur->setText("Hauteur");
	mTxtHauteur->setFixedWidth(100);
	mValeurHauteur = new QRealValueBox;
	mValeurHauteur->setSpinFixedWidth(70);
	mValeurHauteur->setRange(0, 1.0);
	mValeurHauteur->setValue(1.0);
	mTxtM2 = new QLabel;
	mTxtM2->setText("m");
	mTxtM2->setAlignment(Qt::AlignRight);
	mTxtM2->setFixedHeight(12);
	mTxtM2->setFixedWidth(60);
	mHorHauteurLayout->addWidget(mTxtHauteur);
	mHorHauteurLayout->addWidget(mValeurHauteur);
	mHorHauteurLayout->addWidget(mTxtM2);
	mVerticalReservoir->addItem(mHorHauteurLayout);
	connect(mValeurHauteur, &QRealValueBox::valueChanged, this, &SpaceShuttle::modifierReservoir);

	mHorPosHLayout = new QHBoxLayout;
	mTxtPosH = new QLabel;
	mTxtPosH->setText("Position Horizontale");
	mTxtPosH->setFixedWidth(100);
	mValeurPosH = new QRealValueBox;
	mValeurPosH->setSpinFixedWidth(70);
	mValeurPosH->setRange(-50, 50);
	mValeurPosH->setValue(0);
	mTxtM3 = new QLabel;
	mTxtM3->setText("m");
	mTxtM3->setAlignment(Qt::AlignRight);
	mTxtM3->setFixedHeight(12);
	mTxtM3->setFixedWidth(60);
	mHorPosHLayout->addWidget(mTxtPosH);
	mHorPosHLayout->addWidget(mValeurPosH);
	mHorPosHLayout->addWidget(mTxtM3);
	mVerticalReservoir->addItem(mHorPosHLayout);
	connect(mValeurPosH, &QRealValueBox::valueChanged, this, &SpaceShuttle::modifPosition);

	mHorPosVLayout = new QHBoxLayout;
	mTxtPosV = new QLabel;
	mTxtPosV->setText("Position Verticale");
	mTxtPosV->setFixedWidth(100);
	mValeurPosV = new QRealValueBox;
	mValeurPosV->setSpinFixedWidth(70);
	mValeurPosV->setRange(-50, 50);
	mValeurPosV->setValue(0);
	mTxtM4 = new QLabel;
	mTxtM4->setText("m");
	mTxtM4->setAlignment(Qt::AlignRight);
	mTxtM4->setFixedHeight(12);
	mTxtM4->setFixedWidth(60);
	mHorPosVLayout->addWidget(mTxtPosV);
	mHorPosVLayout->addWidget(mValeurPosV);
	mHorPosVLayout->addWidget(mTxtM4);
	mVerticalReservoir->addItem(mHorPosVLayout);
	connect(mValeurPosV, &QRealValueBox::valueChanged, this, &SpaceShuttle::modifPosition);

	mHorOriLayout = new QHBoxLayout;
	mTxtOri = new QLabel;
	mTxtOri->setText("Orientation");
	mTxtOri->setFixedWidth(100);
	mValeurOri = new QRealValueBox;
	mValeurOri->setSpinFixedWidth(70);
	mValeurOri->setRange(-360, 360);
	mValeurOri->setValue(0);
	mTxtDeg = new QLabel;
	mTxtDeg->setText("d");
	mTxtDeg->setAlignment(Qt::AlignRight);
	mTxtDeg->setFixedHeight(12);
	mTxtDeg->setFixedWidth(60);
	mHorOriLayout->addWidget(mTxtOri);
	mHorOriLayout->addWidget(mValeurOri);
	mHorOriLayout->addWidget(mTxtDeg);
	mVerticalReservoir->addItem(mHorOriLayout);
	connect(mValeurOri, &QRealValueBox::valueChanged, this, &SpaceShuttle::modifOrientation);

	mHorColResLayout = new QHBoxLayout;
	mTxtColRes = new QLabel;
	mTxtColRes->setText("Couleur du reservoir");
	mTxtColRes->setFixedWidth(150);
	mValeurColRes = new QColorBox;
	mValeurColRes->setColor(QColor(128, 128, 128));
	mHorColResLayout->addWidget(mTxtColRes);
	mHorColResLayout->addWidget(mValeurColRes);
	mVerticalReservoir->addItem(mHorColResLayout);
	

	mHorColConLayout = new QHBoxLayout;
	mTxtColCon = new QLabel;
	mTxtColCon->setText("Couleur du contour");
	mTxtColCon->setFixedWidth(150);
	mValeurColCon = new QColorBox;
	mValeurColRes->setColor(QColor(0,0,0));
	mHorColConLayout->addWidget(mTxtColCon);
	mHorColConLayout->addWidget(mValeurColCon);
	mVerticalReservoir->addItem(mHorColConLayout);
	

	mHorColCarLayout = new QHBoxLayout;
	mTxtColCar = new QLabel;
	mTxtColCar->setText("Couleur du carburant");
	mTxtColCar->setFixedWidth(150);
	mValeurColCar = new QColorBox;
	mValeurColCar->setColor(QColor(196, 255, 0));
	mHorColCarLayout->addWidget(mTxtColCar);
	mHorColCarLayout->addWidget(mValeurColCar);
	mVerticalReservoir->addItem(mHorColCarLayout);

	connect(mValeurColCon, &QColorBox::colorChanged, this, &SpaceShuttle::modifierReservoir);
	connect(mValeurColRes, &QColorBox::colorChanged, this, &SpaceShuttle::modifierReservoir);
	connect(mValeurColCar, &QColorBox::colorChanged, this, &SpaceShuttle::modifierReservoir);

	mTabReservoir->setLayout(mVerticalReservoir);
	mTabInterface->addTab(mTabReservoir, tr("Reservoir"));
}

void SpaceShuttle::creerWidgetPropulseurs() {
	//Widget pour les propulseurs

	//GroupBoxPropulsion
	mScrollBarNbPropulseur = new  QIntValueBox;
	mScrollBarNbPropulseur->setRange(1, 10);
	mScrollBarNbPropulseur->setSpinFixedWidth(50);
	mScrollBarNbPropulseur->setValue(mNbThrusters);
	

	mLabelNbPropulseurs = new QLabel;
	mLabelNbPropulseurs->setText("Nombre de propulseurs:");
	mLabelNbPropulseurs->setFixedWidth(150);

	mLayoutHPropulsion = new QHBoxLayout;
	mLayoutHPropulsion->addWidget(mLabelNbPropulseurs);
	mLayoutHPropulsion->addWidget(mScrollBarNbPropulseur);

	mGroupBoxPropulsion = new QGroupBox("Propulsion");
	mGroupBoxPropulsion->setLayout(mLayoutHPropulsion);

	connect(mScrollBarNbPropulseur, &QIntValueBox::valueChanged, this, &SpaceShuttle::nombrePropulseurs);

	//GroupBoxPropulseur
	//Layout Selection du Propulseur-----------------------------------
	mLabelSelectionProp = new QLabel("Selection du propulseur");
	mLabelSelectionProp->setFixedWidth(200);

	mComboSelectionProp = new QComboBox;
	//mComboSelectionProp->addItem("Propulseur 0");
	mNbThrusters = mShuttle->thrusters().size();
	for (int i = 0; i < mNbThrusters; ++i) {
		mComboSelectionProp->addItem(QString("Propulseur %1").arg(i));
	}
	mComboSelectionProp->setCurrentIndex(0);

	mLayoutHSelectionProp = new QHBoxLayout;
	mLayoutHSelectionProp->addWidget(mLabelSelectionProp);
	mLayoutHSelectionProp->addWidget(mComboSelectionProp);

	connect(mComboSelectionProp, SIGNAL(currentIndexChanged(int)), this, SLOT(afficherPropulseur()));

	//Layout Dimension--------------------------------------------------
	mLabelDimension = new QLabel("Dimension:");
	mLabelDimension->setFixedWidth(200);
	mLabelDimension->setAlignment(Qt::AlignVCenter);

	mScrollBarDimension = new QRealValueBox;
	mScrollBarDimension->setRange(0.001, 1000.00);
	mScrollBarDimension->setSpinFixedWidth(75);

	mLabelRien = new QLabel("-");
	mLabelRien->setAlignment(Qt::AlignHCenter);
	mLabelRien->setFixedWidth(50);

	mLayoutHDimension = new QHBoxLayout;
	mLayoutHDimension->addWidget(mLabelDimension);
	mLayoutHDimension->addWidget(mScrollBarDimension);
	mLayoutHDimension->addWidget(mLabelRien);

	//Layout Masse Surfacique Propulseurs----------------------------------
	mLabelMasseProp = new QLabel("Masse surfacique:");
	mLabelMasseProp->setFixedWidth(200);
	mLabelMasseProp->setAlignment(Qt::AlignVCenter);

	mScrollBarMasseProp = new QRealValueBox;
	mScrollBarMasseProp->setRange(0.001, 1000.00);
	mScrollBarMasseProp->setSpinFixedWidth(75);

	mLabelKgM2 = new QLabel("kg/m2");
	mLabelKgM2->setAlignment(Qt::AlignHCenter);
	mLabelKgM2->setFixedWidth(50);


	mLayoutHMasseProp = new QHBoxLayout;
	mLayoutHMasseProp->addWidget(mLabelMasseProp);
	mLayoutHMasseProp->addWidget(mScrollBarMasseProp);
	mLayoutHMasseProp->addWidget(mLabelKgM2);

	connect(mScrollBarMasseProp, &QRealValueBox::valueChanged, this, &SpaceShuttle::modifPropulseur);

	//Layout Debit Massique du carburant -----------------------------------
	mLabelDebit = new QLabel("Debit massique du carburant:");
	mLabelDebit->setFixedWidth(200);
	mLabelDebit->setAlignment(Qt::AlignVCenter);

	mScrollBarDebit = new QRealValueBox;
	mScrollBarDebit->setDecimals(1);
	mScrollBarDebit->setRange(1.0, 250.0);
	mScrollBarDebit->setSpinFixedWidth(75);
	mScrollBarDebit->setValue(100.0);
	
	mLabelKg2s = new QLabel("kg2/s");
	mLabelKg2s->setAlignment(Qt::AlignHCenter);
	mLabelKg2s->setFixedWidth(50);


	mLayoutHDebit = new QHBoxLayout;
	mLayoutHDebit->addWidget(mLabelDebit);
	mLayoutHDebit->addWidget(mScrollBarDebit);
	mLayoutHDebit->addWidget(mLabelKg2s);

	connect(mScrollBarDebit, &QRealValueBox::valueChanged, this, &SpaceShuttle::modifPropulseur);

	//Layout Vitesse ejection du carburant-----------------------------------
	mLabelVitesseEjec = new QLabel("Vitesse d'ejection du carburant:");
	mLabelVitesseEjec->setFixedWidth(200);
	mLabelVitesseEjec->setAlignment(Qt::AlignVCenter);

	mScrollBarVitesseEjec = new QRealValueBox;
	mScrollBarVitesseEjec->setRange(1.0, 250.0);
	mScrollBarVitesseEjec->setDecimals(1);
	mScrollBarVitesseEjec->setSpinFixedWidth(75);
	mScrollBarVitesseEjec->setValue(100.0);
	
	mLabelMs = new QLabel("kg2/s");
	mLabelMs->setAlignment(Qt::AlignHCenter);
	mLabelMs->setFixedWidth(50);


	mLayoutHVitesseEjec = new QHBoxLayout;
	mLayoutHVitesseEjec->addWidget(mLabelVitesseEjec);
	mLayoutHVitesseEjec->addWidget(mScrollBarVitesseEjec);
	mLayoutHVitesseEjec->addWidget(mLabelMs);

	connect(mScrollBarVitesseEjec, &QRealValueBox::valueChanged, this, &SpaceShuttle::modifPropulseur);

	//Layout Position Horizontale--------------------------------------------
	mLabelPosH = new QLabel("Position horizontale:");
	mLabelPosH->setFixedWidth(200);
	mLabelPosH->setAlignment(Qt::AlignVCenter);

	mScrollBarPosH = new QRealValueBox;
	mScrollBarPosH->setRange(-1.000, 1.000);
	mScrollBarPosH->setSpinFixedWidth(75);
	

	mLabelMH = new QLabel("m");
	mLabelMH->setAlignment(Qt::AlignHCenter);
	mLabelMH->setFixedWidth(50);


	mLayoutHPosH = new QHBoxLayout;
	mLayoutHPosH->addWidget(mLabelPosH);
	mLayoutHPosH->addWidget(mScrollBarPosH);
	mLayoutHPosH->addWidget(mLabelMH);

	connect(mScrollBarPosH, &QRealValueBox::valueChanged, this, &SpaceShuttle::modifPropulseurX);

	//Layout Position Verticale----------------------------------------------
	mLabelPosV = new QLabel("Position verticale:");
	mLabelPosV->setFixedWidth(200);
	mLabelPosV->setAlignment(Qt::AlignVCenter);

	mScrollBarPosV = new QRealValueBox;
	mScrollBarPosV->setSpinFixedWidth(75);
	mScrollBarPosV->setRange(-1.000, 1.000);
	mScrollBarPosV->setSpinFixedWidth(75);

	mLabelMV = new QLabel("m");
	mLabelMV->setAlignment(Qt::AlignHCenter);
	mLabelMV->setFixedWidth(50);


	mLayoutHPosV = new QHBoxLayout;
	mLayoutHPosV->addWidget(mLabelPosV);
	mLayoutHPosV->addWidget(mScrollBarPosV);
	mLayoutHPosV->addWidget(mLabelMV);

	connect(mScrollBarPosV, &QRealValueBox::valueChanged, this, &SpaceShuttle::modifPropulseurY);

	//Layout Orientation-----------------------------------------------------
	mLabelOri = new QLabel("Orientation:");
	mLabelOri->setFixedWidth(200);
	mLabelOri->setAlignment(Qt::AlignVCenter);

	mScrollBarOri = new QRealValueBox;
	mScrollBarOri->setRange(-360.000, 360.000);
	mScrollBarOri->setSpinFixedWidth(75);

	mLabelDegrees = new QLabel("degrees");
	mLabelDegrees->setAlignment(Qt::AlignHCenter);
	mLabelDegrees->setFixedWidth(50);


	mLayoutHOri = new QHBoxLayout;
	mLayoutHOri->addWidget(mLabelOri);
	mLayoutHOri->addWidget(mScrollBarOri);
	mLayoutHOri->addWidget(mLabelDegrees);

	connect(mScrollBarOri, &QRealValueBox::valueChanged, this, &SpaceShuttle::modifPropulseur);

	//Layout Touche de controle----------------------------------------------
	mLabelToucheCtrl = new QLabel("Touche de controle:");
	mLabelToucheCtrl->setFixedWidth(200);
	mLabelToucheCtrl->setAlignment(Qt::AlignVCenter);

	mEditShortcutProp = new QLineEdit;
	mEditShortcutProp->setPlaceholderText("Press shortcut");

	mLayoutHCtrl = new QHBoxLayout;
	mLayoutHCtrl->addWidget(mLabelToucheCtrl);
	mLayoutHCtrl->addWidget(mEditShortcutProp);

	connect(mEditShortcutProp, &QLineEdit::returnPressed, this, &SpaceShuttle::modifTouche);

	//Ajout des layouts horizontaux Propulseur au layout vertical Propulseur--
	mLayoutVPropulseur = new QVBoxLayout;
	mLayoutVPropulseur->addLayout(mLayoutHSelectionProp);
	mLayoutVPropulseur->addLayout(mLayoutHDimension);
	mLayoutVPropulseur->addLayout(mLayoutHMasseProp);
	mLayoutVPropulseur->addLayout(mLayoutHDebit);
	mLayoutVPropulseur->addLayout(mLayoutHVitesseEjec);
	mLayoutVPropulseur->addLayout(mLayoutHPosH);
	mLayoutVPropulseur->addLayout(mLayoutHPosV);
	mLayoutVPropulseur->addLayout(mLayoutHOri);
	mLayoutVPropulseur->addLayout(mLayoutHCtrl);

	mGroupBoxPropulseur = new QGroupBox("Propulseur");
	mGroupBoxPropulseur->setLayout(mLayoutVPropulseur);

	//GroupBox Forme des propulseurs
	mLayoutVFormePropulseurs = new QVBoxLayout;
	mLayoutVFormePropulseurs->addWidget(mPropulseurEditor);

	mGroupBoxFormeProp = new QGroupBox("Forme des propulseurs");
	mGroupBoxFormeProp->setLayout(mLayoutVFormePropulseurs);

	//Création du layout qui englobe la page
	mLayoutPropulseursFinal = new QVBoxLayout;
	mGroupBoxPropulsion->setFixedHeight(75);
	mLayoutPropulseursFinal->addWidget(mGroupBoxPropulsion);
	mGroupBoxPropulseur->setFixedHeight(370);
	mLayoutPropulseursFinal->addWidget(mGroupBoxPropulseur);
	mGroupBoxFormeProp->setFixedHeight(350);
	mLayoutPropulseursFinal->addWidget(mGroupBoxFormeProp);


	QWidget *tabPropulseurs = new QWidget;
	tabPropulseurs->setLayout(mLayoutPropulseursFinal);
	mTabInterface->addTab(tabPropulseurs, tr("Propulseurs"));

	//Setter l'interface pour avoir les infos du premier propulseur
	mShuttleThruster = mShuttle->thrusters()[0];
	mScrollBarMasseProp->setValue(mShuttleThruster->surfaceMass());
	mScrollBarDebit->setValue(mShuttleThruster->massFlowRate());
	mScrollBarVitesseEjec->setValue(mShuttleThruster->massEjectedSpeed());
	mScrollBarPosH->setValue(mShuttleThruster->linearPosition().x());
	mScrollBarPosV->setValue(mShuttleThruster->linearPosition().y());
	mScrollBarOri->setValue(mShuttleThruster->angularPosition());
	mEditShortcutProp->setText(static_cast<QThrusterKeyboardController*>(mShuttleThruster->controller())->linkedKey().toString());
}

void SpaceShuttle::creerWidgetSimulation() {
	//Widget pour la simulation
	mTabSimulation = new QWidget;
	mTabSimulation->setFixedHeight(300);
	mTabSimulation->setFixedWidth(180);


	mVerticalSimul = new QVBoxLayout;

	mBoutonDemarrer = new QPushButton;
	mBoutonDemarrer->setText("Demarrer");
	mBoutonDemarrer->setCheckable(true);
	mBoutonDemarrer->setEnabled(false);
	connect(mBoutonDemarrer, &QPushButton::clicked, this, &SpaceShuttle::demarrerSimul);
	

	mBoutonArreter = new QPushButton;
	mBoutonArreter->setCheckable(true);
	mBoutonArreter->setText("Arreter");
	connect(mBoutonArreter, &QPushButton::clicked, this, &SpaceShuttle::arreterSimul);

	mBoutonPas = new QPushButton;
	mBoutonPas->setText("1 pas");
	mBoutonPas->setEnabled(false);
	connect(mBoutonPas, &QPushButton::clicked, this, &SpaceShuttle::unPas);

	mBoutonReinit = new QPushButton;
	mBoutonReinit->setText("Reinitialiser");
	connect(mBoutonReinit, &QPushButton::clicked, this, &SpaceShuttle::reinitialiserVaisseau);

	mBoxDemAuto = new QCheckBox("Demarrage automatique");
	mBoxDemAuto->setChecked(true);

	mBoxCentrer = new QCheckBox("Centrer la vue");
	mBoxCentrer->setChecked(true);
	connect(mBoxCentrer, &QCheckBox::stateChanged, this, &SpaceShuttle::changerCentrage);

	mVerticalSimul->addWidget(mBoutonDemarrer);
	mVerticalSimul->addWidget(mBoutonArreter);
	mVerticalSimul->addWidget(mBoutonPas);
	mVerticalSimul->addWidget(mBoutonReinit);
	mVerticalSimul->addWidget(mBoxDemAuto);
	mVerticalSimul->addWidget(mBoxCentrer);

	mTabSimulation->setLayout(mVerticalSimul);
	mTabInterface->addTab(mTabSimulation, tr("Simulation"));
}

void SpaceShuttle::creerWidgetNavette() {
	//Widget pour la navette prédéterminée
	mGroupeWidget = new QGroupBox("Caracteristiques");

	mVerticalNavette = new QVBoxLayout;

	mComboNavette = new QComboBox;
	QStringList list = (QStringList() << "Le basique 2017" << "Horizon_6t_k");
	mComboNavette->addItems(list);
	mVerticalNavette->addWidget(mComboNavette);

	mHorTailleLayout = new QHBoxLayout;

	mTxtTaille = new QLabel;
	mTxtTaille->setText("Taille");
	mTxtTaille->setFixedWidth(60);
	mValeurTaille = new QRealValueBox;
	mValeurTaille->setSpinFixedWidth(50);
	mTxtPixel = new QLabel;
	mTxtPixel->setText("Pixels");
	mTxtPixel->setAlignment(Qt::AlignRight);
	mTxtPixel->setFixedHeight(12);
	mTxtPixel->setFixedWidth(60);

	mHorTailleLayout->addWidget(mTxtTaille);
	mHorTailleLayout->addWidget(mValeurTaille);
	mHorTailleLayout->addWidget(mTxtPixel);

	mVerticalNavette->addItem(mHorTailleLayout);

	mBoutonCreer = new QPushButton;
	mBoutonCreer->setText("Creer");
	connect(mBoutonCreer, &QPushButton::clicked, this, &SpaceShuttle::creerVaisseau);

	mVerticalNavette->addWidget(mBoutonCreer);

	mGroupeWidget->setLayout(mVerticalNavette);
	mGroupeWidget->setFixedWidth(400);
	mGroupeWidget->setFixedHeight(130);

	mTabInterface->addTab(mGroupeWidget, tr("Navette predeterminee"));


}



//Fonction qui permet de centrer ou décentrer la vue sur le vaisseau
void SpaceShuttle::changerCentrage(){
	if(mCentered){
		mCentered = false;
		//mSceneModel->rect();
		//mSceneVC->resetTarget();

	}else{
		mCentered=true;
		mSceneVC->targetShuttle();
	}
}

//Fonctions pour la simulation

//Fonction qui permet de démarrer la simulation
void SpaceShuttle::demarrerSimul() {
	mBoutonDemarrer->setEnabled(false);
	mBoutonArreter->setEnabled(true);
	mBoutonPas->setEnabled(false);
	mTimer.start(mSimulSpeed);
	mSimul = true;

	mSceneVC->setFocus();
}

//Fonction qui permet d'arrêter la simulation
void SpaceShuttle::arreterSimul() {
	mBoutonDemarrer->setEnabled(true);
	mBoutonArreter->setEnabled(false);
	mBoutonPas->setEnabled(true);
	mTimer.stop();
	mSimul = false;

	mSceneVC->setFocus();
}

//Fonction qui permet de faire avancer la simulation pas à pas
void SpaceShuttle::unPas() {
	move();
}

//Fonction qui permet de reinitialiser le vaisseau avec ses paramètres d'origine (changer avec la BD?)
void SpaceShuttle::reinitialiserVaisseau() {
	mShuttle->setLinearPosition(QPointF(mSceneSize.width() / 2.0, mSceneSize.height() / 2.0));
	mShuttle->setLinearAcceleration(QPointF(0.0, 0.0));
	mShuttle->setLinearSpeed(QPointF(0.0, 0.0));
	mShuttle->setAngularPosition(0.0);
	mShuttle->setAngularAcceleration(0);
	mShuttle->setAngularSpeed(0);
	
	if(mSimul){
		mBoutonDemarrer->setEnabled(false);
		mBoutonArreter->setEnabled(true);
		mBoutonPas->setEnabled(false);
	}else{
		mBoutonDemarrer->setEnabled(true);
		mBoutonArreter->setEnabled(false);
		mBoutonPas->setEnabled(true);
	}

	mSceneVC->setFocus();
}

//Fonction qui permet de rafraîchir l'affichage de la simulation
void SpaceShuttle::move() {
	mSceneModel->tic(mSimulSpeed / 1000.0);
	mSceneModel->rect();
	mSceneVC->resetTarget();
	mSceneVC->targetShuttle();
	mSceneVC->update();
}

//Fonctions pour les propulseurs

//Modifie la touche associée à la propulsion
void SpaceShuttle::modifTouche() {
	mShuttleThruster = mShuttle->thrusters()[mComboSelectionProp->currentIndex()];
	static_cast<QThrusterKeyboardController*>(mShuttleThruster->controller())->linkKey(mEditShortcutProp->text());

	mSceneVC->setFocus();
}

//Modifie la forme des propulseurs
void SpaceShuttle::modifFormeProp() {
	for (auto thruster : mShuttle->thrusters()) {
		static_cast<QPolygonalBody*>(thruster->shape())->setPolygon(mPropulseurEditor->polygon());
		thruster->shape()->setBrush(mPropulseurEditor->brush());
		thruster->shape()->setPen(mPropulseurEditor->pen());
	}
}

//Ajoute un nouveau propulseur sur le vaisseau(pas utilisée pour l'instant)
void SpaceShuttle::creerThrusters(int nombre) {

	for (int i; i < nombre; ++i) {
		QBrush thrusterBrush(QColor(128, 128, 128));
		QPen thrusterPen(QPen(Qt::white, 0.5));
		qreal flowRate{ 100.0 };
		qreal ejectSpeed{ 100.0 };
		addNewThruster(mSceneModel, mShuttleFuelTank, mSize / 2.5, flowRate, ejectSpeed, static_cast<QPolygonalBody*>(mShuttle->shape())->polygon()[19], 0.0, thrusterBrush, thrusterPen, QKeySequence("5"));
	}
}

//Gère le nombre de propulseurs sur le vaisseau
void SpaceShuttle::nombrePropulseurs() {
	int nbThrustersCourant = mScrollBarNbPropulseur->value();

	int difference = abs(nbThrustersCourant - mNbThrusters);

	difference;
	if (nbThrustersCourant > mNbThrusters) {
		//ajoute
		for (int i = 0; i < difference; ++i) {
			mComboSelectionProp->addItem(QString("Propulseur %1").arg(mNbThrusters + i));
			addNewThruster(mSceneModel, mShuttleFuelTank, mSize / 7.5, 100, 100, static_cast<QPolygonalBody*>(mShuttle->shape())->polygon()[2], 0, mPropulseurEditor->brush(), mPropulseurEditor->pen(), QKeySequence(mKeySequences[mScrollBarNbPropulseur->value()]));
		}
	}
	else if (mNbThrusters > nbThrustersCourant) {
		//enleve
		/*int nbEnlever = mNbThrusters - nbThrustersCourant;
		for (int i = 0; i < nbEnlever; ++i) {
		mShuttle->removeThruster(mShuttle->thrusters().at(mShuttle->thrusters().size() - 1));
		}*/
		for (int i = 0; i < difference; ++i) {
			mComboSelectionProp->removeItem(mNbThrusters - 1 - i);
			mShuttle->removeThruster(mShuttle->thrusters().at(mShuttle->thrusters().size() - 1));

		}
	}
	mNbThrusters = nbThrustersCourant;

	mSceneVC->setFocus();
}

//Modifie la masse surfacique, le débit de gaz et la position angulaire d'un thruster
void SpaceShuttle::modifPropulseur() {
	mShuttleThruster = mShuttle->thrusters()[mComboSelectionProp->currentIndex()];
	mShuttleThruster->setSurfaceMass(mScrollBarMasseProp->value());
	mShuttleThruster->setThrusterEfficiency(mScrollBarDebit->value(), mScrollBarVitesseEjec->value());
	mShuttleThruster->setAngularPosition(Trigo<>::deg2rad(mScrollBarOri->value()));
}

//Affiche les informations du thruster sélectionné dans le combo box
void SpaceShuttle::afficherPropulseur() {
	mShuttleThruster = mShuttle->thrusters()[mComboSelectionProp->currentIndex()];
	mScrollBarMasseProp->setValueQuiet(mShuttleThruster->surfaceMass());
	mScrollBarDebit->setValueQuiet(mShuttleThruster->massFlowRate());
	mScrollBarVitesseEjec->setValueQuiet(mShuttleThruster->massEjectedSpeed());
	mScrollBarPosH->setValueQuiet(mShuttleThruster->linearPosition().x());
	mScrollBarPosV->setValueQuiet(mShuttleThruster->linearPosition().y());
	mScrollBarOri->setValueQuiet(mShuttleThruster->angularPosition());
	mEditShortcutProp->setText(static_cast<QThrusterKeyboardController*>(mShuttleThruster->controller())->linkedKey().toString());
}

//Modifie la position en X d'un propulseur
void SpaceShuttle::modifPropulseurX() {
	mShuttleThruster = mShuttle->thrusters()[mComboSelectionProp->currentIndex()];
	mShuttleThruster->setLinearPosition(QPointF(mScrollBarPosH->value() * 100, mShuttleThruster->linearPosition().y()));

}

//Modifie la position en Y d'un propulseur
void SpaceShuttle::modifPropulseurY() {
	mShuttleThruster = mShuttle->thrusters()[mComboSelectionProp->currentIndex()];
	mShuttleThruster->setLinearPosition(QPointF(mShuttleThruster->linearPosition().x(), mScrollBarPosV->value() * 100));
}


//Fonctions pour le réservoir

//Modifie le niveau d'essence dans le réservoir
void SpaceShuttle::modifNiveau(){
	//Niveau
	mShuttleFuelTank->fill();
	mShuttleFuelTank->takeFuel((1 - mValeurNiveau->value())*mShuttleFuelTank->capacity());
}

//Modifie la position du réservoir
void SpaceShuttle::modifPosition() {
	mShuttleFuelTank->setLinearPosition(QPointF(mValeurPosH->value(), mValeurPosV->value()));
}


//Modifie l'orientation du réservoir
void SpaceShuttle::modifOrientation() {
	mShuttleFuelTank->setAngularPosition(Trigo<>::deg2rad(mValeurOri->value()));
}

//Modifie la capacité du réservoir
void SpaceShuttle::modifCapacite() {
	//Capacité
	mShuttleFuelTank->setCapacity(mValeurCapacite->value());
	modifNiveau();
}

//Modifie la taille et la couleur du réservoir
void SpaceShuttle::modifierReservoir() {
	//hauteur et largeur
	mShuttleWidth = (mValeurLargeur->value())*mScaleX;
	mShuttleHeight = (mValeurHauteur->value())*mScaleY;
	mShuttleFuelTank->setTankSize(mShuttleWidth, mShuttleHeight);


	mShuttleFuelTank->shape()->setBrush(mValeurColRes->color());
	mShuttleFuelTank->shape()->setPen(QPen(mValeurColCon->color(), 1.0));
	mShuttleFuelTank->setFuelColor(mValeurColCar->color());
}

//Affiche les informations du réservoir
void SpaceShuttle::afficherReservoir() {

	mValeurNiveau->setValueQuiet(mShuttleFuelTank->fuelLevel());
	mValeurPosH->setValueQuiet(mShuttleFuelTank->linearPosition().x());
	mValeurPosV->setValueQuiet(mShuttleFuelTank->linearPosition().y());
	mValeurOri->setValueQuiet(Trigo<>::rad2deg(mShuttleFuelTank->angularPosition()));
	mValeurCapacite->setValueQuiet(mShuttleFuelTank->capacity());
	mValeurLargeur->setValueQuiet(static_cast<QRectangularBody*>(mShuttleFuelTank->shape())->width()/mScaleX);
	mValeurHauteur->setValueQuiet(static_cast<QRectangularBody*>(mShuttleFuelTank->shape())->height()/mScaleY);

}

//Fonctions pour le shuttle

//Génère un vaisseau de base
void SpaceShuttle::genererVaisseauBase(qreal s){
	qreal size{ s };

	// Step 1 - Build shuttle with a polygonal shape (instead of the default circular shape)
	mShuttle = new QShuttle(new QPolygonalBody);

	// Step 1.1 - Assign shuttle to the scene model
	mSceneModel->setOneShuttle(mShuttle);

	// Step 1.2 - Create the polygon for the body
	// Step 1.2.1 - Create unit polygon
	QPolygonF shutteShape;
	shutteShape << QPointF(0.500000, 0.000000)
		<< QPointF(0.000000, -0.500000)
		<< QPointF(0.000000, 0.500000)
		<< QPointF(0.500000, 0.000000);

	mShuttleEditor->setPolygon(shutteShape);
	//shutteShape.translate(-0.5, 0.0);
	size *= 2.0;
	// Step 1.2.2 - Scaling the polygon
	for (auto & point : shutteShape) {
		point *= size;
	}
	// Step 1.2.3 - Assigning the polygon
	static_cast<QPolygonalBody*>(mShuttle->shape())->setPolygon(shutteShape);

	// Step 1.3 - Defining the shape name and color
	mShuttle->setName("Le basique 2017");
	mShuttle->shape()->setBrush(QColor(95, 125, 185));
	mShuttle->shape()->setPen(QPen(QColor(50, 70, 115), 1.0));

	// Step 1.4 - Adding the shuttle controller
	mShuttleController = new QShuttleKeyboardController;
	mShuttle->setController(mShuttleController);

	// Step 1.5 - Centering and orienting the shuttle
	mShuttle->setLinearPosition(QPointF(mSceneSize.width() / 2.0, mSceneSize.height() / 2.0));
	mShuttle->setAngularPosition(0.0);

	// Step 2 - Creating the fuel tank with width and height
	mShuttleFuelTank = new QShuttleFuelTank(size / 12.5, size / 3.0);

	// Step 2.1 - Assigning the brush and pen of the tank
	mShuttleFuelTank->shape()->setBrush(QColor(128, 128, 128));
	mShuttleFuelTank->shape()->setPen(QPen(Qt::black, 1.0));

	// Step 2.2 - Assigning the fuel color
	mShuttleFuelTank->setFuelColor(QColor(196, 255, 0));

	// Step 2.3 - Set capacity and fill it
	mShuttleFuelTank->setCapacity(500000.0);
	mShuttleFuelTank->fill();

	// Step 2.4 - Add the tank to the shuttle
	mShuttle->addFuelTank(mShuttleFuelTank, QPointF(size/3.5-20, 0.0), Trigo<>::deg2rad(90.0));

	// Step 3 - Create and add all thrusters
	QBrush thrusterBrush(QColor(128, 128, 128));
	QPen thrusterPen(QPen(Qt::white, 0.5));
	qreal flowRate{ 100.0 };
	qreal ejectSpeed{ 100.0 };
	addNewThruster(mSceneModel, mShuttleFuelTank, size / 7.5, flowRate, ejectSpeed, static_cast<QPolygonalBody*>(mShuttle->shape())->polygon()[1], 0.0, thrusterBrush, thrusterPen, QKeySequence("2"));
	addNewThruster(mSceneModel, mShuttleFuelTank, size / 7.5, flowRate, ejectSpeed, static_cast<QPolygonalBody*>(mShuttle->shape())->polygon()[2], 0.0, thrusterBrush, thrusterPen, QKeySequence("8"));
	mNbThrusters = 2;

	//Mettre les connect ici
	mShuttleEditor->setPolygon(shutteShape);
	mPropulseurEditor->setOutputScale(30);
	connect(mShuttleEditor, &QPolygonEditor::polygonUpdated, this, &SpaceShuttle::modifForme);
	connect(mPropulseurEditor, &QPolygonEditor::polygonUpdated, this, &SpaceShuttle::modifFormeProp);

	mSceneVC->update();
}

//Génère le horizon 6t k
void SpaceShuttle::generate_Horizon_6t_k(qreal s) {
	qreal size{ s };

	// Step 1 - Build shuttle with a polygonal shape (instead of the default circular shape)
	mShuttle = new QShuttle(new QPolygonalBody);

	// Step 1.1 - Assign shuttle to the scene model
	mSceneModel->setOneShuttle(mShuttle);

	// Step 1.2 - Create the polygon for the body
	// Step 1.2.1 - Create unit polygon
	QPolygonF shutteShape;
	shutteShape << QPointF(1.000000, 0.000000)
		<< QPointF(1.000000, -0.069444)
		<< QPointF(0.930556, -0.069444)
		<< QPointF(0.868056, -0.111111)
		<< QPointF(0.791667, -0.208333)
		<< QPointF(0.791667, -0.277778)
		<< QPointF(0.743056, -0.298611)
		<< QPointF(0.694444, -0.277778)
		<< QPointF(0.694444, -0.208333)
		<< QPointF(0.597222, -0.138889)
		<< QPointF(0.395833, -0.180556)
		<< QPointF(0.354167, -0.298611)
		<< QPointF(0.354167, -0.388889)
		<< QPointF(0.270833, -0.409722)
		<< QPointF(0.180556, -0.388889)
		<< QPointF(0.180556, -0.305556)
		<< QPointF(0.138889, -0.180556)
		<< QPointF(0.069444, -0.138889)
		<< QPointF(0.000000, -0.138889)
		<< QPointF(0.000000, 0.000000)
		<< QPointF(0.000000, 0.138889)
		<< QPointF(0.069444, 0.138889)
		<< QPointF(0.138889, 0.180556)
		<< QPointF(0.180556, 0.305556)
		<< QPointF(0.180556, 0.388889)
		<< QPointF(0.270833, 0.409722)
		<< QPointF(0.354167, 0.388889)
		<< QPointF(0.354167, 0.298611)
		<< QPointF(0.395833, 0.180556)
		<< QPointF(0.597222, 0.138889)
		<< QPointF(0.694444, 0.208333)
		<< QPointF(0.694444, 0.277778)
		<< QPointF(0.743056, 0.298611)
		<< QPointF(0.791667, 0.277778)
		<< QPointF(0.791667, 0.208333)
		<< QPointF(0.868056, 0.111111)
		<< QPointF(0.930556, 0.069444)
		<< QPointF(1.000000, 0.069444)
		<< QPointF(1.000000, 0.000000);
	shutteShape.translate(-0.5, 0.0);
	size *= 2.0;

	// Step 1.2.2 - Scaling the polygon
	for (auto & point : shutteShape) {
		point *= size;
	}
	// Step 1.2.3 - Assigning the polygon
	static_cast<QPolygonalBody*>(mShuttle->shape())->setPolygon(shutteShape);

	// Step 1.3 - Defining the shape name and color
	mShuttle->setName("Horizon 6t K-Shuttle");
	mShuttle->shape()->setBrush(QColor(95, 125, 185));
	mShuttle->shape()->setPen(QPen(QColor(50, 70, 115), 1.0));

	// Step 1.4 - Adding the shuttle controller
	mShuttleController = new QShuttleKeyboardController;
	mShuttle->setController(mShuttleController);

	// Step 1.5 - Centering and orienting the shuttle
	mShuttle->setLinearPosition(QPointF(mSceneSize.width() / 2.0, mSceneSize.height() / 2.0));
	mShuttle->setAngularPosition(0.0);

	// Step 2 - Creating the fuel tank with width and height
	mShuttleFuelTank = new QShuttleFuelTank(size / 12.5, size / 2.0);

	// Step 2.1 - Assigning the brush and pen of the tank
	mShuttleFuelTank->shape()->setBrush(QColor(128, 128, 128));
	mShuttleFuelTank->shape()->setPen(QPen(Qt::black, 1.0));

	// Step 2.2 - Assigning the fuel color
	mShuttleFuelTank->setFuelColor(QColor(196, 255, 0));

	// Step 2.3 - Set capacity and fill it
	mShuttleFuelTank->setCapacity(500000.0);
	mShuttleFuelTank->fill();

	// Step 2.4 - Add the tank to the shuttle
	mShuttle->addFuelTank(mShuttleFuelTank, QPointF(size / 3.0 - 0.5 * size, 0.0), Trigo<>::deg2rad(90.0));

	// Step 3 - Create and add all thrusters
	QBrush thrusterBrush(QColor(128, 128, 128));
	QPen thrusterPen(QPen(Qt::white, 0.5));
	qreal flowRate{ 100.0 };
	qreal ejectSpeed{ 100.0 };
	addNewThruster(mSceneModel, mShuttleFuelTank, size / 2.5, flowRate, ejectSpeed, static_cast<QPolygonalBody*>(mShuttle->shape())->polygon()[19], 0.0, thrusterBrush, thrusterPen, QKeySequence("5"));
	addNewThruster(mSceneModel, mShuttleFuelTank, size / 5.5, flowRate, ejectSpeed, static_cast<QPolygonalBody*>(mShuttle->shape())->polygon()[25], -90.0, thrusterBrush, thrusterPen, QKeySequence("4"));
	addNewThruster(mSceneModel, mShuttleFuelTank, size / 5.5, flowRate, ejectSpeed, static_cast<QPolygonalBody*>(mShuttle->shape())->polygon()[13], 90.0, thrusterBrush, thrusterPen, QKeySequence("6"));
	addNewThruster(mSceneModel, mShuttleFuelTank, size / 5.5, flowRate, ejectSpeed, static_cast<QPolygonalBody*>(mShuttle->shape())->polygon()[0], 180.0, thrusterBrush, thrusterPen, QKeySequence("8"));
	addNewThruster(mSceneModel, mShuttleFuelTank, size / 7.5, flowRate, ejectSpeed, static_cast<QPolygonalBody*>(mShuttle->shape())->polygon()[32], -90.0, thrusterBrush, thrusterPen, QKeySequence("7"));
	addNewThruster(mSceneModel, mShuttleFuelTank, size / 7.5, flowRate, ejectSpeed, static_cast<QPolygonalBody*>(mShuttle->shape())->polygon()[6], 90.0, thrusterBrush, thrusterPen, QKeySequence("9"));
	mNbThrusters = 6;

	mShuttleEditor->setPolygon(shutteShape);
	connect(mShuttleEditor, &QPolygonEditor::polygonUpdated, this, &SpaceShuttle::modifForme);
	connect(mPropulseurEditor, &QPolygonEditor::polygonUpdated, this, &SpaceShuttle::modifFormeProp);
	mSceneVC->update();
}

//Modifie la forme du shuttle en fonction du editor
void SpaceShuttle::modifForme(){
	static_cast<QPolygonalBody*>(mShuttle->shape())->setPolygon(mShuttleEditor->polygon());
	mShuttle->shape()->setBrush(mShuttleEditor->brush());
	mShuttle->shape()->setPen(mShuttleEditor->pen());
}

//Affiche un vaisseau à l'écran
void SpaceShuttle::creerVaisseau() {
	if (mComboNavette->currentText() == "Le basique 2017") {
		genererVaisseauBase(mValeurTaille->value());
	}
	else if (mComboNavette->currentText() == "Horizon_6t_k") {
		generate_Horizon_6t_k(mSize);
	}
}

//Modifie les infos des editors lors du chargement d'un nouveau vaisseau
void SpaceShuttle::modifierEditors(){

	mShuttleEditor->blockSignals(true);
	mPropulseurEditor->blockSignals(true);

	mShuttleEditor->setPolygon(static_cast<QPolygonalBody*>(mShuttle->shape())->polygon());
	mShuttleEditor->setBrush(mShuttle->shape()->brush());
	mShuttleEditor->setPen(mShuttle->shape()->pen());

	mPropulseurEditor->setPolygon(static_cast<QPolygonalBody*>(mShuttle->thrusters()[0]->shape())->polygon());
	mPropulseurEditor->setBrush(mShuttle->thrusters()[0]->shape()->brush());
	mPropulseurEditor->setPen(mShuttle->thrusters()[0]->shape()->pen());

	mShuttleEditor->blockSignals(false);
	mPropulseurEditor->blockSignals(false);
}


//Fonctions pour le database
void SpaceShuttle::databaseInsert() {
	QString txtValide = mEditNomShuttle->text().replace(QString(" (not saved)"), QString(""));
	mShuttle->setName(txtValide);
	mEditNom->setText(txtValide);
	mEditNomShuttle->setText(txtValide);
	mDatabase->insertShuttle(mShuttle);
	QStringList listeTemp = mDatabase->availableShuttles();
	mListeVaisseaux->clear();
	mListeVaisseaux->addItems(listeTemp);
	mListeVaisseaux->setCurrentIndex(mListeVaisseaux->findText(mEditNom->text()));
}

void SpaceShuttle::databaseUpdate() {
	mShuttle->setName(mListeVaisseaux->currentText());
	mDatabase->updateShuttle(mShuttle, mShuttle->name());
	afficherPropulseur();
	modifierEditors();
}

void SpaceShuttle::databaseSelect() {
	mShuttle->setName(mListeVaisseaux->currentText());
	mEditNomShuttle->setText(mListeVaisseaux->currentText());
	mDatabase->retrieveShuttle(mShuttle, mListeVaisseaux->currentText(), *mSceneModel);
	
	mValeurColCar->blockSignals(true);
	mValeurColCar->setColor(mShuttleFuelTank->fuelColor());
	mValeurColCar->blockSignals(false);

	afficherPropulseur();
	afficherReservoir();
	modifierEditors();
}

void SpaceShuttle::databaseDelete() {
	mDatabase->deleteShuttle(mEditNomShuttle->text());

	mShuttle->setName(mEditNomShuttle->text() + " (not saved)");
	mEditNom->setText(mEditNomShuttle->text() + " (not saved)");
	mEditNomShuttle->setText(mEditNomShuttle->text() + " (not saved)");

	QStringList listeTemp = mDatabase->availableShuttles();
	mListeVaisseaux->clear();
	mListeVaisseaux->addItems(listeTemp);
	mListeVaisseaux->addItem(mEditNom->text());
	mListeVaisseaux->setCurrentIndex(mListeVaisseaux->findText(mEditNom->text()));


}

void SpaceShuttle::addNewThruster(QSceneModel * model,
	QShuttleFuelTank * fuelTank,
	qreal thrusterSize,
	qreal massFlowRate,
	qreal massEjectSpeed,
	QPointF const & position,
	qreal orientationDegrees,
	QBrush const & brush,
	QPen const & pen,
	QKeySequence const & key)
{
	// Step 1 - Build thruster with a polygonal shape (instead of the default circular shape) 
	QShuttleThruster * thruster = new QShuttleThruster(new QPolygonalBody);
	// Step 2 - Assign the polygonal shape - in this case, we use an equilateral polygon
	static_cast<QPolygonalBody*>(thruster->shape())->setPolygon(QPolygonFactory::equilateralPolygon(3, thrusterSize, 0.0));
	// Step 3 - Assign brush and pen to the shape
	thruster->shape()->setBrush(brush);
	thruster->shape()->setPen(pen);
	// Step 4 - Assign model to the thruster
	thruster->setSceneModel(model);
	// Step 5 - Assign the fuel tank
	thruster->linkToFuelTank(fuelTank);
	// Step 6 - Set flow rate and eject speed
	thruster->setThrusterEfficiency(massFlowRate, massEjectSpeed);
	// Step 7 - Create and assign the keyboard controller
	thruster->setController(new QThrusterKeyboardController(key));
	// Step 8 - Assign the thruster to the shuttle
	mShuttle->addThruster(thruster, position, Trigo<>::deg2rad(orientationDegrees));

	mPropulseurEditor->setPolygon(static_cast<QPolygonalBody*>(thruster->shape())->polygon());
}
