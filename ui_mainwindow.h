/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen_File;
    QAction *actionSave_Project;
    QAction *actionClose_Project;
    QAction *actionDisplayStackedSurface;
    QAction *actionDisplayShotGatherDonwhole;
    QAction *actionContact_Info;
    QAction *actionVersion;
    QAction *actionLicense;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QMdiArea *mdiArea;
    QMenuBar *menuBar;
    QMenu *menuOpen;
    QMenu *menuAbout;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget_Nav_2;
    QWidget *dockWidgetContents_4;
    QVBoxLayout *verticalLayout_3;
    QTabWidget *tabWidget_Nav;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QTreeWidget *treeWdgtHdrs;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1119, 771);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setCursor(QCursor(Qt::ArrowCursor));
        MainWindow->setMouseTracking(false);
        actionOpen_File = new QAction(MainWindow);
        actionOpen_File->setObjectName(QString::fromUtf8("actionOpen_File"));
        actionSave_Project = new QAction(MainWindow);
        actionSave_Project->setObjectName(QString::fromUtf8("actionSave_Project"));
        actionSave_Project->setEnabled(false);
        actionClose_Project = new QAction(MainWindow);
        actionClose_Project->setObjectName(QString::fromUtf8("actionClose_Project"));
        actionDisplayStackedSurface = new QAction(MainWindow);
        actionDisplayStackedSurface->setObjectName(QString::fromUtf8("actionDisplayStackedSurface"));
        actionDisplayShotGatherDonwhole = new QAction(MainWindow);
        actionDisplayShotGatherDonwhole->setObjectName(QString::fromUtf8("actionDisplayShotGatherDonwhole"));
        actionContact_Info = new QAction(MainWindow);
        actionContact_Info->setObjectName(QString::fromUtf8("actionContact_Info"));
        actionVersion = new QAction(MainWindow);
        actionVersion->setObjectName(QString::fromUtf8("actionVersion"));
        actionLicense = new QAction(MainWindow);
        actionLicense->setObjectName(QString::fromUtf8("actionLicense"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        mdiArea = new QMdiArea(centralWidget);
        mdiArea->setObjectName(QString::fromUtf8("mdiArea"));

        verticalLayout->addWidget(mdiArea);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1119, 20));
        QFont font;
        font.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
        menuBar->setFont(font);
        menuOpen = new QMenu(menuBar);
        menuOpen->setObjectName(QString::fromUtf8("menuOpen"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockWidget_Nav_2 = new QDockWidget(MainWindow);
        dockWidget_Nav_2->setObjectName(QString::fromUtf8("dockWidget_Nav_2"));
        dockWidget_Nav_2->setMinimumSize(QSize(187, 156));
        dockWidget_Nav_2->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetVerticalTitleBar);
        dockWidget_Nav_2->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QString::fromUtf8("dockWidgetContents_4"));
        verticalLayout_3 = new QVBoxLayout(dockWidgetContents_4);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tabWidget_Nav = new QTabWidget(dockWidgetContents_4);
        tabWidget_Nav->setObjectName(QString::fromUtf8("tabWidget_Nav"));
        tabWidget_Nav->setTabPosition(QTabWidget::West);
        tabWidget_Nav->setElideMode(Qt::ElideLeft);
        tabWidget_Nav->setTabsClosable(false);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        treeWdgtHdrs = new QTreeWidget(tab);
        treeWdgtHdrs->headerItem()->setText(2, QString());
        treeWdgtHdrs->setObjectName(QString::fromUtf8("treeWdgtHdrs"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
        font1.setPointSize(8);
        treeWdgtHdrs->setFont(font1);
        treeWdgtHdrs->setContextMenuPolicy(Qt::DefaultContextMenu);
        treeWdgtHdrs->setFrameShadow(QFrame::Plain);
        treeWdgtHdrs->setEditTriggers(QAbstractItemView::NoEditTriggers);
        treeWdgtHdrs->setProperty("showDropIndicator", QVariant(false));
        treeWdgtHdrs->setAlternatingRowColors(false);
        treeWdgtHdrs->setSelectionBehavior(QAbstractItemView::SelectRows);
        treeWdgtHdrs->setTextElideMode(Qt::ElideRight);
        treeWdgtHdrs->setUniformRowHeights(true);
        treeWdgtHdrs->setAnimated(true);
        treeWdgtHdrs->setAllColumnsShowFocus(true);
        treeWdgtHdrs->setHeaderHidden(false);
        treeWdgtHdrs->setExpandsOnDoubleClick(true);
        treeWdgtHdrs->setColumnCount(3);
        treeWdgtHdrs->header()->setVisible(true);
        treeWdgtHdrs->header()->setCascadingSectionResizes(false);
        treeWdgtHdrs->header()->setStretchLastSection(false);

        verticalLayout_2->addWidget(treeWdgtHdrs);

        tabWidget_Nav->addTab(tab, QString());

        verticalLayout_3->addWidget(tabWidget_Nav);

        dockWidget_Nav_2->setWidget(dockWidgetContents_4);
        MainWindow->addDockWidget(Qt::LeftDockWidgetArea, dockWidget_Nav_2);

        menuBar->addAction(menuOpen->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuOpen->addAction(actionOpen_File);
        menuOpen->addSeparator();
        menuOpen->addAction(actionSave_Project);
        menuOpen->addSeparator();
        menuOpen->addAction(actionClose_Project);
        menuAbout->addAction(actionContact_Info);
        menuAbout->addAction(actionVersion);
        menuAbout->addAction(actionLicense);

        retranslateUi(MainWindow);

        tabWidget_Nav->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "GnG Analyzer v0.1", nullptr));
        actionOpen_File->setText(QCoreApplication::translate("MainWindow", "Open File", nullptr));
        actionSave_Project->setText(QCoreApplication::translate("MainWindow", "Save Project", nullptr));
        actionClose_Project->setText(QCoreApplication::translate("MainWindow", "Close Project", nullptr));
        actionDisplayStackedSurface->setText(QCoreApplication::translate("MainWindow", "Stacked - Surface", nullptr));
        actionDisplayShotGatherDonwhole->setText(QCoreApplication::translate("MainWindow", "Shot Gather - Donwhole ", nullptr));
        actionContact_Info->setText(QCoreApplication::translate("MainWindow", "Contact Info", nullptr));
        actionVersion->setText(QCoreApplication::translate("MainWindow", "Version", nullptr));
        actionLicense->setText(QCoreApplication::translate("MainWindow", "License", nullptr));
        menuOpen->setTitle(QCoreApplication::translate("MainWindow", "Load Data", nullptr));
        menuAbout->setTitle(QCoreApplication::translate("MainWindow", "About", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWdgtHdrs->headerItem();
        ___qtreewidgetitem->setText(1, QCoreApplication::translate("MainWindow", "Values", nullptr));
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("MainWindow", "Name / Description", nullptr));
        tabWidget_Nav->setTabText(tabWidget_Nav->indexOf(tab), QCoreApplication::translate("MainWindow", "Opened Files", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
