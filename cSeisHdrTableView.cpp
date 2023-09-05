#include "cSeisHdrTableView.h"
#include <QDebug>
//#include <QtConcurrent/QtConcurrent>

//namespace GnV //GnGViewer
//{

	cSeisHdrTableView::cSeisHdrTableView(QWidget* parent)
	{
		Q_UNUSED(parent);
		//The below is generating an exception need to investigate
		//this->setAttribute(Qt::WA_DeleteOnClose); //to ensure that window is deleted when closed

	}

	void cSeisHdrTableView::InitSeisHdrTblView(std::shared_ptr<cSeisData> pSeisData)
	{

		//QtConcurrent::run([this, pSeisData]() { this->setModel(pSeisData.get()); } );

		this->setModel(pSeisData.get());

		this->setShowGrid(true);

		this->setMinimumSize(1000, 500);

		this->setWindowTitle(pSeisData->m_DataName);

		this->show();

	}

//}