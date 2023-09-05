#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QProcess>
#include <QTime>
#include <QDialog>
#include <QMainWindow>
#include <QMdiArea>
#include <QStandardPaths>
#include <QDir>
#include <QTreeWidgetItem>
#include <QLinkedList>

#include "csegydlg.h"
#include "cSegYReadWrite.h"
#include "cGLWindow.h"
#include "cColorMap.h"


struct IndxFileSort {
    //Struct, used to return the index of currently selected File and / or Sort item in the treeWidget.
    int64_t indxFile = -1;
    int64_t indxSortOrder = -1;
};

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
    bool AddDataTopNode(std::shared_ptr<cSeisData> pdt);

private slots:
    void on_actionOpen_File_triggered();
    bool on_HeaderTblShow();
    bool on_uiParameters();
    bool SetDispParameters();
    bool on_DisplayGathers();
    bool on_BtnApplyNorm();
    bool on_BtnApplyFlipElev();
    bool on_removeSortGather();
    std::shared_ptr<cSeisData> GetcSeisDataObj(QString name);
    bool on_SelHdrs();
    void treeWidgetMenu(const QPoint& pos);
    void on_actionClose_Project_triggered();
    void Display3DDownHoleMultiRcvSingleSrc(std::shared_ptr<cSeisData> pDt);
    void Display3DMigrated(std::shared_ptr<cSeisData> sDt);
    void on_actionContact_Info_triggered();
    void on_actionVersion_triggered();
    void on_actionLicense_triggered();

private:
    Ui::MainWindow* ui;
    QDir m_wd;

    std::shared_ptr<cColorMap> m_defaultDivergingColorMap = std::make_shared<cColorMap>();
    std::shared_ptr<cColorMap> m_defaultLinearColorMap = std::make_shared<cColorMap>();
    bool LoadDownHoleMultiRcvSingleSrc(std::shared_ptr<cSegYReadWrite> pSgyRedWrt, std::shared_ptr<cSeisData> pdt, QTreeWidgetItem* itmFileName, QTreeWidgetItem* srcItem, QTreeWidgetItem* rcvItem);
    bool LoadMigrated(std::shared_ptr<cSegYReadWrite> pSgyRedWrt, std::shared_ptr<cSeisData> pdt, QTreeWidgetItem* itmFileName, QTreeWidgetItem* rcvItem);
    QTreeWidgetItem* AddToTreeWidget(std::shared_ptr<cSeisData> pDt, QString& dataDesc, quint32 dataIndx);
    virtual void closeEvent(QCloseEvent* event); //https://stackoverflow.com/questions/25454648/qmainwindow-close-signal-not-emitted
    void onDisplay3D(int m_vDataIndx);
    bool on_FileRemove(int m_vDataIndx);
    void onDispNormChanged(double val);
    void onFlipElevationAxis(bool state);
    bool GenerateColorMaps();
    QList<std::shared_ptr<cSeisData>> m_vData;//Stores pointers to all loaded data sets
    std::shared_ptr<cGLWindow> m_GLWnd;  //3D OpenGL Window
    IndxFileSort GetCurrentIndxFileSort();
    void createTopNodeMenuActions();
    void createChildNodeMenuActions();
    bool CreateSubWindow();
    QTreeWidgetItem* GetTopNode(QTreeWidgetItem* sel);
    int GetTopNodeIndex(QTreeWidgetItem* sel);
    QAction* sortAct;
    QAction* removeFileAct;
    QAction* hdrTableAct;
    QAction* uiDisplay;
    QAction* displayAct;
    QAction* removeSortAct;
    QAction* selectHdrsAct;
    QMenu* TopNodeMenu;
    QMenu* ChildNodeMenu;
    QList<std::shared_ptr <cSeisHdrTableView>> vUObjs; //Stores objects for the header view.
};

#endif // MAINWINDOW_H
