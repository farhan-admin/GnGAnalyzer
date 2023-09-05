void MainWindow::on_Sort(){

    try{
        //IndxFileSort indx = GetCurrentIndxFileSort();

        //if(indx.indxFile < 0)//Sorting only runs on the TopNode(file), if indxFile is invalid we cannot sort
        //    return;

        //std::shared_ptr<cSeisData> pdtf = m_vData[indx.indxFile];
        //CSelSortHdrs srthdr(pdtf);

        //if(srthdr.exec()){
        //    QTreeWidgetItem *parentItm = this->ui->treeWdgtHdrs->currentItem();
        //    QTreeWidgetItem *childItm = new QTreeWidgetItem;
        //    childItm->setText(0,QString::number(pdtf->m_vecSortGathers.size()));//Adding Index for the sorted set to third column
        //    childItm->setText(1,QString::fromStdString(pdtf->m_vecSortGathers[pdtf->m_vecSortGathers.size()-1].m_strSortingName));//Getting name from the last added sorted order

        //    if(!parentItm->parent()){//if parent is NULL then its a top node, and we can add childitem to it.
        //        quint32 sortIndx = static_cast<quint32>(pdtf->m_vecSortGathers.size()-1);
        //        auto result = std::async(std::launch::async,&MainWindow::SortThread,this,pdtf,sortIndx);
        //        if (result.get()){
        //            childItm->setText(2,QString::number(pdtf->m_vecSortGathers[pdtf->m_vecSortGathers.size()-1].m_vecGathIndx.size()));//Getting number of gathers
        //            parentItm->addChild(childItm);
        //        }
        //    }
        //}
    }

    catch(const exception& e){
        std::cerr << "EXCEPTION: " << e.what() << std::endl;}
}

//bool MainWindow::SortThread(std::shared_ptr<cSeisData> pdtf, uint8_t orderNum ){
//
//    //This function is executed in a separate thread
//
//    CSort cs;
//    return cs.MapSort(pdtf,orderNum);
//}

bool MainWindow::UpdateTree(quint32 indx, QString info) //, std::shared_ptr<cSeisData> pdt)
{
    //QTreeWidgetItem* itm = this->ui->treeWdgtHdrs->topLevelItem(indx);// ->child(1);
    //    if (itm)
    //    {
    //        //itm->setText(0, QString::number(indx));
    //        itm->setText(1, info);
    //        itm->setForeground(1, QBrush(Qt::black));
    //    }

    
//
//    DataItem->setText(0, QString::number(m_vData.size())); //Adding Index Number in first column
//    DataItem->setText(1, pdt->m_DataName); //Adding File name in the second column
//
//    return true;
//
//
//    //Initializing the tree
//
////    this->ui->treeWdgtHdrs->setColumnCount(2); //Column for Name and Description
//
////    this->ui->treeWdgtHdrs->setHeaderLabels(QStringList() << "Name" << "Description");
////    //AddDataTopNode(QString prmDataName, QString prmDataDesc)
//
//    //AddDataTopNode(td);

    return true;
}



