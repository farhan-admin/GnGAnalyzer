#include <future>
#include <memory>
#include <math.h>

#include <QMdiSubWindow>
#include <QDesktopWidget>
#include <QtGlobal>
#include <QtConcurrent/QtConcurrent>
#include <QVector3D>
#include <QCheckBox>

#include "cSeisData.h"
#include "cSegYReadWrite.h"
#include "cColorBar.h"
#include "infodialog.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"


// Column numbers for displaying data in 
// the tree view
constexpr quint8 FILE_NAME_COL = 0;
constexpr quint8 SECTION_HDRS_COL = 0;
constexpr quint8 CHILD_INFO_LABL = 0;
constexpr quint8 CHILD_INFO_AND_CNTRL_COL = 1;
constexpr quint8 APPLY_BTN_CNTRL_COL = 2;


MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
    {
        //this->setWindowTitle("GnG Analyzer v0.1 - Created by Farhan Naseer");

        ui->setupUi(this);
        ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        
        //SetUp TreeView Menus();
        this->ui->treeWdgtHdrs->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this->ui->treeWdgtHdrs, &QTreeWidget::customContextMenuRequested, this, &MainWindow::treeWidgetMenu);
        
        TopNodeMenu = new QMenu(this->ui->treeWdgtHdrs);
        createTopNodeMenuActions();

 /*       ChildNodeMenu = new QMenu(this->ui->treeWdgtHdrs);
        createChildNodeMenuActions();*/
        CreateSubWindow();

        //Generating and Storing default color maps;
        GenerateColorMaps();

        SetDispParameters();

        //    wd = QStandardPaths::displayName(QStandardPaths::DocumentsLocation); //Setting up a working directory
        //    if(wd.exists())
        //        cErrWarnInfo::EWI(3,"Setting working directry to "+ wd.absolutePath().toStdString());
        //    else
        //        cErrWarnInfo::EWI(2,"Please select a working directry.");
    }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createTopNodeMenuActions()
{
    //sortAct = new QAction(tr("&Sort"), this);
    //sortAct->setStatusTip(tr("Sort data, using the headers"));
    //sortAct->setToolTip(tr("Sort data, using the headers"));
    //TopNodeMenu->addAction(sortAct);
    //connect(sortAct, &QAction::triggered, this, &MainWindow::on_Sort);

    uiDisplay = new QAction(tr("&Display"), this);
    uiDisplay->setStatusTip(tr("Display in the 3D window"));
    uiDisplay->setToolTip(tr("Display in the 3D window"));
    TopNodeMenu->addAction(uiDisplay);
    connect(uiDisplay, &QAction::triggered, this, &MainWindow::onDisplay3D);

    removeFileAct = new QAction(tr("&Remove Display"), this);
    removeFileAct->setStatusTip(tr("Remove data from the 3D window"));
    removeFileAct->setToolTip(tr("Remove data from the 3D window"));
    TopNodeMenu->addAction(removeFileAct);
    connect(removeFileAct, &QAction::triggered, this, &MainWindow::on_FileRemove);

    TopNodeMenu->addSeparator();

    hdrTableAct = new QAction(tr("&Header Table"));
    hdrTableAct->setStatusTip(tr("Show the header table"));
    hdrTableAct->setToolTip(tr("Show the header table"));
    TopNodeMenu->addAction(hdrTableAct);
    connect(hdrTableAct, &QAction::triggered, this, &MainWindow::on_HeaderTblShow);

}

