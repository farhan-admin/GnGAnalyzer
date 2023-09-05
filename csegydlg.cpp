#include <QBuffer>
#include <QTime>
#include <QVector>
#include <QThread>

#include "csegydlg.h"
//#include "mainwindow.h"


class MainWindow;

CSegYDlg::CSegYDlg(std::shared_ptr<cSeisData> pSeisData,
    std::shared_ptr<cSegYReadWrite> pSgyRedWrt,
    QWidget* parent) :
    QDialog(parent),
    ui(new Ui::CSegYDlg),
    m_pSeisData(pSeisData),
    m_pSgyRedWrt(pSgyRedWrt)
{

    ui->setupUi(this);
    this->ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Load");
    EnableOK(false);//Disable Load button, since no file is currently selected

    m_IsSegYTemplOK = false;
    m_IsSegYDataOK = false;

    DataTypeComboBox();
    HdrTreeWdgtSetUp();

  }

CSegYDlg::~CSegYDlg()
{
    delete ui;
}

bool CSegYDlg::HdrTreeWdgtSetUp()
{
    int HDR_COL = 0; //Header column number

    /// TEXT Header
    QTreeWidgetItem* itmEbcdcHdrs = new QTreeWidgetItem();
    itmEbcdcHdrs->setText(HDR_COL, "Text Header");
    itmEbcdcHdrs->setForeground(HDR_COL, QBrush(Qt::gray));
    this->ui->treeWdgtSegYHdrs->addTopLevelItem(itmEbcdcHdrs);

    QTreeWidgetItem* itmEbcdcHdrsTxtBox = new QTreeWidgetItem();
    itmEbcdcHdrs->addChild(itmEbcdcHdrsTxtBox);

    this->ui->treeWdgtSegYHdrs->setItemWidget(itmEbcdcHdrsTxtBox,HDR_COL, m_txtBxTextHdr);
    ///END TEXT Header

    QStringList sl;
    sl << "Name" << "Type" << "Value" << "Load" << "Start Byte" << "End Byte" << "Description";

    /// BINARY Header
    QTreeWidgetItem* itmBinHdr = new QTreeWidgetItem();
    itmBinHdr->setText(HDR_COL, "Binary Headers");
    itmBinHdr->setForeground(HDR_COL, QBrush(Qt::gray));
    this->ui->treeWdgtSegYHdrs->addTopLevelItem(itmBinHdr);

    QTreeWidgetItem* itmBinHdrTable = new QTreeWidgetItem();
    itmBinHdr->addChild(itmBinHdrTable);

    this->ui->treeWdgtSegYHdrs->setItemWidget(itmBinHdrTable, HDR_COL, m_tableWdgtBinHdr);

    //Initializing column headers for the binary header table
    m_tableWdgtBinHdr->setRowCount(0);
    m_tableWdgtBinHdr->setColumnCount(sl.count());
    m_tableWdgtBinHdr->setHorizontalHeaderLabels(sl);

    ///END BINARY Header

    /// TRACE Header
    QTreeWidgetItem* itmTrcHdr = new QTreeWidgetItem();
    itmTrcHdr->setText(HDR_COL, "Trace Headers");
    itmTrcHdr->setForeground(HDR_COL, QBrush(Qt::gray));
    this->ui->treeWdgtSegYHdrs->addTopLevelItem(itmTrcHdr);

    QTreeWidgetItem* itmTrcHdrQCSpinBox = new QTreeWidgetItem();
    itmTrcHdr->addChild(itmTrcHdrQCSpinBox);
    m_trcHdrQCSpinBox->setPrefix("Trace Number: ");
    QObject::connect(m_trcHdrQCSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &CSegYDlg::on_spinBoxTrcNum_valueChanged );

    this->ui->treeWdgtSegYHdrs->setItemWidget(itmTrcHdrQCSpinBox, HDR_COL, m_trcHdrQCSpinBox);

    QTreeWidgetItem* itmTrcHdrTable = new QTreeWidgetItem();
    itmTrcHdr->addChild(itmTrcHdrTable);
    this->ui->treeWdgtSegYHdrs->setItemWidget(itmTrcHdrTable, HDR_COL, m_tableWdgtTrcHdr);

    //Initializing column headers for the trace header table
    m_tableWdgtTrcHdr->setRowCount(0);
    m_tableWdgtTrcHdr->setColumnCount(sl.count());
    m_tableWdgtTrcHdr->setHorizontalHeaderLabels(sl);

    ///END TRACE Header

    return false;
}

bool CSegYDlg::DataTypeComboBox()
{

    this->ui->cmbBxDataType->addItems(m_cmBxLst);

    this->ui->cmbBxDataType->setCurrentIndex(0);

    return false;
}

