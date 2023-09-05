/********************************************************************************
** Form generated from reading UI file 'csegydlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CSEGYDLG_H
#define UI_CSEGYDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CSegYDlg
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QSplitter *splitter;
    QPushButton *pshBtnTmplBrowse;
    QLabel *lblTemplateFileName;
    QSplitter *splitter_2;
    QPushButton *pshBtnSegYBrowse;
    QLabel *lblSegYFileName;
    QHBoxLayout *horizontalLayout;
    QLabel *lbl_SelectDataType;
    QComboBox *cmbBxDataType;
    QSpacerItem *horizontalSpacer;
    QSplitter *splitter_3;
    QTreeWidget *treeWdgtSegYHdrs;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CSegYDlg)
    {
        if (CSegYDlg->objectName().isEmpty())
            CSegYDlg->setObjectName(QString::fromUtf8("CSegYDlg"));
        CSegYDlg->resize(1130, 698);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CSegYDlg->sizePolicy().hasHeightForWidth());
        CSegYDlg->setSizePolicy(sizePolicy);
        CSegYDlg->setModal(false);
        verticalLayout_2 = new QVBoxLayout(CSegYDlg);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(20);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        splitter = new QSplitter(CSegYDlg);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setMinimumSize(QSize(0, 10));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        splitter->setFont(font);
        splitter->setOrientation(Qt::Horizontal);
        pshBtnTmplBrowse = new QPushButton(splitter);
        pshBtnTmplBrowse->setObjectName(QString::fromUtf8("pshBtnTmplBrowse"));
        pshBtnTmplBrowse->setMinimumSize(QSize(0, 20));
        pshBtnTmplBrowse->setMaximumSize(QSize(130, 35));
        QFont font1;
        font1.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
        pshBtnTmplBrowse->setFont(font1);
        splitter->addWidget(pshBtnTmplBrowse);
        lblTemplateFileName = new QLabel(splitter);
        lblTemplateFileName->setObjectName(QString::fromUtf8("lblTemplateFileName"));
        lblTemplateFileName->setMinimumSize(QSize(0, 15));
        lblTemplateFileName->setMaximumSize(QSize(16777215, 35));
        QFont font2;
        font2.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
        font2.setPointSize(8);
        font2.setKerning(false);
        lblTemplateFileName->setFont(font2);
        lblTemplateFileName->setAutoFillBackground(true);
        lblTemplateFileName->setFrameShape(QFrame::Panel);
        splitter->addWidget(lblTemplateFileName);

        verticalLayout->addWidget(splitter);

        splitter_2 = new QSplitter(CSegYDlg);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setMinimumSize(QSize(0, 10));
        splitter_2->setFont(font);
        splitter_2->setOrientation(Qt::Horizontal);
        pshBtnSegYBrowse = new QPushButton(splitter_2);
        pshBtnSegYBrowse->setObjectName(QString::fromUtf8("pshBtnSegYBrowse"));
        pshBtnSegYBrowse->setMinimumSize(QSize(0, 20));
        pshBtnSegYBrowse->setMaximumSize(QSize(130, 35));
        QFont font3;
        font3.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
        font3.setPointSize(8);
        pshBtnSegYBrowse->setFont(font3);
        splitter_2->addWidget(pshBtnSegYBrowse);
        lblSegYFileName = new QLabel(splitter_2);
        lblSegYFileName->setObjectName(QString::fromUtf8("lblSegYFileName"));
        lblSegYFileName->setMinimumSize(QSize(0, 20));
        lblSegYFileName->setMaximumSize(QSize(16777215, 35));
        lblSegYFileName->setFont(font2);
        lblSegYFileName->setAutoFillBackground(true);
        lblSegYFileName->setFrameShape(QFrame::Panel);
        splitter_2->addWidget(lblSegYFileName);

        verticalLayout->addWidget(splitter_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, -1, 10);
        lbl_SelectDataType = new QLabel(CSegYDlg);
        lbl_SelectDataType->setObjectName(QString::fromUtf8("lbl_SelectDataType"));
        sizePolicy.setHeightForWidth(lbl_SelectDataType->sizePolicy().hasHeightForWidth());
        lbl_SelectDataType->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(lbl_SelectDataType);

        cmbBxDataType = new QComboBox(CSegYDlg);
        cmbBxDataType->setObjectName(QString::fromUtf8("cmbBxDataType"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cmbBxDataType->sizePolicy().hasHeightForWidth());
        cmbBxDataType->setSizePolicy(sizePolicy1);
        QFont font4;
        font4.setPointSize(8);
        cmbBxDataType->setFont(font4);
        cmbBxDataType->setFrame(true);

        horizontalLayout->addWidget(cmbBxDataType);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        splitter_3 = new QSplitter(CSegYDlg);
        splitter_3->setObjectName(QString::fromUtf8("splitter_3"));
        splitter_3->setMinimumSize(QSize(0, 10));
        splitter_3->setFont(font);
        splitter_3->setOrientation(Qt::Vertical);
        treeWdgtSegYHdrs = new QTreeWidget(splitter_3);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("SegY Headers"));
        treeWdgtSegYHdrs->setHeaderItem(__qtreewidgetitem);
        treeWdgtSegYHdrs->setObjectName(QString::fromUtf8("treeWdgtSegYHdrs"));
        treeWdgtSegYHdrs->setFont(font3);
        splitter_3->addWidget(treeWdgtSegYHdrs);

        verticalLayout->addWidget(splitter_3);


        verticalLayout_2->addLayout(verticalLayout);

        buttonBox = new QDialogButtonBox(CSegYDlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setMinimumSize(QSize(0, 20));
        buttonBox->setFont(font);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(CSegYDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), CSegYDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CSegYDlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(CSegYDlg);
    } // setupUi

    void retranslateUi(QDialog *CSegYDlg)
    {
        CSegYDlg->setWindowTitle(QCoreApplication::translate("CSegYDlg", "Load Seg-Y data", nullptr));
        pshBtnTmplBrowse->setText(QCoreApplication::translate("CSegYDlg", "SegY Template", nullptr));
        lblTemplateFileName->setText(QCoreApplication::translate("CSegYDlg", "Chose a template file.", nullptr));
        pshBtnSegYBrowse->setText(QCoreApplication::translate("CSegYDlg", "SegY File", nullptr));
        lblSegYFileName->setText(QCoreApplication::translate("CSegYDlg", "Chose a SegY file.", nullptr));
        lbl_SelectDataType->setText(QCoreApplication::translate("CSegYDlg", "Select Data Type", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CSegYDlg: public Ui_CSegYDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CSEGYDLG_H