void MainWindow::createChildNodeMenuActions() {

    displayAct = new QAction(tr("&Display Gather"), this);
    displayAct->setStatusTip(tr("Display Sorted Gathers"));
    displayAct->setToolTip(tr("Display Sorted Gathers"));
    ChildNodeMenu->addAction(displayAct);
    connect(displayAct, &QAction::triggered, this, &MainWindow::on_DisplayGathers);

    selectHdrsAct = new QAction(tr("&Select Headers"));
    selectHdrsAct->setStatusTip(tr("Select X Y Z, Headers"));
    selectHdrsAct->setToolTip(tr("Select X Y Z, Headers"));
    ChildNodeMenu->addAction(selectHdrsAct);
    connect(selectHdrsAct, &QAction::triggered, this, &MainWindow::on_SelHdrs);

    ChildNodeMenu->addSeparator();

    removeSortAct = new QAction(tr("&Remove Sort"), this);
    removeSortAct->setStatusTip(tr("Remove Sorting Defination"));
    removeSortAct->setToolTip(tr("Remove Sorting Defination"));
    ChildNodeMenu->addAction(removeSortAct);
    connect(removeSortAct, &QAction::triggered, this, &MainWindow::on_removeSortGather);
}

bool MainWindow::CreateSubWindow()
{
    m_GLWnd = std::make_shared<cGLWindow>();

    QMdiSubWindow* sub_GLWnd = this->ui->mdiArea->addSubWindow(QWidget::createWindowContainer(m_GLWnd.get()));
    sub_GLWnd->setWindowFlag(Qt::FramelessWindowHint, true);
    sub_GLWnd->setWindowState(Qt::WindowMaximized);

    return false;
}

//Traverses the tree upwards until it finds the node without a parent
QTreeWidgetItem* MainWindow::GetTopNode(QTreeWidgetItem* sel)
{
    while (sel->parent() != nullptr)
    {
        sel = sel->parent();
    }
        
    return sel;
}

//Returns m_vData index of the parent object. This index was stored in the third column when item was created during SegY file load.
int MainWindow::GetTopNodeIndex(QTreeWidgetItem* sel)
{
    QTreeWidgetItem* topNode = GetTopNode(sel);

    return topNode->text(2).toInt();

}

//Returns shared_ptr<cSeisData> by looking up the "name" in the m_vData QList
std::shared_ptr<cSeisData> MainWindow::GetcSeisDataObj(QString name)
{
    for (auto i : m_vData)
    {
        if (i->m_DataName == name)
            return i;
    }

    return nullptr;
}

bool MainWindow::GenerateColorMaps()
{
    strctColor minColr(150, 150, 150);
    strctColor zeroColr(75, 75, 75);
    strctColor maxColr(5, 5, 5);
    quint32 numColrs = 256;
    m_defaultDivergingColorMap->GeneDivergingColorMap(maxColr, minColr, zeroColr, numColrs);

    strctColor maxLinearColr(238, 79, 11);
    strctColor minLinearColr(150, 223, 55);
//    strctColor maxLinearColr(240, 0, 0);
//    strctColor minLinearColr(0, 0, 240);
    numColrs = 256;
    m_defaultLinearColorMap->GeneLinearColorMap(maxLinearColr, minLinearColr, numColrs);

    return false;
}

bool MainWindow::on_SelHdrs()
{
    return false;
}

bool MainWindow::on_BtnApplyFlipElev()
{
    return false;
}

bool MainWindow::on_removeSortGather() {
    std::cerr << "Feature not implemented yet" << std::endl;
    return false;
}

bool MainWindow::on_DisplayGathers() {

    IndxFileSort indx = GetCurrentIndxFileSort();

    if ((indx.indxFile < 0) || (indx.indxSortOrder < 0))//Can only display childNodes (Sort Orders), if indxSortOrder or indxFile is invalid we cannot display
        return false;

    std::shared_ptr<cSeisData> pdtf = m_vData[indx.indxFile];
    // SortOrder* so = &pdtf->vecSortGathers[indx.indxSortOrder];


        //ReadGather(pdtf,indx.indxSortOrder,1);

    return true;
}

