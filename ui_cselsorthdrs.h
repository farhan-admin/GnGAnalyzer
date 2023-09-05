/********************************************************************************
** Form generated from reading UI file 'cselsorthdrs.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CSELSORTHDRS_H
#define UI_CSELSORTHDRS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CSelSortHdrs
{
public:
    QDialogButtonBox *buttonBox;
    QListWidget *listWidgetAvailHdrs;
    QLabel *lblAvailHdrs;
    QListWidget *listWidgetSelHdrs;
    QLabel *lblSelHdrs;
    QPushButton *pushButtonAddSel;
    QPushButton *pushButtonRmvSel;
    QComboBox *comboBoxGthSplitHdr;
    QLabel *label;
    QLineEdit *lineEditGatherName;
    QLabel *label_2;

    void setupUi(QDialog *CSelSortHdrs)
    {
        if (CSelSortHdrs->objectName().isEmpty())
            CSelSortHdrs->setObjectName(QString::fromUtf8("CSelSortHdrs"));
        CSelSortHdrs->resize(565, 665);
        buttonBox = new QDialogButtonBox(CSelSortHdrs);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(70, 630, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        listWidgetAvailHdrs = new QListWidget(CSelSortHdrs);
        listWidgetAvailHdrs->setObjectName(QString::fromUtf8("listWidgetAvailHdrs"));
        listWidgetAvailHdrs->setGeometry(QRect(10, 60, 241, 561));
        listWidgetAvailHdrs->setSortingEnabled(true);
        lblAvailHdrs = new QLabel(CSelSortHdrs);
        lblAvailHdrs->setObjectName(QString::fromUtf8("lblAvailHdrs"));
        lblAvailHdrs->setGeometry(QRect(10, 40, 131, 16));
        QFont font;
        font.setPointSize(8);
        lblAvailHdrs->setFont(font);
        lblAvailHdrs->setCursor(QCursor(Qt::ArrowCursor));
        lblAvailHdrs->setToolTipDuration(1);
        listWidgetSelHdrs = new QListWidget(CSelSortHdrs);
        listWidgetSelHdrs->setObjectName(QString::fromUtf8("listWidgetSelHdrs"));
        listWidgetSelHdrs->setGeometry(QRect(320, 60, 231, 411));
        listWidgetSelHdrs->setSortingEnabled(false);
        lblSelHdrs = new QLabel(CSelSortHdrs);
        lblSelHdrs->setObjectName(QString::fromUtf8("lblSelHdrs"));
        lblSelHdrs->setGeometry(QRect(320, 40, 131, 16));
        lblSelHdrs->setFont(font);
        lblSelHdrs->setCursor(QCursor(Qt::ArrowCursor));
        lblSelHdrs->setToolTipDuration(1);
        pushButtonAddSel = new QPushButton(CSelSortHdrs);
        pushButtonAddSel->setObjectName(QString::fromUtf8("pushButtonAddSel"));
        pushButtonAddSel->setGeometry(QRect(270, 100, 31, 31));
        pushButtonRmvSel = new QPushButton(CSelSortHdrs);
        pushButtonRmvSel->setObjectName(QString::fromUtf8("pushButtonRmvSel"));
        pushButtonRmvSel->setGeometry(QRect(270, 170, 31, 31));
        comboBoxGthSplitHdr = new QComboBox(CSelSortHdrs);
        comboBoxGthSplitHdr->setObjectName(QString::fromUtf8("comboBoxGthSplitHdr"));
        comboBoxGthSplitHdr->setGeometry(QRect(320, 530, 231, 31));
        label = new QLabel(CSelSortHdrs);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(320, 510, 151, 16));
        label->setFont(font);
        lineEditGatherName = new QLineEdit(CSelSortHdrs);
        lineEditGatherName->setObjectName(QString::fromUtf8("lineEditGatherName"));
        lineEditGatherName->setGeometry(QRect(170, 10, 113, 20));
        label_2 = new QLabel(CSelSortHdrs);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 10, 141, 16));

        retranslateUi(CSelSortHdrs);
        QObject::connect(buttonBox, SIGNAL(accepted()), CSelSortHdrs, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CSelSortHdrs, SLOT(reject()));

        QMetaObject::connectSlotsByName(CSelSortHdrs);
    } // setupUi

    void retranslateUi(QDialog *CSelSortHdrs)
    {
        CSelSortHdrs->setWindowTitle(QCoreApplication::translate("CSelSortHdrs", "Dialog", nullptr));
        lblAvailHdrs->setText(QCoreApplication::translate("CSelSortHdrs", "Available Headers", nullptr));
        lblSelHdrs->setText(QCoreApplication::translate("CSelSortHdrs", "Selected Headers", nullptr));
        pushButtonAddSel->setText(QCoreApplication::translate("CSelSortHdrs", ">>", nullptr));
        pushButtonRmvSel->setText(QCoreApplication::translate("CSelSortHdrs", "<<", nullptr));
        label->setText(QCoreApplication::translate("CSelSortHdrs", "Gather Splitting Header", nullptr));
        lineEditGatherName->setText(QCoreApplication::translate("CSelSortHdrs", "Custom Sort", nullptr));
        label_2->setText(QCoreApplication::translate("CSelSortHdrs", "Enter a Name for your sorted gather", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CSelSortHdrs: public Ui_CSelSortHdrs {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CSELSORTHDRS_H
