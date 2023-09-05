#ifndef CSEGYDLG_H
#define CSEGYDLG_H

#include <QTableWidget>
#include <QFileDialog>
#include <QString>
#include <QDebug>
#include <QDialog>
#include <QTextEdit>
#include <QSpinBox>
#include <memory>
#include "cSegYReadWrite.h"
#include "cSeisData.h"
#include "csegyFunc.h"
#include "AllEnums.h"
#include "ui_csegydlg.h"


class cSegYReadWrite; //forward declaration

namespace Ui {
    class CSegYDlg;
}

class CSegYDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CSegYDlg(std::shared_ptr<cSeisData> pSeisData,
        std::shared_ptr<cSegYReadWrite> pSgyRedWrt,
        QWidget* parent = 0);

    ~CSegYDlg();
    QStringList m_sgyFiles;
    QStringList m_sgyTemplFile;
    bool m_IsSegYTemplOK;
    bool m_IsSegYDataOK;
    void UpdtEBCDICHdrUI();
    void UpdtBinHdrUI(bool isValid);
    bool ExecuteSequence(QFile* inSegYFile); //Main sequence control function

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_pshBtnSegYBrowse_clicked();
    void on_pshBtnTmplBrowse_clicked();
    void LoadSegYTmplaToWgt(QTableWidget* tblWgtHdr, quint32 row, QStringList sl);
    void on_spinBoxTrcNum_valueChanged(int arg1);

private:
    Ui::CSegYDlg* ui;
    bool TextParser(QString s, QStringList* sl);
    void EnableOK(bool state);
    QStringList ExtractTableHeaders(QString s);
    bool HdrTreeWdgtSetUp();

    bool DataTypeComboBox();

    QTextEdit* m_txtBxTextHdr = new QTextEdit();
    QTableWidget* m_tableWdgtBinHdr = new QTableWidget();
    QTableWidget* m_tableWdgtTrcHdr = new QTableWidget();
    QSpinBox* m_trcHdrQCSpinBox = new QSpinBox();

    std::shared_ptr<cSeisData> m_pSeisData;
    std::shared_ptr<cSegYReadWrite> m_pSgyRedWrt;
    quint32 m_indx;        //Treewidget Index of item being loaded

    quint8 m_colName, m_colType, m_colValue, m_colLoad, m_colStartByte, m_colEndByte, m_colDesc;

    QStringList m_cmBxLst =
    {
        "2D Seismic Slice",
        "2D Geophysical Property"
    };
};

#endif // CSEGYDLG_H