IndxFileSort MainWindow::GetCurrentIndxFileSort() {

    IndxFileSort indx;

    QTreeWidgetItem* itm = this->ui->treeWdgtHdrs->currentItem()->parent();

    if (!itm)//if itm is NULL then selected item is a Top node, and will only have a valid indxFile, and will NOT have an indxSort
        indx.indxFile = this->ui->treeWdgtHdrs->currentItem()->text(0).toInt() - 1;

    else if (itm) {//if itm is valid then selected item is a child item, and will have a indxSort and an indxFile
        indx.indxFile = itm->text(0).toInt() - 1;
        indx.indxSortOrder = this->ui->treeWdgtHdrs->currentItem()->text(0).toInt() - 1;
    }

    return indx;
}

bool MainWindow::on_FileRemove(int m_vDataIndx) 
{
    //Selecting the Parent node of the currently clicked Apply button
    QTreeWidgetItem* curItem = this->ui->treeWdgtHdrs->currentItem();
    m_vDataIndx = GetTopNodeIndex(curItem);
      
    if (m_vDataIndx < m_vData.size())
    {
        std::shared_ptr<cSeisData> sDt = m_vData[m_vDataIndx];

        for (auto gth : sDt->m_ptrChildGather)
        {
            m_GLWnd->RemoveSeisGath(gth);
        }

        sDt->m_ptrChildGather.clear();
        sDt->m_ptrChildGather.squeeze();

        for (auto hdr : sDt->m_ptrChildHdr)
        {
            m_GLWnd->RemoveSeisHdr(hdr);
        }

        sDt->m_ptrChildHdr.clear();
        sDt->m_ptrChildHdr.squeeze();
    }

    //QModelIndex indx = this->ui->treeWdgtHdrs->currentIndex();
    //qDebug() << "Feature not implemented yet" ;
    return false;

}

bool MainWindow::on_HeaderTblShow()
{
    QModelIndex indx = this->ui->treeWdgtHdrs->currentIndex();

    if (m_vData.size() > indx.row())
    {

        std::shared_ptr<cSeisHdrTableView> hdrTbl(new cSeisHdrTableView(this));
        vUObjs.append(hdrTbl);
        hdrTbl->InitSeisHdrTblView(m_vData[indx.row()]);

        //ToDo Need to enable multi threading on this
        /*Cannot create Threads for headers loading. Need to come back to this later*/
        //QtConcurrent::run([this,vu, indx]() { vu->InitSeisHdrTblView(m_vData[indx]); } ); 
    }

    return true;
}

bool MainWindow::on_uiParameters()
{
    return false;
}

bool MainWindow::SetDispParameters()
{
    return false;    
}

void MainWindow::treeWidgetMenu(const QPoint& pos)
{
    QTreeWidgetItem* nd = this->ui->treeWdgtHdrs->itemAt(pos);

    if (nd && !nd->parent()) {//Show Top Node Menu: Only show this menu if nd is valid and is a top parent node
        nd->setSelected(true);
        TopNodeMenu->exec(this->ui->treeWdgtHdrs->mapToGlobal(pos));
    }

    //else if (nd && nd->parent()) {//Show Child Node Menu: Only show this menu if nd is valid and is a child node
    //    nd->setSelected(true);
    //    ChildNodeMenu->exec(this->ui->treeWdgtHdrs->mapToGlobal(pos));
    //}
}