//Main sequence control function - It checks if all the required data is available
bool CSegYDlg::ExecuteSequence(QFile* inSegYFile)
{

    m_pSgyRedWrt->StoreSegYTemplate(m_pSeisData, m_tableWdgtBinHdr, m_tableWdgtTrcHdr);

    m_pSgyRedWrt->ReadEBCDIChdr(m_pSeisData, inSegYFile);
    UpdtEBCDICHdrUI();

    m_pSgyRedWrt->ReadBinaryHeader(m_pSeisData, inSegYFile);

    bool isValid = false;

    isValid = m_pSgyRedWrt->FindNumOfTraces(m_pSeisData, inSegYFile->size());

    if (isValid)
    {
        UpdtBinHdrUI(isValid);

        quint64 frmTrc = 0;
        quint64 toTrc = 100;  //Only loading first 100 traces for preview
        QString file = inSegYFile->fileName();

        m_pSgyRedWrt->ReadTrcHdr(m_pSeisData, file, frmTrc, toTrc); //Only Read 100 traces for SegY tempalte QC.

        m_trcHdrQCSpinBox->setRange(1, m_pSeisData->m_NumOfTrcs);

        on_spinBoxTrcNum_valueChanged(1); //calling the spin button to value for the first trace headers

        EnableOK(true);//Enable Load button, because both required files are available

        return true;
    }

    else
    {
        UpdtBinHdrUI(isValid); //if there was reading error in the binary header change font color to Red;

        return false;
    }
}

void CSegYDlg::on_buttonBox_accepted()
{
    int type = this->ui->cmbBxDataType->currentIndex();

    switch (type)
    {
    case 0:
        m_pSeisData->m_dataType = DataType::Migrated;
        break;

    case 1:
        m_pSeisData->m_dataType = DataType::GeoPhysicalProperty;
        break;

    default:
        m_pSeisData->m_dataType = DataType::Migrated;
        break;
    }
}

void CSegYDlg::on_buttonBox_rejected()
{
    m_pSeisData->Clear(); //Deleting any data that might have been previously loaded
}

///Select SegY file(s). If the SegY template file is already selected read header values from the first file, update the header values in the GUI table.
void CSegYDlg::on_pshBtnSegYBrowse_clicked()
{
    QFileDialog dlg(this);
    dlg.setFileMode(QFileDialog::ExistingFile); //No Multi File Selection
    QString dir = QDir::homePath();
    dlg.setDirectory(dir);

    QStringList flt;
    flt << "SegY files (*.sgy *.segy)"
        << "Any files (*)";
    dlg.setNameFilters(flt);

    if (dlg.exec())
    {
        m_pSeisData->Clear(); //Everytime user choses a new template or SegY file, m_TrcHdrs should be renewed.

        m_sgyFiles = dlg.selectedFiles();

        QFile inSegYFile(m_sgyFiles[0]);
        this->ui->lblSegYFileName->setText(m_sgyFiles[0]);
        m_pSeisData->m_SgyFullFilePath = m_sgyFiles[0];
        QFileInfo fi(inSegYFile);
        m_pSeisData->m_DataName = fi.fileName();

        m_IsSegYDataOK = true;

        if (m_IsSegYTemplOK)
            ExecuteSequence(&inSegYFile);

    }
    else
        m_IsSegYDataOK = false;

}

