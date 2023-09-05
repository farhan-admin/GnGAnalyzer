/********************************************************************************
** Form generated from reading UI file 'infodialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFODIALOG_H
#define UI_INFODIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_cInfoDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTextBrowser *textBrowser;

    void setupUi(QDialog *cInfoDialog)
    {
        if (cInfoDialog->objectName().isEmpty())
            cInfoDialog->setObjectName(QString::fromUtf8("cInfoDialog"));
        cInfoDialog->setWindowModality(Qt::ApplicationModal);
        cInfoDialog->resize(554, 457);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(cInfoDialog->sizePolicy().hasHeightForWidth());
        cInfoDialog->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(cInfoDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        textBrowser = new QTextBrowser(cInfoDialog);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout->addWidget(textBrowser);


        retranslateUi(cInfoDialog);

        QMetaObject::connectSlotsByName(cInfoDialog);
    } // setupUi

    void retranslateUi(QDialog *cInfoDialog)
    {
        cInfoDialog->setWindowTitle(QCoreApplication::translate("cInfoDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class cInfoDialog: public Ui_cInfoDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFODIALOG_H
