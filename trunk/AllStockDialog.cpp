#include "StdAfx.h"
#include "AllStockDialog.h"
#include "DataEngine.h"

CAllStockDialog::CAllStockDialog( QWidget* parent /*= 0*/ )
	: QDialog(parent)
{
	QSplitter* pSplitter = new QSplitter(Qt::Vertical,this);
	QVBoxLayout* pLayout = new QVBoxLayout(this);
	pLayout->addWidget(pSplitter);
	pLayout->setSpacing(0);
	setLayout(pLayout);

	m_pWidgetAll = new CAllStockWidget(0);
	pSplitter->addWidget(m_pWidgetAll);

	QWidget* pWidgetCtrl = new QWidget(pSplitter);
	pSplitter->addWidget(pWidgetCtrl);

	pSplitter->setSizes(QList<int>()<<70<<30);

	QHBoxLayout* pHLayout = new QHBoxLayout(pWidgetCtrl);
	pHLayout->setSpacing(0);
	pWidgetCtrl->setLayout(pHLayout);

	{
		m_pWidgetKLine = new CKLineWidget(0);
		//
		QFile file(qApp->applicationDirPath()+"/AllStock.xml");
		if(file.open(QFile::ReadOnly))
		{
			QDomDocument doc;
			doc.setContent(file.readAll());
			file.close();

			QDomElement eleRoot = doc.firstChildElement("ROOT");
			if(eleRoot.isElement())
			{
				QDomElement eleWidget = eleRoot.firstChildElement("widget");
				if(eleWidget.isElement())
				{
					m_pWidgetKLine->loadPanelInfo(eleWidget);
				}
			}
		}

		connect(m_pWidgetAll,SIGNAL(stockFocus(CStockInfoItem*)),this,SLOT(onStockFocusChanged(CStockInfoItem*)));
		pHLayout->addWidget(m_pWidgetKLine,70);
	}

	{
		QGridLayout* pGLayout = new QGridLayout();
		{
			//��ʼ�����ڰ�ť
			QPushButton* pBtn5Min = new QPushButton(pWidgetCtrl);
			pBtn5Min->setText(tr("5��"));
			connect(pBtn5Min,SIGNAL(clicked()),this,SLOT(onCircle5Min()));
			pGLayout->addWidget(pBtn5Min,0,0,1,1);

			QPushButton* pBtnDay = new QPushButton(pWidgetCtrl);
			pBtnDay->setText(tr("����"));
			connect(pBtnDay,SIGNAL(clicked()),this,SLOT(onCircleDay()));
			pGLayout->addWidget(pBtnDay,0,1,1,1);

			QPushButton* pBtnWeek = new QPushButton(pWidgetCtrl);
			pBtnWeek->setText(tr("����"));
			connect(pBtnWeek,SIGNAL(clicked()),this,SLOT(onCircleWeek()));
			pGLayout->addWidget(pBtnWeek,0,2,1,1);

			QPushButton* pBtnMonth = new QPushButton(pWidgetCtrl);
			pBtnMonth->setText(tr("����"));
			connect(pBtnMonth,SIGNAL(clicked()),this,SLOT(onCircleMonth()));
			pGLayout->addWidget(pBtnMonth,0,3,1,1);
		}
		{
			//��ʼ������ť
//			SortByCode = 1,		//����Ʊ��������
//			SortByZGB,			//���ܹɱ�����
//			SortByIncrease,		//���Ƿ�����
//			SortByTurnRatio,	//������������
//			SortByVolumeRatio,	//����������

			QPushButton* pBtnStCode = new QPushButton(pWidgetCtrl);
			pBtnStCode->setText(tr("����"));
			connect(pBtnStCode,SIGNAL(clicked()),this,SLOT(onSortCode()));
			pGLayout->addWidget(pBtnStCode,1,0,1,1);

			QPushButton* pBtnStZGB = new QPushButton(pWidgetCtrl);
			pBtnStZGB->setText(tr("�ܹɱ�"));
			connect(pBtnStZGB,SIGNAL(clicked()),this,SLOT(onSortZGB()));
			pGLayout->addWidget(pBtnStZGB,1,1,1,1);

			QPushButton* pBtnStInc = new QPushButton(pWidgetCtrl);
			pBtnStInc->setText(tr("�Ƿ�"));
			connect(pBtnStInc,SIGNAL(clicked()),this,SLOT(onSortInc()));
			pGLayout->addWidget(pBtnStInc,1,2,1,1);

			QPushButton* pBtnStTurnRatio = new QPushButton(pWidgetCtrl);
			pBtnStTurnRatio->setText(tr("������"));
			connect(pBtnStTurnRatio,SIGNAL(clicked()),this,SLOT(onSortTurnRatio()));
			pGLayout->addWidget(pBtnStTurnRatio,1,3,1,1);

			QPushButton* pBtnStVolumeRatio = new QPushButton(pWidgetCtrl);
			pBtnStVolumeRatio->setText(tr("����"));
			connect(pBtnStVolumeRatio,SIGNAL(clicked()),this,SLOT(onSortVolumeRatio()));
			pGLayout->addWidget(pBtnStVolumeRatio,1,4,1,1);
		}

		pHLayout->addLayout(pGLayout,30);
	}

	showMaximized();
}

CAllStockDialog::~CAllStockDialog(void)
{
	if(m_pWidgetAll)
		delete m_pWidgetAll;
	if(m_pWidgetKLine)
	{
		QDomDocument doc("AllStock");
		QDomElement eleRoot = doc.createElement("ROOT");
		doc.appendChild(eleRoot);
		QDomElement eleWidget = doc.createElement("widget");
		eleRoot.appendChild(eleWidget);
		m_pWidgetKLine->savePanelInfo(doc,eleWidget);

		QFile file(qApp->applicationDirPath()+"/AllStock.xml");
		if(file.open(QFile::Truncate|QFile::WriteOnly))
		{
			file.write(doc.toByteArray());
			file.close();
		}

		delete m_pWidgetKLine;
	}
}

void CAllStockDialog::onCircle5Min()
{
	m_pWidgetAll->setCircle(Min5);
}

void CAllStockDialog::onCircleDay()
{
	m_pWidgetAll->setCircle(Day);
}

void CAllStockDialog::onCircleWeek()
{
	m_pWidgetAll->setCircle(Week);
}

void CAllStockDialog::onCircleMonth()
{
	m_pWidgetAll->setCircle(Month);
}

void CAllStockDialog::onSortCode()
{
	m_pWidgetAll->setSortType(CAllStockWidget::SortByCode);
}

void CAllStockDialog::onSortZGB()
{
	m_pWidgetAll->setSortType(CAllStockWidget::SortByZGB);
}

void CAllStockDialog::onSortInc()
{
	m_pWidgetAll->setSortType(CAllStockWidget::SortByIncrease);
}

void CAllStockDialog::onSortTurnRatio()
{
	m_pWidgetAll->setSortType(CAllStockWidget::SortByTurnRatio);
}

void CAllStockDialog::onSortVolumeRatio()
{
	m_pWidgetAll->setSortType(CAllStockWidget::SortByVolumeRatio);
}

void CAllStockDialog::onStockFocusChanged( CStockInfoItem* pStock )
{
	m_pWidgetKLine->setStockCode(pStock->getOnly());
}