///Load SegY Template (*.sgyt) file into the GUI table
void CSegYDlg::on_pshBtnTmplBrowse_clicked()
{

    QFileDialog dlg(this);
    dlg.setFileMode(QFileDialog::ExistingFile); //No Multi File Selection
    QString dir = QDir::homePath();
    dlg.setDirectory(dir);

    QStringList flt;
    flt << "SegY Template (*.sgyt)"
        << "Any files (*)";
    dlg.setNameFilters(flt);

    if (dlg.exec())
    {
        m_pSeisData->Clear(); //Everytime user choses a new template or SegY file, m_TrcHdrs should be renewed.

        m_sgyTemplFile = dlg.selectedFiles();

        QFileInfo finfo(m_sgyTemplFile[0]);
        m_pSeisData->m_SgyTmplFilePath = finfo.absolutePath();

        if ((finfo.exists()) && (finfo.suffix() == "sgyt"))
        {
            QFile inTemfile(m_sgyTemplFile[0]);
            inTemfile.open(QIODevice::ReadOnly);
            QTextStream inStrm(&inTemfile);
            QString s;
            QStringList sl;
            quint32 RowBinHdr = 0;
            quint32 RowTrcHdr = 0;

            this->ui->lblTemplateFileName->setText(finfo.absoluteFilePath());
            m_pSeisData->m_SgyTmplFilePath = finfo.absoluteFilePath();

            m_tableWdgtBinHdr->setRowCount(0);
            m_tableWdgtTrcHdr->setRowCount(0);

            while (true)
            {
                sl.clear();//Clean the QStringList
                s = inStrm.readLine(0);

                if (s.isNull()) break;//exit the loop if file ended

                //Extracting the Table Headers
                if (s.indexOf("##", 0, Qt::CaseSensitive) == 0)
                {
                    QStringList sl;
                    sl = ExtractTableHeaders(s);

                    m_tableWdgtBinHdr->setRowCount(0);
                    m_tableWdgtTrcHdr->setRowCount(0);
                    m_tableWdgtBinHdr->setColumnCount(sl.count());
                    m_tableWdgtBinHdr->setHorizontalHeaderLabels(sl);
                    m_tableWdgtTrcHdr->setColumnCount(sl.count());
                    m_tableWdgtTrcHdr->setHorizontalHeaderLabels(sl);
                }

                //Only Reading the non-comments lines
                else if (s.indexOf("//", 0, Qt::CaseSensitive) == -1)
                {
                    if (!TextParser(s, &sl)) break; //exit the loop if TextParser runs into trouble

                    //Filling the binary header table in the GUI
                    if (sl.at(0).trimmed() == "bh")
                    {
                        LoadSegYTmplaToWgt(m_tableWdgtBinHdr, RowBinHdr, sl);
                        ++RowBinHdr;
                    }

                    //Filling the trace header table in the GUI
                    if (sl.at(0).trimmed() == "th")
                    {
                        LoadSegYTmplaToWgt(m_tableWdgtTrcHdr, RowTrcHdr, sl);
                        //m_pSeisData->SegYTemplate.push_back(*HdrTmpla);

                        ++RowTrcHdr;
                    }

                }

            } // while(true)

            m_tableWdgtBinHdr->resizeColumnsToContents();
            m_tableWdgtTrcHdr->resizeColumnsToContents();

            if (m_IsSegYDataOK)
            {
                QFile inSegYFile(m_sgyFiles[0]);
                ExecuteSequence(&inSegYFile);
            }

            m_IsSegYTemplOK = true;

        } // if((finfo.exists()) && (finfo.suffix()=="sgyt") )

        else
        {
            m_IsSegYTemplOK = false;

            cErrWarnInfo::EWI(ewiERROR, "File selected as Segy template (*.sgyt) does not exist or has incorrect file extension.");
            m_sgyTemplFile.clear();//if file does not exist, file path is made NULL, so sgyTemplFile would not be used anywhere else.
        }

    } //if(dlg.exec())

}

//Function to update the Binary Header and Trace Header Table Widgets in the UI.
void CSegYDlg::LoadSegYTmplaToWgt(QTableWidget* tblWgtHdr, quint32 row, QStringList sl)
{

    QString hn, desc, val, type;
    QString sb, eb, hdrNum, load;
    quint16 lnb;

    hn = sl.at(m_colName + 1).trimmed();

    type = sl.at(m_colType + 1).trimmed();

    val = sl.at(m_colValue + 1).trimmed();

    load = sl.at(m_colLoad + 1).trimmed();

    sb = sl.at(m_colStartByte + 1).trimmed();

    eb = sl.at(m_colEndByte + 1).trimmed();

    desc = sl.at(m_colDesc + 1).trimmed();

    tblWgtHdr->insertRow(tblWgtHdr->rowCount()); //Apending a row
    tblWgtHdr->setItem(row, m_colName, new QTableWidgetItem(hn));//hdrName
    tblWgtHdr->setItem(row, m_colType, new QTableWidgetItem(type));//type
    tblWgtHdr->setItem(row, m_colValue, new QTableWidgetItem(val));//Value
    tblWgtHdr->setItem(row, m_colLoad, new QTableWidgetItem(load));//Data Load or Not
    tblWgtHdr->setItem(row, m_colStartByte, new QTableWidgetItem(sb));//Start Byte
    tblWgtHdr->setItem(row, m_colEndByte, new QTableWidgetItem(eb));//End Byte
    tblWgtHdr->setItem(row, m_colDesc, new QTableWidgetItem(desc));//Description

    lnb = eb.toInt() - sb.toInt() + 1;
    if (lnb > 4) //All data in the headers is suppose to be 2 or 4 byte long
    {
        cErrWarnInfo::EWI(ewiWARNI, "In SegY template row number " + QString::number(row) + ", header byte length is longer than 4 bytes. Start byte " + sb +
            " and End byte " + eb + ".These headers will be ignored.");
    }
}