bool MainWindow::ReadTraces(std::shared_ptr<cSeisData> pdtf, quint64 fromTrc, quint64 toTrc)
{
    Q_UNUSED(toTrc);
    Q_UNUSED(fromTrc);
    Q_UNUSED(pdtf);

//    QFile inSegyfile(pdtf->m_FullFilePath);
//    inSegyfile.open(QIODevice::ReadOnly);

//    //SortOrder s = vecSortGathers.at(sortOrderNum);

//    CSegYFunc sgyFunc;
//    quint64 trcLoc = 0;
//    quint64 numOfSamp = pdtf->GetBinHdr("NumSampTrc")->m_val;
//    quint64 numOfBytes = numOfSamp * 4; // "4" is the size of each floating point sample

//    switch(pdtf->GetBinHdr("DataFormat")->m_val){

//    case 1:{ // Reading IBM floating point

//        QByteArray sampBuf;
//        float_t temp;
//        sampBuf.resize(numOfBytes);

//        for(quint64 tn = fromTrc; tn < toTrc; ++tn){

//            trcLoc = pdtf->GetTraceAddrs(tn);
//            inSegyfile.seek(trcLoc);
//            sampBuf = inSegyfile.read(numOfBytes);

//            for(quint64 sn = 0; sn < numOfSamp; ++sn){
//                sgyFunc.ibm2float(&temp,sampBuf.mid(sn,4));
//                std::cout << "Sample number: " << sn << " = " << temp << "\n";
//                sn = sn + 4;
//            }
//        }
//        break;}

//    case 5:{// Reading IEEE flaoting point

//            bool hdrFound;

//            std::shared_ptr<CGather> pGth (new CGather);

//            for(quint64 tn = fromTrc; tn < toTrc; ++tn){

//               CTrace* trc =  new CTrace(numOfSamp);
//               std::cout << "Reading Trace Number: " << tn << std::endl;
//               //trc->m_TrcPosX = float_t(pdtf->GetDoubleHdr("XsrcCoord", hdrFound)->ReadItem(tn));
//               //trc->m_TrcPosY = float_t(pdtf->GetDoubleHdr("YsrcCoord", hdrFound)->ReadItem(tn));
//               //trc->m_TrcStartPosZ = float_t(pdtf->GetDoubleHdr("RcvGrpElev", hdrFound)->ReadItem(tn));
//               trcLoc = pdtf->GetTraceAddrs(tn);
//               inSegyfile.seek(trcLoc);
//               inSegyfile.read(reinterpret_cast<char*>(trc->m_vTrcSamp.data()), numOfBytes);
//               pGth->m_Gather.push_back(trc);
//              }
//                std::cout << "Completed reading " << pGth->m_Gather.size() << " traces." << std::endl;

//               DisplayTraces(pGth);
//    break;}


//    default:
//        std::cerr << "Trace samples format not supported in the current version." << std::endl;
//        return false;
//    }

//    inSegyfile.close();
//   // sgyFunc.ibm2float();
//   return true;
//}

//bool MainWindow::ReadGather(std::shared_ptr<cSeisData> pdtf, int64_t sortOrderNum, quint64 gatherNum){

//    //SortOrder *s = &pdtf->m_vecSortGathers.at(sortOrderNum);
//    //std::cout << s->m_strSortingName <<std::endl;

//    //if(gatherNum==0)
//    //    ReadTraces(pdtf,0,s->m_vecGathIndx[gatherNum]);//Read from first trace, for the first gather

//    //else
//    //    ReadTraces(pdtf, s->m_vecGathIndx[gatherNum-1],s->m_vecGathIndx[gatherNum]);

    return true;
}

//void MainWindow::DisplayTraces(std::shared_ptr<CGather> pGth){

//    Q3DScatter *graph = new Q3DScatter();
//    QWidget *container = QWidget::createWindowContainer(graph);

//    QSize screenSize = graph->screen()->size();
//    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.5));
//    container->setMaximumSize(screenSize);
//    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    container->setFocusPolicy(Qt::StrongFocus);

//    QWidget *widget = new QWidget;
//    QHBoxLayout *hLayout = new QHBoxLayout(widget);
//    QVBoxLayout *vLayout = new QVBoxLayout();
//    hLayout->addWidget(container, 1);
//    hLayout->addLayout(vLayout);

//    QScatterDataArray dataArray;//, dataArray2 ;
//    QScatter3DSeries *series = new QScatter3DSeries;
//    //QScatter3DSeries *series2 = new QScatter3DSeries;

//    for(quint64 trc = 0; trc < pGth->Gather.size(); ++trc){

//        for(quint64 samp = 0; samp < pGth->Gather.size(); ++samp){

//            dataArray << QVector3D( pGth->Gather[trc]->TrcPosX, pGth->Gather[trc]->TrcPosY, pGth->Gather[trc]->vTrcSamp[samp]);
//        }
//    }

//    ScatterDataModifier *modifier = new ScatterDataModifier(graph);

//    series->dataProxy()->addItems(dataArray);
//    //series2->dataProxy()->addItems(dataArray2);

//    modifier->addData(series);
//   // modifier->addData(series2);

//    widget->setWindowTitle(QStringLiteral("3D Display"));
//    this->ui->mdiArea->addSubWindow(widget);
//    widget->showMaximized();//>show();
//}

bool MainWindow::LoadHeavyData()
{
//    CSegYDlg sgDlg;
//    sgDlg.exec();
//    uint paramNumOfTrca = 100;
//    uint paramNumOfHdrs = 20;

//    cSeisData trchdrs(paramNumOfTrca);
//    uint i=1,j;
//    for(i=1;i< paramNumOfHdrs;i++)
//    {
//        trchdrs.InitInt32Hdr(to_string(i));

//        for(j=0;j<trchdrs.GetNumOfTraces();j++)
//            trchdrs.UpdateInt32Hdr(to_string(i),j,(i+j));
//    }

//   // trchdrs.InitDoubleHdr("td1");
//    cErrWarnInfo info(3, to_string(trchdrs.GetNumOfInt32Hdrs()));

//    trchdrs.GetInt32Hdr("21");
    return true;

}

