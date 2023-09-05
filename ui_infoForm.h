/********************************************************************************
** Form generated from reading UI file 'infoForm.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFOFORM_H
#define UI_INFOFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_aboutForm
{
public:

    void setupUi(QWidget *aboutForm)
    {
        if (aboutForm->objectName().isEmpty())
            aboutForm->setObjectName(QString::fromUtf8("aboutForm"));
        aboutForm->resize(400, 300);

        retranslateUi(aboutForm);

        QMetaObject::connectSlotsByName(aboutForm);
    } // setupUi

    void retranslateUi(QWidget *aboutForm)
    {
        aboutForm->setWindowTitle(QCoreApplication::translate("aboutForm", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class aboutForm: public Ui_aboutForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFOFORM_H
