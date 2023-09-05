#include "infodialog.h"
#include "ui_infodialog.h"

cInfoDialog::cInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cInfoDialog)
{
    ui->setupUi(this);
}

cInfoDialog::~cInfoDialog()
{
    delete ui;
}

bool cInfoDialog::UpdateText(QString txt)
{
    this->ui->textBrowser->setText(txt);
    return false;
}