void MainWindow::Create3DSurfaceChildWindow(cSeisData &td)
{
    Q_UNUSED(td);
   // CHdrDisplay *hDispChild = new CHdrDisplay;


//    Q3DSurface *graph = new Q3DSurface();
//    QWidget *container = QWidget::createWindowContainer(graph);

//    container->setMinimumSize(QSize(800, 600));
//    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    container->setFocusPolicy(Qt::StrongFocus);

//    QWidget *widget = new QWidget;
//    QHBoxLayout *hLayout = new QHBoxLayout(widget);
//    QVBoxLayout *vLayoutLeft = new QVBoxLayout();
//    vLayoutLeft->setAlignment(Qt::AlignTop);
//    QVBoxLayout *vLayoutRight = new QVBoxLayout();
//    vLayoutRight->setAlignment(Qt::AlignTop);
//    hLayout->addLayout(vLayoutLeft);
//    hLayout->addWidget(container, 1);
//    hLayout->addLayout(vLayoutRight);

//    QFont font = QFont("Century Gothic", 14);
//    QLabel *label = new QLabel("Show:");
//    font.setBold(true);
//    label->setFont(font);
//    vLayoutLeft->addWidget(label);

//    font.setBold(false);
//    QCheckBox *checkboxOne = new QCheckBox("Oil Rig 1");
//    checkboxOne->setChecked(true);
//    checkboxOne->setFont(font);
//    vLayoutLeft->addWidget(checkboxOne);

//    QCheckBox *checkboxTwo = new QCheckBox("Oil Rig 2");
//    checkboxTwo->setChecked(true);
//    checkboxTwo->setFont(font);
//    vLayoutLeft->addWidget(checkboxTwo);

//    QCheckBox *checkboxThree = new QCheckBox("Refinery");
//    checkboxThree->setFont(font);
//    vLayoutLeft->addWidget(checkboxThree);

//    QLabel *label2 = new QLabel("Visuals:");
//    font.setBold(true);
//    label2->setFont(font);
//    vLayoutRight->addWidget(label2);

//    QCheckBox *checkboxOneRight = new QCheckBox("See-Through");
//    font.setBold(false);
//    checkboxOneRight->setFont(font);
//    vLayoutRight->addWidget(checkboxOneRight);

//    QCheckBox *checkboxTwoRight = new QCheckBox("Highlight Oil");
//    checkboxTwoRight->setFont(font);
//    vLayoutRight->addWidget(checkboxTwoRight);

//    QCheckBox *checkboxThreeRight = new QCheckBox("Shadows");
//    checkboxThreeRight->setFont(font);
//    checkboxThreeRight->setChecked(true);
//    vLayoutRight->addWidget(checkboxThreeRight);

//    QLabel *label3 = new QLabel("Selection:");
//    font.setBold(true);
//    label3->setFont(font);
//    vLayoutRight->addWidget(label3);

//    QLabel *label4 = new QLabel("Nothing");
//    font.setBold(false);
//    font.setPointSize(11);
//    label4->setFont(font);
//    vLayoutRight->addWidget(label4);

//    widget->setWindowTitle(QStringLiteral("Custom Items Example"));

//    CustomItemGraph *modifier = new CustomItemGraph(td,graph, label4);

//    QObject::connect(checkboxOne, &QCheckBox::stateChanged,
//                     modifier, &CustomItemGraph::toggleItemOne);
//    QObject::connect(checkboxTwo, &QCheckBox::stateChanged,
//                     modifier, &CustomItemGraph::toggleItemTwo);
//    QObject::connect(checkboxThree, &QCheckBox::stateChanged,
//                     modifier, &CustomItemGraph::toggleItemThree);

//    QObject::connect(checkboxOneRight, &QCheckBox::stateChanged,
//                     modifier, &CustomItemGraph::toggleSeeThrough);
//    QObject::connect(checkboxTwoRight, &QCheckBox::stateChanged,
//                     modifier, &CustomItemGraph::toggleOilHighlight);
//    QObject::connect(checkboxThreeRight, &QCheckBox::stateChanged,
//                     modifier, &CustomItemGraph::toggleShadows);

//    this->ui->mdiArea->addSubWindow(widget);
//    widget->show();
}

