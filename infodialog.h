#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QDialog>

namespace Ui {
class cInfoDialog;
}

class cInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit cInfoDialog(QWidget *parent = nullptr);
    ~cInfoDialog();

    bool UpdateText(QString txt);

private:
    Ui::cInfoDialog *ui;
};

#endif // INFODIALOG_H