void MainWindow::on_actionOpen_File_triggered()
{
    try
    {
        std::shared_ptr<cSeisData> pDt = std::make_shared<cSeisData>();
        std::shared_ptr<cSegYReadWrite> pSgyRedWrt = std::make_shared<cSegYReadWrite>();

        CSegYDlg sgDlg(pDt, pSgyRedWrt, this);

        if (sgDlg.exec())
        {
            m_vData.push_back(pDt);

            if (m_vData.size() > 0)
            {
                statusBar()->showMessage("Loading Selected Headers.");

                DataType datatype = pDt->m_dataType;

                switch (datatype)
                {
                    case DataType::AcousticFiberWithHdrs:
                        break;

                    case DataType::AcousticFiberWithoutHdrs:
                        break;

                    case DataType::DownholeRcvSingleSrc:
                    {
                        QString des = "Downhole, Single Offset";
                        pDt->m_ColorMap = m_defaultDivergingColorMap;
                        QTreeWidgetItem* itmFileName = AddToTreeWidget(pDt, des, m_vData.size() - 1);
                        QTreeWidgetItem* dataItem = itmFileName->child(0);
                        QTreeWidgetItem* srcItem = dataItem->child(0);
                        QTreeWidgetItem* rcvItem = dataItem->child(1);
                        LoadDownHoleMultiRcvSingleSrc(pSgyRedWrt, pDt, itmFileName, srcItem, rcvItem);

                        break;
                    }
                    case DataType::DownholeRcvMultiSrc:
                        break;

                    case DataType::SurfaceRcvMultiSrc:
                        break;

                    case DataType::Migrated:
                    {
                        QString des = "Migrated";
                        pDt->m_ColorMap = m_defaultDivergingColorMap;
                        QTreeWidgetItem* itmFileName = AddToTreeWidget(pDt, des, m_vData.size() - 1);
                        QTreeWidgetItem* dataItem = itmFileName->child(0);
                        QTreeWidgetItem* srcItem = dataItem->child(0);
                        QTreeWidgetItem* rcvItem = dataItem->child(1);
                        dataItem->removeChild(srcItem); // This is not needed for Migrated data
                        LoadMigrated(pSgyRedWrt, pDt, itmFileName, rcvItem);

                        break;
                    }

                    case DataType::PassiveMonitoring:
                        break;


                    case DataType::GeoPhysicalProperty:
                    {
                        QString des = "Geophysical Property";
                        pDt->m_ColorMap = m_defaultLinearColorMap;
                        QTreeWidgetItem* itmFileName = AddToTreeWidget(pDt, des, m_vData.size() - 1);
                        QTreeWidgetItem* dataItem = itmFileName->child(0);
                        QTreeWidgetItem* srcItem = dataItem->child(0);
                        QTreeWidgetItem* rcvItem = dataItem->child(1);
                        dataItem->removeChild(srcItem); // This is not needed for Property data
                        LoadMigrated(pSgyRedWrt, pDt, itmFileName, rcvItem);

                        break;
                    }
                } // switch
            }// if
                    

               
        }
    }//try

    catch (const exception& e)
    {
        cErrWarnInfo::EWI(ewiERROR, e.what());
    }
}

bool MainWindow::LoadDownHoleMultiRcvSingleSrc(std::shared_ptr<cSegYReadWrite> pSgyRedWrt, std::shared_ptr<cSeisData> pdt, QTreeWidgetItem* itmFileName, QTreeWidgetItem* srcItem, QTreeWidgetItem* rcvItem)
{
    QtConcurrent::run([pSgyRedWrt, pdt, itmFileName, this, srcItem, rcvItem]()
        {
            quint64 frmTrc = 0;
            quint64 toTrc = 0;
            if (pSgyRedWrt->ReadTrcHdr(pdt, pdt->m_SgyFullFilePath, frmTrc, toTrc))
            {
                pdt->FindUniqueLoc("XsrcCoord", "YsrcCoord", "SurfElevAtSrc", pdt->m_UniqSrcs);
                pdt->FindUniqueLoc("XrcvGrpCoord", "YrcvGrpCoord", "RcvGrpElev", pdt->m_UniqRcvs);

                pdt->SetRcvLocAvg(pdt->m_UniqRcvs);
                pdt->SetSrcLocAvg(pdt->m_UniqSrcs);

                pdt->UpdateMinMax();

                srcItem->setText(CHILD_INFO_AND_CNTRL_COL, QString::number(pdt->m_UniqSrcs.size()));
                rcvItem->setText(CHILD_INFO_AND_CNTRL_COL, QString::number(pdt->m_UniqRcvs.size()));

                itmFileName->setText(FILE_NAME_COL, pdt->m_DataName);
                itmFileName->setForeground(FILE_NAME_COL, QBrush(Qt::darkBlue));

                this->ui->treeWdgtHdrs->viewport()->update();

                this->statusBar()->showMessage("Ready");

            }

            else
            {
                QString str = pdt->m_DataName;
                pdt->Clear();
                itmFileName->setText(1, "FAILED Loading : " + str); //Updating File name in the second column
                itmFileName->setForeground(1, QBrush(Qt::red));
                this->statusBar()->showMessage("Failed to load SegY file.");
                return false;
            }
        }
    ); // QtConcurrent::run([pSgyRedWrt, pdt, itm, this]()

    return true;
}