void MainWindow::Create3DScatterChildWindow(std::shared_ptr<cSeisData> pdt)
{
//   // QGuiApplication app(argc, argv);
//    Q3DScatter *graph = new Q3DScatter();
//    QWidget *container = QWidget::createWindowContainer(graph);
//    //! [0] GUI SetUp
//    QSize screenSize = graph->screen()->size();
//    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.5));
//    container->setMaximumSize(screenSize);
//    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    container->setFocusPolicy(Qt::StrongFocus);

//    //! [1]
//    QWidget *widget = new QWidget;
//    QHBoxLayout *hLayout = new QHBoxLayout(widget);
//    QVBoxLayout *vLayout = new QVBoxLayout();
//    hLayout->addWidget(container, 1);
//    hLayout->addLayout(vLayout);
//    //! [1]

//    widget->setWindowTitle(QStringLiteral("3D Display"));

//    //! [4]
//    QComboBox *themeList = new QComboBox(widget);
//    themeList->addItem(QStringLiteral("Qt"));
//    themeList->addItem(QStringLiteral("Primary Colors"));
//    themeList->addItem(QStringLiteral("Digia"));
//    themeList->addItem(QStringLiteral("Stone Moss"));
//    themeList->addItem(QStringLiteral("Army Blue"));
//    themeList->addItem(QStringLiteral("Retro"));
//    themeList->addItem(QStringLiteral("Ebony"));
//    themeList->addItem(QStringLiteral("Isabelle"));
//    themeList->setCurrentIndex(0);

//    QPushButton *labelButton = new QPushButton(widget);
//    labelButton->setText(QStringLiteral("Change label style"));

//    QCheckBox *smoothCheckBox = new QCheckBox(widget);
//    smoothCheckBox->setText(QStringLiteral("Smooth dots"));
//    smoothCheckBox->setChecked(true);

//    QComboBox *itemStyleList = new QComboBox(widget);
//    itemStyleList->addItem(QStringLiteral("Sphere"), int(QAbstract3DSeries::MeshSphere));
//    itemStyleList->addItem(QStringLiteral("Cube"), int(QAbstract3DSeries::MeshCube));
//    itemStyleList->addItem(QStringLiteral("Minimal"), int(QAbstract3DSeries::MeshMinimal));
//    itemStyleList->addItem(QStringLiteral("Point"), int(QAbstract3DSeries::MeshPoint));
//    itemStyleList->setCurrentIndex(0);

//    QPushButton *cameraButton = new QPushButton(widget);
//    cameraButton->setText(QStringLiteral("Change camera preset"));

//    QPushButton *itemCountButton = new QPushButton(widget);
//    itemCountButton->setText(QStringLiteral("Toggle item count"));

//    QCheckBox *backgroundCheckBox = new QCheckBox(widget);
//    backgroundCheckBox->setText(QStringLiteral("Show background"));
//    backgroundCheckBox->setChecked(true);

//    QCheckBox *gridCheckBox = new QCheckBox(widget);
//    gridCheckBox->setText(QStringLiteral("Show grid"));
//    gridCheckBox->setChecked(true);

//    QComboBox *shadowQuality = new QComboBox(widget);
//    shadowQuality->addItem(QStringLiteral("None"));
//    shadowQuality->addItem(QStringLiteral("Low"));
//    shadowQuality->addItem(QStringLiteral("Medium"));
//    shadowQuality->addItem(QStringLiteral("High"));
//    shadowQuality->addItem(QStringLiteral("Low Soft"));
//    shadowQuality->addItem(QStringLiteral("Medium Soft"));
//    shadowQuality->addItem(QStringLiteral("High Soft"));
//    shadowQuality->setCurrentIndex(0);

//    QFontComboBox *fontList = new QFontComboBox(widget);
//    fontList->setCurrentFont(QFont("Arial"));
//    //! [4]

//    //! [5]
//    vLayout->addWidget(labelButton, 0, Qt::AlignTop);
//    vLayout->addWidget(cameraButton, 0, Qt::AlignTop);
//    vLayout->addWidget(itemCountButton, 0, Qt::AlignTop);
//    vLayout->addWidget(backgroundCheckBox);
//    vLayout->addWidget(gridCheckBox);
//    vLayout->addWidget(smoothCheckBox, 0, Qt::AlignTop);
//    vLayout->addWidget(new QLabel(QStringLiteral("Change dot style")));
//    vLayout->addWidget(itemStyleList);
//    vLayout->addWidget(new QLabel(QStringLiteral("Change theme")));
//    vLayout->addWidget(themeList);
//    vLayout->addWidget(new QLabel(QStringLiteral("Adjust shadow quality")));
//    vLayout->addWidget(shadowQuality);
//    vLayout->addWidget(new QLabel(QStringLiteral("Change font")));
//    vLayout->addWidget(fontList, 1, Qt::AlignTop);
//    //! [5]

//    //! [2]
//    ScatterDataModifier *modifier = new ScatterDataModifier(graph);
//    //! [2]

//    //! [6]
//    QObject::connect(cameraButton, &QPushButton::clicked, modifier,
//                     &ScatterDataModifier::changePresetCamera);
//    QObject::connect(labelButton, &QPushButton::clicked, modifier,
//                     &ScatterDataModifier::changeLabelStyle);
//    QObject::connect(itemCountButton, &QPushButton::clicked, modifier,
//                     &ScatterDataModifier::toggleItemCount);

//    QObject::connect(backgroundCheckBox, &QCheckBox::stateChanged, modifier,
//                     &ScatterDataModifier::setBackgroundEnabled);
//    QObject::connect(gridCheckBox, &QCheckBox::stateChanged, modifier,
//                     &ScatterDataModifier::setGridEnabled);
//    QObject::connect(smoothCheckBox, &QCheckBox::stateChanged, modifier,
//                     &ScatterDataModifier::setSmoothDots);

//    QObject::connect(modifier, &ScatterDataModifier::backgroundEnabledChanged,
//                     backgroundCheckBox, &QCheckBox::setChecked);
//    QObject::connect(modifier, &ScatterDataModifier::gridEnabledChanged,
//                     gridCheckBox, &QCheckBox::setChecked);
//    QObject::connect(itemStyleList, SIGNAL(currentIndexChanged(int)), modifier,
//                     SLOT(changeStyle(int)));

//    QObject::connect(themeList, SIGNAL(currentIndexChanged(int)), modifier,
//                     SLOT(changeTheme(int)));

//    QObject::connect(shadowQuality, SIGNAL(currentIndexChanged(int)), modifier,
//                     SLOT(changeShadowQuality(int)));

//    QObject::connect(modifier, &ScatterDataModifier::shadowQualityChanged, shadowQuality,
//                     &QComboBox::setCurrentIndex);
//    QObject::connect(graph, &Q3DScatter::shadowQualityChanged, modifier,
//                     &ScatterDataModifier::shadowQualityUpdatedByVisual);

//    QObject::connect(fontList, &QFontComboBox::currentFontChanged, modifier,
//                     &ScatterDataModifier::changeFont);

//    QObject::connect(modifier, &ScatterDataModifier::fontChanged, fontList,
//                     &QFontComboBox::setCurrentFont);
//    //! [6]

//    //! [3]

//    bool exist;
//    uint iter;
//    QScatterDataArray dataArray, dataArray2 ;
//    QScatter3DSeries *series = new QScatter3DSeries;
//    QScatter3DSeries *series2 = new QScatter3DSeries;

//    std::shared_ptr <CDoubleTrcHdr> xSrc = pdt->GetDoubleHdr("XsrcCoord",exist);
//        if(!exist) return;
//    std::shared_ptr <CDoubleTrcHdr> ySrc = pdt->GetDoubleHdr("YsrcCoord",exist);
//        if(!exist) return;
//    std::shared_ptr <CDoubleTrcHdr> zSrc = pdt->GetDoubleHdr("SurfElevAtSrc",exist);
//        if(!exist) return;
//    std::shared_ptr <CDoubleTrcHdr> xRcv = pdt->GetDoubleHdr("XrcvGrpCoord",exist);
//        if(!exist) return;
//    std::shared_ptr <CDoubleTrcHdr> yRcv = pdt->GetDoubleHdr("YrcvGrpCoord",exist);
//        if(!exist) return;
//    std::shared_ptr <CDoubleTrcHdr> zRcv = pdt->GetDoubleHdr("RcvGrpElev",exist);
//        if(!exist) return;

//    for(iter=0;iter<pdt->GetNumOfTraces();++iter)
//    {
//        dataArray << QVector3D( (float(xSrc->ReadItem(iter))), (float(zSrc->ReadItem(iter))), (float(ySrc->ReadItem(iter))));
//        dataArray2 << QVector3D( (float(xRcv->ReadItem(iter))), (float(zRcv->ReadItem(iter))), (float(yRcv->ReadItem(iter))));
//    }

//    cErrWarnInfo::EWI(3,"Header Reading Completed.");

//    series->dataProxy()->addItems(dataArray);
//    series2->dataProxy()->addItems(dataArray2);

//    modifier->addData(series);
//    modifier->addData(series2);

//    this->ui->mdiArea->addSubWindow(widget);
//    widget->showMaximized();//>show();

    // return app.exec();
}