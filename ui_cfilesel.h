/********************************************************************************
** Form generated from reading UI file 'cfilesel.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CFILESEL_H
#define UI_CFILESEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>

QT_BEGIN_NAMESPACE

class Ui_CFileSel
{
public:
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CFileSel)
    {
        if (CFileSel->objectName().isEmpty())
            CFileSel->setObjectName(QString::fromUtf8("CFileSel"));
        CFileSel->resize(400, 300);
        buttonBox = new QDialogButtonBox(CFileSel);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        retranslateUi(CFileSel);
        QObject::connect(buttonBox, SIGNAL(accepted()), CFileSel, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CFileSel, SLOT(reject()));

        QMetaObject::connectSlotsByName(CFileSel);
    } // setupUi

    void retranslateUi(QDialog *CFileSel)
    {
        CFileSel->setWindowTitle(QCoreApplication::translate("CFileSel", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CFileSel: public Ui_CFileSel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CFILESEL_H
