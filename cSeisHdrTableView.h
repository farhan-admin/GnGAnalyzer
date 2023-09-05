#pragma once

#include <memory>
#include <QTableView>
#include "cSeisData.h"


class cSeisHdrTableView : public QTableView
{
    Q_OBJECT

public:
    cSeisHdrTableView(QWidget* parent = nullptr);
    void InitSeisHdrTblView(std::shared_ptr<cSeisData> pSeisData);


};


