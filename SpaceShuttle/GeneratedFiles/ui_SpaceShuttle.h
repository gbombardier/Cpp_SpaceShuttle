/********************************************************************************
** Form generated from reading UI file 'SpaceShuttle.ui'
**
** Created by: Qt User Interface Compiler version 5.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPACESHUTTLE_H
#define UI_SPACESHUTTLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SpaceShuttleClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SpaceShuttleClass)
    {
        if (SpaceShuttleClass->objectName().isEmpty())
            SpaceShuttleClass->setObjectName(QStringLiteral("SpaceShuttleClass"));
        SpaceShuttleClass->resize(600, 400);
        SpaceShuttleClass->setMaximumSize(QSize(1200, 16777215));
        centralWidget = new QWidget(SpaceShuttleClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        SpaceShuttleClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SpaceShuttleClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        SpaceShuttleClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SpaceShuttleClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SpaceShuttleClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(SpaceShuttleClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SpaceShuttleClass->setStatusBar(statusBar);

        retranslateUi(SpaceShuttleClass);

        QMetaObject::connectSlotsByName(SpaceShuttleClass);
    } // setupUi

    void retranslateUi(QMainWindow *SpaceShuttleClass)
    {
        SpaceShuttleClass->setWindowTitle(QApplication::translate("SpaceShuttleClass", "SpaceShuttle", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SpaceShuttleClass: public Ui_SpaceShuttleClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPACESHUTTLE_H