bool MainWindow::LoadMigrated(std::shared_ptr<cSegYReadWrite> pSgyRedWrt, std::shared_ptr<cSeisData> pdt, QTreeWidgetItem* itmFileName, QTreeWidgetItem* rcvItem)
{
    QtConcurrent::run([pSgyRedWrt, pdt, itmFileName, this, rcvItem]()
        {
            quint64 frmTrc = 0;
            quint64 toTrc = 0;
            if (pSgyRedWrt->ReadTrcHdr(pdt, pdt->m_SgyFullFilePath, frmTrc, toTrc))
            {
                pdt->GetLocHdrs("XsrcCoord", "YsrcCoord", "SurfElevAtSrc", pdt->m_UniqSrcs);
                rcvItem->setText(SECTION_HDRS_COL, "Trace Locations");
                rcvItem->setText(CHILD_INFO_AND_CNTRL_COL, QString::number(pdt->m_UniqSrcs.size()));

                pdt->SetRcvLocAvg(pdt->m_UniqSrcs);

                pdt->UpdateMinMax();
                itmFileName->setText(FILE_NAME_COL, pdt->m_DataName);
                itmFileName->setForeground(FILE_NAME_COL, QBrush(Qt::darkBlue));

                this->ui->treeWdgtHdrs->viewport()->update();

                this->statusBar()->showMessage("Ready");
            }
            else
            {
                QString str = pdt->m_DataName;
                pdt->Clear();
                itmFileName->setText(1, "FAILED Loading : " + str); //Adding File name in the second column
                itmFileName->setForeground(1, QBrush(Qt::red));
                this->statusBar()->showMessage("Failed to load SegY file.");
            }
        }
    ); // QtConcurrent::run([pSgyRedWrt, pdt, itm, this]()
    return false;
}

