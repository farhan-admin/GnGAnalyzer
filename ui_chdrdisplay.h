/********************************************************************************
** Form generated from reading UI file 'chdrdisplay.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHDRDISPLAY_H
#define UI_CHDRDISPLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_CHdrDisplay
{
public:
    QDialogButtonBox *buttonBox;
    QTableWidget *tblwgtSpredShet;

    void setupUi(QDialog *CHdrDisplay)
    {
        if (CHdrDisplay->objectName().isEmpty())
            CHdrDisplay->setObjectName(QString::fromUtf8("CHdrDisplay"));
        CHdrDisplay->resize(1046, 713);
        buttonBox = new QDialogButtonBox(CHdrDisplay);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(860, 670, 171, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        tblwgtSpredShet = new QTableWidget(CHdrDisplay);
        tblwgtSpredShet->setObjectName(QString::fromUtf8("tblwgtSpredShet"));
        tblwgtSpredShet->setGeometry(QRect(10, 10, 1021, 651));
        QFont font;
        font.setFamily(QString::fromUtf8("FreeSans"));
        tblwgtSpredShet->setFont(font);

        retranslateUi(CHdrDisplay);
        QObject::connect(buttonBox, SIGNAL(accepted()), CHdrDisplay, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CHdrDisplay, SLOT(reject()));

        QMetaObject::connectSlotsByName(CHdrDisplay);
    } // setupUi

    void retranslateUi(QDialog *CHdrDisplay)
    {
        CHdrDisplay->setWindowTitle(QCoreApplication::translate("CHdrDisplay", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CHdrDisplay: public Ui_CHdrDisplay {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHDRDISPLAY_H