//Extracting Headers from the loaded template file
QStringList CSegYDlg::ExtractTableHeaders(QString s)
{
    QStringList tmp;
    //colName, colType, colValue, colLoad, colStartByte, colEndByte, colDesc;
    //##;Name;Type;Value;Load;Start Byte;End Byte;Description

    if (TextParser(s, &tmp))
    {
        tmp.removeAt(0);//Removing the '##'
        if ((m_colName = tmp.indexOf(QRegExp("Name"), 0)) == -1) cErrWarnInfo::EWI(1, "'Name' column not found in SegY template file header.");
        if ((m_colType = tmp.indexOf(QRegExp("Type"), 0)) == -1) cErrWarnInfo::EWI(1, "'Type' column not found in SegY template file header.");
        if ((m_colValue = tmp.indexOf(QRegExp("Value"), 0)) == -1) cErrWarnInfo::EWI(1, "'Value' column not found in SegY template file header.");
        if ((m_colLoad = tmp.indexOf(QRegExp("Load"), 0)) == -1) cErrWarnInfo::EWI(1, "'Load' column not found in SegY template file header.");
        if ((m_colStartByte = tmp.indexOf(QRegExp("Start Byte"), 0)) == -1) cErrWarnInfo::EWI(1, "'Start Byte' column not found in SegY template file header.");
        if ((m_colEndByte = tmp.indexOf(QRegExp("End Byte"), 0)) == -1) cErrWarnInfo::EWI(1, "'End Byte' column not found in SegY template file header.");
        if ((m_colDesc = tmp.indexOf(QRegExp("Description"), 0)) == -1) cErrWarnInfo::EWI(1, "'Description' column not found in SegY template file header.");
    }

    return tmp;
}

//Parsing SegY Template file
bool CSegYDlg::TextParser(QString s, QStringList* sl)
{
    if (s.isEmpty() || s.isNull() || s.indexOf(";", 0, Qt::CaseSensitive) == -1)
    {
        cErrWarnInfo::EWI(ewiERROR, "Template file has an Empty or NULL line. Or (;) is missing within this line");
        return false;
    }

    *sl = s.split(';');

    if (sl->count() != 8)
    {
        cErrWarnInfo::EWI(ewiERROR, "Template file does not have 8 columns at line, " + s);
        return false;
    }

    return true;
}

//Update trace header values in the GUI
void CSegYDlg::on_spinBoxTrcNum_valueChanged(int arg1)
{
    arg1 = arg1 - 1; //Array indexing starts from zero
    if (m_IsSegYDataOK && m_IsSegYTemplOK)
    {

        std::shared_ptr<QVector<QVariant>> vHdr = m_pSeisData->GetAllHdrsForATrc(arg1);
        QTableWidgetItem* itm, * hItm;

        quint32 i = 0; //Index for the table widget
        quint32 j = 0; //Index for the trace array

        while (true)
        {
            itm = m_tableWdgtTrcHdr->item(i, m_colLoad);

            if (itm == nullptr)
                break;

            if (itm->text().toInt() == 1)
            {
                hItm = new QTableWidgetItem(vHdr->at(j).toString());
                hItm->setForeground(QBrush(Qt::blue));
                m_tableWdgtTrcHdr->setItem(i, m_colValue, hItm);

                ++j;

                if (j == vHdr->size())
                    break;
            }

            ++i;
        }
    }
}

void CSegYDlg::EnableOK(bool state)
{
    this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(state);
}

void CSegYDlg::UpdtEBCDICHdrUI()
{
    //Populate the GUI
    size_t size = m_pSeisData->GetEBCDIChdr()->size();
    quint16 i;

    for (i = 0; i < size; ++i) {
        m_txtBxTextHdr->append(m_pSeisData->GetEBCDIChdr()->at(i));
        //qDebug() << QString::fromStdString(trc->GetEBCDIChdr()->at(i));
    }
}

void CSegYDlg::UpdtBinHdrUI(bool isValid)
{
    //Populate the GUI
    size_t size = m_pSeisData->m_BinHdrs.size();
    quint16 i;
    long hdrVal;
    QBrush color;
    QTableWidgetItem* item;

    if (isValid)
        color = QBrush(Qt::blue);
    else
        color = QBrush(Qt::red);

    for (i = 0; i < size; ++i)
    {
        hdrVal = m_pSeisData->m_BinHdrs[i].m_val;
        item = new QTableWidgetItem(QString::number(hdrVal));
        item->setForeground(color);
        m_tableWdgtBinHdr->setItem(i, 2, item);
    }
}