QTreeWidgetItem* MainWindow::AddToTreeWidget(std::shared_ptr<cSeisData> pDt, QString& dataDesc, quint32 dataIndx)
{

    QTreeWidgetItem* itmFileName = new QTreeWidgetItem(this->ui->treeWdgtHdrs);
    itmFileName->setText(0, "Loading : " + pDt->m_DataName); //Adding File name in the first column
    itmFileName->setText(1, dataDesc);
    itmFileName->setText(2, QString::number(dataIndx));
    itmFileName->setForeground(0, QBrush(Qt::blue));

    ///
    ///Adding Data Attribute Nodes and Sub Nodes
    ///
    QTreeWidgetItem* itmAttribData = new QTreeWidgetItem();
    itmAttribData->setText(SECTION_HDRS_COL, "Data");
    itmAttribData->setForeground(SECTION_HDRS_COL, QBrush(Qt::darkBlue));

    QTreeWidgetItem* srcItem = new QTreeWidgetItem();
    srcItem->setText(CHILD_INFO_LABL, "No. of distinct sources:");
    srcItem->setForeground(CHILD_INFO_LABL, QBrush(Qt::darkGray));

    QTreeWidgetItem* rcvItem = new QTreeWidgetItem();
    rcvItem->setText(CHILD_INFO_LABL, "No. of distinct receivers:");
    rcvItem->setForeground(CHILD_INFO_LABL, QBrush(Qt::darkGray));

    itmAttribData->addChild(srcItem);
    itmAttribData->addChild(rcvItem);
    ///
    ///END: Adding Data Attribute Nodes and Sub Nodes
        
    /// <summary>
    /// Adding Parameter Nodes
    /// </summary>
    QTreeWidgetItem* itmAttribParam = new QTreeWidgetItem();
    itmAttribParam->setText(SECTION_HDRS_COL, "Parameters");
    itmAttribParam->setForeground(SECTION_HDRS_COL, QBrush(Qt::darkBlue));

    QTreeWidgetItem* itmDispNorm = new QTreeWidgetItem();
    itmDispNorm->setText(CHILD_INFO_LABL, "Amplitude Normalization:");
    itmDispNorm->setForeground(CHILD_INFO_LABL, QBrush(Qt::darkGray));
    itmAttribParam->addChild(itmDispNorm);

    QDoubleSpinBox* spinBoxDispNorm = new QDoubleSpinBox();
    spinBoxDispNorm->setMinimum(0.01); //The display normalization factor cannot be Zero. Since its used as a divisor
    spinBoxDispNorm->setMaximum(1000000000);
    spinBoxDispNorm->setDecimals(3);
    spinBoxDispNorm->setSingleStep(1);
    spinBoxDispNorm->setValue(pDt->m_dispNorm);
    this->ui->treeWdgtHdrs->setItemWidget(itmDispNorm, CHILD_INFO_AND_CNTRL_COL, spinBoxDispNorm);

    QPushButton* pushBtnApplyNorm = new QPushButton();
    pushBtnApplyNorm->setText("Set");
    this->ui->treeWdgtHdrs->setItemWidget(itmDispNorm, APPLY_BTN_CNTRL_COL, pushBtnApplyNorm);
    QObject::connect(pushBtnApplyNorm, &QPushButton::clicked, this, &MainWindow::on_BtnApplyNorm);

    QTreeWidgetItem* itmFlipElevationAxis = new QTreeWidgetItem();
    itmFlipElevationAxis->setText(CHILD_INFO_LABL, "Flip Elevation Axis:");
    itmFlipElevationAxis->setForeground(CHILD_INFO_LABL, QBrush(Qt::darkGray));
    itmAttribParam->addChild(itmFlipElevationAxis);

    QCheckBox* chkBoxFlipElevationAxis = new QCheckBox();
    this->ui->treeWdgtHdrs->setItemWidget(itmFlipElevationAxis, CHILD_INFO_AND_CNTRL_COL, chkBoxFlipElevationAxis);
        
    /// <summary>
    /// conenct solution taken from 
    /// https://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5
    /// </summary>
    connect(spinBoxDispNorm, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onDispNormChanged);
    connect(chkBoxFlipElevationAxis, &QCheckBox::stateChanged, this, &MainWindow::onFlipElevationAxis);
        
    QTreeWidgetItem* itmColorBar = new QTreeWidgetItem();
    itmColorBar->setText(CHILD_INFO_LABL, "Color Map:");
    itmColorBar->setForeground(CHILD_INFO_LABL, QBrush(Qt::darkGray));
    itmAttribParam->addChild(itmColorBar);

    cColorBar* colorBar = new cColorBar(pDt->m_ColorMap->GetColorBar());
    this->ui->treeWdgtHdrs->setItemWidget(itmColorBar, CHILD_INFO_AND_CNTRL_COL, colorBar);
    /// <summary>
    /// END Adding Parameter Nodes
    /// </summary>

    itmFileName->addChild(itmAttribData);
    itmFileName->addChild(itmAttribParam);

    return itmFileName;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event);

    if (vUObjs.size() > 0) //Closing any open cSeisHdrTableView objects
    {
        for (auto i : vUObjs)
        {
            if (i)
                i->close();
        }
    }
}

bool MainWindow::on_BtnApplyNorm()
{
    //Selecting the Parent node of the currently clicked Apply button
    QTreeWidgetItem* curItem = this->ui->treeWdgtHdrs->currentItem();
    int m_vDataIndx = GetTopNodeIndex(curItem);

    on_FileRemove(m_vDataIndx); //First delete the old data from the GLWindow

    onDisplay3D(m_vDataIndx);

    return false;
}
    
void MainWindow::onDisplay3D(int m_vDataIndx)
{
    //Selecting the Parent node of the currently clicked Apply button
    QTreeWidgetItem* curItem = this->ui->treeWdgtHdrs->currentItem();
    m_vDataIndx = GetTopNodeIndex(curItem);

    std::shared_ptr<cSeisData> pDt = m_vData[m_vDataIndx];

    DataType datatype = pDt->m_dataType;

    switch (datatype)
    {
    case DataType::AcousticFiberWithHdrs:
        break;

    case DataType::AcousticFiberWithoutHdrs:
        break;

    case DataType::DownholeRcvSingleSrc:
        Display3DDownHoleMultiRcvSingleSrc(pDt);
        break;

    case DataType::DownholeRcvMultiSrc:
        break;

    case DataType::SurfaceRcvMultiSrc:
        break;

    case DataType::Migrated:
        Display3DMigrated(pDt);
        break;

    case DataType::PassiveMonitoring:
        break;

    case DataType::GeoPhysicalProperty:
        Display3DMigrated(pDt);
        break;
    }

}

void MainWindow::onDispNormChanged(double val)
{

    QTreeWidgetItem* curItem = this->ui->treeWdgtHdrs->currentItem();
    int m_vDataIndx = GetTopNodeIndex(curItem);
        
    std::shared_ptr<cSeisData> pDt = m_vData[m_vDataIndx];
        
    pDt->m_dispNorm = val;

}

void MainWindow::onFlipElevationAxis(bool state)
{
    QTreeWidgetItem* curItem = this->ui->treeWdgtHdrs->currentItem();
    int m_vDataIndx = GetTopNodeIndex(curItem);
    std::shared_ptr<cSeisData> pDt = m_vData[m_vDataIndx];

    if(state != pDt->m_flipElev) //Only calling redraw if state has actually changed
    {             
        pDt->m_flipElev = state;
        on_FileRemove(m_vDataIndx); //First delete the old file
        onDisplay3D(m_vDataIndx);
    }
}

bool MainWindow::AddDataTopNode(std::shared_ptr<cSeisData> pdt)
{
    QTreeWidgetItem* DataItem = new QTreeWidgetItem(this->ui->treeWdgtHdrs);

    DataItem->setText(0, QString::number(m_vData.size())); //Adding Index Number in first column
    DataItem->setText(1, pdt->m_DataName); //Adding File name in the second column

    return true;
}

void MainWindow::on_actionClose_Project_triggered()
{
    cErrWarnInfo::EWI(ewiINFOR, "Closing Application. From MainWindow::on_actionClose_Project_triggered()");

    QApplication::quit();
}

void MainWindow::Display3DDownHoleMultiRcvSingleSrc(std::shared_ptr<cSeisData> pDt)
{
    if (!m_GLWnd)
        CreateSubWindow();

    ///Load Source and Receivers
    std::shared_ptr<cSeisHdr> rcv = std::make_shared<cSeisHdr>();

    pDt->m_ptrChildHdr.push_back(rcv);

    rcv->m_VcLocXYZ = pDt->GetSrcRcvs(false);
    rcv->m_Color = Qt::blue;
    rcv->m_flipElev = pDt->m_flipElev;

    if (rcv->m_VcLocXYZ)
        m_GLWnd->AddSeisHdrObj(pDt, rcv);

    std::shared_ptr<cSeisHdr> src = std::make_shared<cSeisHdr>();

    pDt->m_ptrChildHdr.push_back(src);

    src->m_VcLocXYZ = pDt->GetSrcRcvs(true);
    src->m_Color = Qt::red;
    src->m_flipElev = pDt->m_flipElev;

    if (src->m_VcLocXYZ)
        m_GLWnd->AddSeisHdrObj(pDt, src);

    //Load Traces

    std::shared_ptr <cSeisGather> gth = std::make_shared<cSeisGather>();

    pDt->m_ptrChildGather.push_back(gth); // Storing reference in the parent.

    gth->m_ptrParent = pDt;

    gth->m_GatherAttributes->m_flipElev = pDt->m_flipElev;

    //gth->m_GatherAttributes->m_ColorMap = m_defaultDivergingColorMap;

    pDt->GetTraceGather(true, 1, gth); //true = Source Gather, false = Receiver Gather. Get the Source gather 1

    gth->m_isTrckVert = false;
    gth->m_smplsTrckScal = QVector3D(0.7f, 0.004f, 0.0f);

    m_GLWnd->AddSeisGath(gth);

    //Load Traces
}

void MainWindow::Display3DMigrated(std::shared_ptr<cSeisData> sDt)
{

    ///Load Source and Receivers

    std::shared_ptr<cSeisHdr> rcv = std::make_shared<cSeisHdr>();
    sDt->m_ptrChildHdr.push_back(rcv);

    rcv->m_VcLocXYZ = sDt->GetSrcRcvs(true);
    rcv->m_flipElev = sDt->m_flipElev;
    rcv->m_Color = Qt::green;
    if (rcv->m_VcLocXYZ)
        m_GLWnd->AddSeisHdrObj(sDt, rcv);

    std::shared_ptr <cSeisGather> gth = std::make_shared<cSeisGather>();

    sDt->m_ptrChildGather.push_back(gth); // Storing reference in the parent.

    gth->m_ptrParent = sDt;

    gth->m_GatherAttributes->m_flipElev = sDt->m_flipElev;

    //gth->m_GatherAttributes->m_ColorMap = m_defaultLinearColorMap;

    quint64 frmTrc = 0;
    quint64 toTrc = sDt->m_NumOfTrcs;
    QString hdrX = "XsrcCoord";
    QString hdrY = "YsrcCoord";
    QString hdrZ = "RcvGrpElev";

    gth->m_isTrckVert = true;
    gth->m_smplsTrckScal = QVector3D(0.001f, 0.7f, 0.0f);


    sDt->GetStackedData(gth, hdrX, hdrY, hdrZ, frmTrc, toTrc); //true = Source Gather, false = Receiver Gather. Get the Source gather 1

    m_GLWnd->AddSeisGath(gth);

}
    
void MainWindow::on_actionContact_Info_triggered()
{

   cInfoDialog infDlg;
   infDlg.setWindowTitle("Contact Information");

   QString text = "The GnG Analyzer was conceived, designed, architected, coded and is maintained by Farhan Naseer (https://www.linkedin.com/in/farhan-naseer-usa/).\n\n"
       "Please feel free to submit bugs, crashes and features request to the link below. Please provided as much information as possible.\n"
       "https://github.com/GnG-Analyzer/Public-Release-v01/issues/new \n\n"
       "Please consider/refer me for your software projects.\n";
       
   infDlg.UpdateText(text);

   infDlg.exec();

}

void MainWindow::on_actionVersion_triggered()
{
    cInfoDialog infDlg;
    infDlg.setWindowTitle("Software Version");

    QString text = "Software version: 0.1\n\n";                   

    infDlg.UpdateText(text);

    infDlg.exec();

}

void MainWindow::on_actionLicense_triggered()
{
    cInfoDialog infDlg;
    infDlg.setWindowTitle("License & Credits");

  QString text =  "Copyright(c) 2020, Farhan Naseer\n\n"

        "Permission is hereby granted, to any person obtaining a copy of this software "
        "to use the Software for data analysis, subject to the following conditions:\n\n"

        "The above copyright notice and this permission notice shall be included in all"
        "copies or substantial portions of the Software.\n\n"

        "THE SOFTWARE IS PROVIDED ""AS IS"", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR "
        "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, "
        "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE "
        "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER "
        "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, "
        "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE "
        "SOFTWARE.\n"

        "\nThe Qt 5.15 source code used in this software is available for download at https://www.qt.io/"
        "\n";
    ;

  infDlg.UpdateText(text);

    infDlg.exec();
}
