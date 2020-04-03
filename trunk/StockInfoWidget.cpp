/************************************************************************/
/* �ļ����ƣ�StockInfoWidget.cpp
/* ����ʱ�䣺2012-12-13 17:55
/*
/* ��    ����������ʾ��ֻ��Ʊ��������Ϣ���̳���CBaseWidget
/************************************************************************/

#include "StdAfx.h"
#include "StockInfoWidget.h"
#include "DataEngine.h"
#include "KeyWizard.h"

#define	R_DATA_COLOR(v)	( (v>=0) ? m_clrDataRed : m_clrDataBlue)

CStockInfoWidget::CStockInfoWidget( CBaseWidget* parent /*= 0*/ )
	: CBaseWidget(parent, WidgetStockInfo)
	, m_pStockItem(0)
	, m_iItemHeight(16)
	, m_clrGrid(176,0,0)
	, m_clrTitle(255,255,0)
	, m_clrDesc(192,192,192)
	, m_clrDataBlue(0,230,0)
	, m_clrDataRed(255,50,50)
	, m_clrDataNone(192,192,0)
{
	m_pMenuCustom = new QMenu(tr("������Ϣ�˵�"));
	m_pMenuCustom->addAction(tr("���ù�Ʊ����"),this,SLOT(onSetStockCode()));
}

CStockInfoWidget::~CStockInfoWidget(void)
{
	delete m_pMenuCustom;
	disconnect(m_pStockItem,SIGNAL(stockItemReportChanged(const QString&)),this,SLOT(updateStockInfo(const QString&)));
}

bool CStockInfoWidget::loadPanelInfo( const QDomElement& eleWidget )
{
	if(!CBaseWidget::loadPanelInfo(eleWidget))
		return false;


	QDomElement eleCode = eleWidget.firstChildElement("code");
	if(eleCode.isElement())
		setStockCode(eleCode.text());

	return true;
}

bool CStockInfoWidget::savePanelInfo( QDomDocument& doc,QDomElement& eleWidget )
{
	if(!CBaseWidget::savePanelInfo(doc,eleWidget))
		return false;

	if(m_pStockItem)
	{
		//��ǰ�Ĺ�Ʊֵ
		QDomElement eleCode = doc.createElement("code");
		eleCode.appendChild(doc.createTextNode(m_pStockItem->getOnly()));
		eleWidget.appendChild(eleCode);
	}

	return true;
}

void CStockInfoWidget::setStockCode( const QString& only )
{
	setStockItem(CDataEngine::getDataEngine()->getStockInfoItem(only));
	return CBaseWidget::setStockCode(only);
}

void CStockInfoWidget::setStockItem( CStockInfoItem* pItem )
{
	if(pItem)
	{
		//�Ƴ����к� updateStockInfo������ �ź�/��
		if(m_pStockItem)
		{
			disconnect(m_pStockItem,SIGNAL(stockItemReportChanged(const QString&)),this,SLOT(updateStockInfo(const QString&)));
		}

		m_pStockItem = pItem;

		//�������»���
		//connect(pItem,SIGNAL(stockItemHistoryChanged(const QString&)),this,SLOT(updateStockInfo(const QString&)));
		//connect(pItem,SIGNAL(stockItemFenBiChanged(const QString&)),this,SLOT(updateStockInfo(const QString&)));
		connect(pItem,SIGNAL(stockItemReportChanged(const QString&)),this,SLOT(updateStockInfo(const QString&)));

		//����������Ϣ
		update();
	}
}

void CStockInfoWidget::updateStockInfo( const QString& only )
{
	if(m_pStockItem&&m_pStockItem->getOnly()!=only)
		return;

	update();
	//	resetTmpData();
}

void CStockInfoWidget::paintEvent( QPaintEvent* )
{
	QPainter p(this);
	QRect rtClient = this->rect();
	p.fillRect(rtClient,QColor(0,0,0));

	if(!m_pStockItem)
		return;


	int iCurY = rtClient.top();
	int iMidX = rtClient.width()/2;

	{
		//���Ʊ���
		QRect rtTitle(rtClient.left(),iCurY,rtClient.width(),m_iItemHeight);
		p.setPen(m_clrGrid);
		p.drawRect(rtTitle);
		p.setPen(m_clrTitle);
		p.drawText(rtTitle,Qt::AlignCenter,QString("%1 %2").arg(m_pStockItem->getCode()).arg(m_pStockItem->getName()));

		iCurY += m_iItemHeight;
	}

	p.setPen(m_clrGrid);
	p.drawLine(iMidX,iCurY,iMidX,rtClient.bottom());
	{
		//���� ί�� ί��
		QRect rtComm(rtClient.left(),iCurY,rtClient.width(),m_iItemHeight);
		QRect rtCommRatio(rtClient.left()+2,iCurY,iMidX-3,m_iItemHeight);	//ί��
		QRect rtCommSent(iMidX+2,iCurY,rtCommRatio.width(),m_iItemHeight); //ί��
		p.setPen(m_clrGrid);
		p.drawRect(rtComm);

		p.setPen(m_clrDesc);
		p.drawText(rtCommRatio,Qt::AlignLeft|Qt::AlignVCenter,tr("ί��"));
		p.drawText(rtCommSent,Qt::AlignLeft|Qt::AlignVCenter,tr("ί��"));

		p.setPen(m_clrDataRed);
		p.drawText(rtCommRatio,Qt::AlignRight|Qt::AlignVCenter,QString("%1%").arg(m_pStockItem->getCommRatio(),0,'f',2));
		p.drawText(rtCommSent,Qt::AlignRight|Qt::AlignVCenter,QString("%1").arg(m_pStockItem->getCommSent(),0,'f',2));

		iCurY += m_iItemHeight;
	}
	{
		//ί�� ί��
		qRcvReportData* pReport = m_pStockItem->getCurrentReport();
		if(pReport)
		{
			//ί��
			{
				QRect rtSell(rtClient.left(),iCurY,rtClient.right(),m_iItemHeight*5);
				p.setPen(m_clrGrid);
				p.drawRect(rtSell);

				{
					QRect rtLeft(rtSell.left()+2,iCurY,iMidX-3,m_iItemHeight);
					QRect rtRight(iMidX+2,iCurY,rtLeft.width(),m_iItemHeight);

					p.setPen(m_clrDesc);
					p.drawText(rtLeft,Qt::AlignLeft|Qt::AlignVCenter,
						QString("ί��5"));

					p.setPen(m_clrDataRed);
					p.drawText(rtLeft,Qt::AlignRight|Qt::AlignVCenter,
						QString("%1").arg(pReport->fSellPrice5,0,'f',2));

					p.setPen(m_clrDataNone);
					p.drawText(rtRight,Qt::AlignRight|Qt::AlignVCenter,
						QString("%1").arg(pReport->fSellVolume5));

					iCurY += m_iItemHeight;
				}
				{
					QRect rtLeft(rtSell.left()+2,iCurY,iMidX-3,m_iItemHeight);
					QRect rtRight(iMidX+2,iCurY,rtLeft.width(),m_iItemHeight);

					p.setPen(m_clrDesc);
					p.drawText(rtLeft,Qt::AlignLeft|Qt::AlignVCenter,
						QString("ί��4"));

					p.setPen(m_clrDataRed);
					p.drawText(rtLeft,Qt::AlignRight|Qt::AlignVCenter,
						QString("%1").arg(pReport->fSellPrice4,0,'f',2));

					p.setPen(m_clrDataNone);
					p.drawText(rtRight,Qt::AlignRight|Qt::AlignVCenter,
						QString("%1").arg(pReport->fSellVolume4));

					iCurY += m_iItemHeight;
				}
				for (int i=2;i>=0;--i)
				{
					QRect rtLeft(rtSell.left()+2,iCurY,iMidX-3,m_iItemHeight);
					QRect rtRight(iMidX+2,iCurY,rtLeft.width(),m_iItemHeight);

					p.setPen(m_clrDesc);
					p.drawText(rtLeft,Qt::AlignLeft|Qt::AlignVCenter,
						QString("ί��%1").arg(i+1));

					p.setPen(m_clrDataRed);
					p.drawText(rtLeft,Qt::AlignRight|Qt::AlignVCenter,
						QString("%1").arg(pReport->fSellPrice[i],0,'f',2));

					p.setPen(m_clrDataNone);
					p.drawText(rtRight,Qt::AlignRight|Qt::AlignVCenter,
						QString("%1").arg(pReport->fSellVolume[i]));

					iCurY += m_iItemHeight;
				}
			}

			//ί��
			{
				QRect rtBuy(rtClient.left(),iCurY,rtClient.right(),m_iItemHeight*5);
				p.setPen(m_clrGrid);
				p.drawRect(rtBuy);

				for (int i=0;i<3;++i)
				{
					QRect rtLeft(rtBuy.left()+2,iCurY,iMidX-3,m_iItemHeight);
					QRect rtRight(iMidX+2,iCurY,rtLeft.width(),m_iItemHeight);

					p.setPen(m_clrDesc);
					p.drawText(rtLeft,Qt::AlignLeft|Qt::AlignVCenter,
						QString("ί��%1").arg(i+1));

					p.setPen(m_clrDataRed);
					p.drawText(rtLeft,Qt::AlignRight|Qt::AlignVCenter,
						QString("%1").arg(pReport->fBuyPrice[i],0,'f',2));

					p.setPen(m_clrDataNone);
					p.drawText(rtRight,Qt::AlignRight|Qt::AlignVCenter,
						QString("%1").arg(pReport->fBuyVolume[i]));

					iCurY += m_iItemHeight;
				}
				{
					QRect rtLeft(rtBuy.left()+2,iCurY,iMidX-3,m_iItemHeight);
					QRect rtRight(iMidX+2,iCurY,rtLeft.width(),m_iItemHeight);

					p.setPen(m_clrDesc);
					p.drawText(rtLeft,Qt::AlignLeft|Qt::AlignVCenter,
						QString("ί��4"));

					p.setPen(m_clrDataRed);
					p.drawText(rtLeft,Qt::AlignRight|Qt::AlignVCenter,
						QString("%1").arg(pReport->fBuyPrice4,0,'f',2));

					p.setPen(m_clrDataNone);
					p.drawText(rtRight,Qt::AlignRight|Qt::AlignVCenter,
						QString("%1").arg(pReport->fBuyVolume4));

					iCurY += m_iItemHeight;
				}
				{
					QRect rtLeft(rtBuy.left()+2,iCurY,iMidX-3,m_iItemHeight);
					QRect rtRight(iMidX+2,iCurY,rtLeft.width(),m_iItemHeight);

					p.setPen(m_clrDesc);
					p.drawText(rtLeft,Qt::AlignLeft|Qt::AlignVCenter,
						QString("ί��5"));

					p.setPen(m_clrDataRed);
					p.drawText(rtLeft,Qt::AlignRight|Qt::AlignVCenter,
						QString("%1").arg(pReport->fBuyPrice5,0,'f',2));

					p.setPen(m_clrDataNone);
					p.drawText(rtRight,Qt::AlignRight|Qt::AlignVCenter,
						QString("%1").arg(pReport->fBuyVolume5));

					iCurY += m_iItemHeight;
				}
			}
		}

	}

	{
		//��ǰ����
		QRect rtInfo(rtClient.left(),iCurY,rtClient.width(),m_iItemHeight*5);
		p.setPen(m_clrGrid);
		p.drawRect(rtInfo);
		{
			//�ּۣ���
			QRect rtLeft(rtInfo.left()+2,iCurY,iMidX-3,m_iItemHeight);
			QRect rtRight(iMidX+2,iCurY,rtLeft.width(),m_iItemHeight);
			p.setPen(m_clrDesc);
			p.drawText(rtLeft,Qt::AlignLeft|Qt::AlignVCenter,tr("�ּ�"));
			p.drawText(rtRight,Qt::AlignLeft|Qt::AlignVCenter,tr("��"));

			p.setPen(R_DATA_COLOR(m_pStockItem->getNewPrice()-m_pStockItem->getLastClose()));
			p.drawText(rtLeft,Qt::AlignRight|Qt::AlignVCenter,QString("%1").arg(m_pStockItem->getNewPrice(),0,'f',2));
			p.setPen(R_DATA_COLOR(m_pStockItem->getOpenPrice()-m_pStockItem->getLastClose()));
			p.drawText(rtRight,Qt::AlignRight|Qt::AlignVCenter,QString("%1").arg(m_pStockItem->getOpenPrice(),0,'f',2));

			iCurY += m_iItemHeight;
		}
		{
			//�ǵ������
			QRect rtLeft(rtInfo.left()+2,iCurY,iMidX-3,m_iItemHeight);
			QRect rtRight(iMidX+2,iCurY,rtLeft.width(),m_iItemHeight);
			p.setPen(m_clrDesc);
			p.drawText(rtLeft,Qt::AlignLeft|Qt::AlignVCenter,tr("�ǵ�"));
			p.drawText(rtRight,Qt::AlignLeft|Qt::AlignVCenter,tr("���"));

			p.setPen(R_DATA_COLOR(m_pStockItem->getPriceFluctuate()));
			p.drawText(rtLeft,Qt::AlignRight|Qt::AlignVCenter,QString("%1").arg(m_pStockItem->getPriceFluctuate(),0,'f',2));
			p.setPen(R_DATA_COLOR(m_pStockItem->getHighPrice()-m_pStockItem->getLastClose()));
			p.drawText(rtRight,Qt::AlignRight|Qt::AlignVCenter,QString("%1").arg(m_pStockItem->getHighPrice(),0,'f',2));

			iCurY += m_iItemHeight;
		}
		{
			//�Ƿ������
			QRect rtLeft(rtInfo.left()+2,iCurY,iMidX-3,m_iItemHeight);
			QRect rtRight(iMidX+2,iCurY,rtLeft.width(),m_iItemHeight);
			p.setPen(m_clrDesc);
			p.drawText(rtLeft,Qt::AlignLeft|Qt::AlignVCenter,tr("�Ƿ�"));
			p.drawText(rtRight,Qt::AlignLeft|Qt::AlignVCenter,tr("���"));

			p.setPen(R_DATA_COLOR(m_pStockItem->getIncrease()));
			p.drawText(rtLeft,Qt::AlignRight|Qt::AlignVCenter,QString("%1%").arg(m_pStockItem->getIncrease(),0,'f',2));
			p.setPen(R_DATA_COLOR(m_pStockItem->getLowPrice()-m_pStockItem->getLastClose()));
			p.drawText(rtRight,Qt::AlignRight|Qt::AlignVCenter,QString("%1").arg(m_pStockItem->getLowPrice(),0,'f',2));

			iCurY += m_iItemHeight;
		}
		{
			//����������
			QRect rtLeft(rtInfo.left()+2,iCurY,iMidX-3,m_iItemHeight);
			QRect rtRight(iMidX+2,iCurY,rtLeft.width(),m_iItemHeight);
			p.setPen(m_clrDesc);
			p.drawText(rtLeft,Qt::AlignLeft|Qt::AlignVCenter,tr("����"));
			p.drawText(rtRight,Qt::AlignLeft|Qt::AlignVCenter,tr("����"));

			p.setPen(m_clrDataNone);
			p.drawText(rtLeft,Qt::AlignRight|Qt::AlignVCenter,QString("%1").arg(m_pStockItem->getTotalAmount()));
			p.setPen(R_DATA_COLOR(m_pStockItem->getVolumeRatio()));
			p.drawText(rtRight,Qt::AlignRight|Qt::AlignVCenter,QString("%1").arg(m_pStockItem->getVolumeRatio(),0,'f',2));

			iCurY += m_iItemHeight;
		}
		{
			//���̣�����
			QRect rtLeft(rtInfo.left()+2,iCurY,iMidX-3,m_iItemHeight);
			QRect rtRight(iMidX+2,iCurY,rtLeft.width(),m_iItemHeight);
			p.setPen(m_clrDesc);
			p.drawText(rtLeft,Qt::AlignLeft|Qt::AlignVCenter,tr("����"));
			p.drawText(rtRight,Qt::AlignLeft|Qt::AlignVCenter,tr("����"));

			p.setPen(m_clrDataRed);
			p.drawText(rtLeft,Qt::AlignRight|Qt::AlignVCenter,QString("%1").arg(m_pStockItem->getSellVOL()));
			p.drawText(rtRight,Qt::AlignRight|Qt::AlignVCenter,QString("%1").arg(m_pStockItem->getBuyVOL()));

			iCurY += m_iItemHeight;
		}
	}
	{
		//������Ϣ
		QRect rtInfo(rtClient.left(),iCurY,rtClient.width(),rtClient.bottom()-iCurY);
		p.setPen(m_clrGrid);
		p.drawRect(rtInfo);
		{
			//���֣��ɱ�
			QRect rtLeft(rtInfo.left()+2,iCurY,iMidX-3,m_iItemHeight);
			QRect rtRight(iMidX+2,iCurY,rtLeft.width(),m_iItemHeight);
			p.setPen(m_clrDesc);
			p.drawText(rtLeft,Qt::AlignLeft|Qt::AlignVCenter,tr("����"));
			p.drawText(rtRight,Qt::AlignLeft|Qt::AlignVCenter,tr("�ɱ�"));

			p.drawText(rtLeft,Qt::AlignRight|Qt::AlignVCenter,QString("%1%").arg(m_pStockItem->getTurnRatio(),0,'f',2));
			p.drawText(rtRight,Qt::AlignRight|Qt::AlignVCenter,QString("%1").arg(m_pStockItem->getZGB()));

			iCurY += m_iItemHeight;
		}
		{
			//���ʣ���ͨ
			QRect rtLeft(rtInfo.left()+2,iCurY,iMidX-3,m_iItemHeight);
			QRect rtRight(iMidX+2,iCurY,rtLeft.width(),m_iItemHeight);
			p.setPen(m_clrDesc);
			p.drawText(rtLeft,Qt::AlignLeft|Qt::AlignVCenter,tr("����"));
			p.drawText(rtRight,Qt::AlignLeft|Qt::AlignVCenter,tr("��ͨ"));

			p.drawText(rtLeft,Qt::AlignRight|Qt::AlignVCenter,QString("%1").arg(m_pStockItem->getMgjzc(),0,'f',2));
			p.drawText(rtRight,Qt::AlignRight|Qt::AlignVCenter,QString("%1").arg(m_pStockItem->getLTSZ()));

			iCurY += m_iItemHeight;
		}
		{
			//���棨������PE������
			QRect rtLeft(rtInfo.left()+2,iCurY,iMidX-3,m_iItemHeight);
			QRect rtRight(iMidX+2,iCurY,rtLeft.width(),m_iItemHeight);
			p.setPen(m_clrDesc);
			p.drawText(rtLeft,Qt::AlignLeft|Qt::AlignVCenter,tr("����(��)"));
			p.drawText(rtRight,Qt::AlignLeft|Qt::AlignVCenter,tr("PE(��)"));

			p.drawText(rtLeft,Qt::AlignRight|Qt::AlignVCenter,QString("%1").arg(m_pStockItem->getMgsy(),0,'f',3));
			p.drawText(rtRight,Qt::AlignRight|Qt::AlignVCenter,QString("%1").arg(m_pStockItem->getPERatio(),0,'f',2));

			iCurY += m_iItemHeight;
		}

	}
}

void CStockInfoWidget::keyPressEvent( QKeyEvent* e )
{
	int iKey = e->key();
	if(Qt::Key_F10 == iKey)
	{
		//F10����
		if(m_pStockItem)
		{
			if(!CDataEngine::getDataEngine()->showF10Data(m_pStockItem->getCode()))
			{
				//δ��F10���� do something
			}
		}
		e->accept();
	}
	else
	{
		return CBaseWidget::keyPressEvent(e);
	}
}

QMenu* CStockInfoWidget::getCustomMenu()
{
	QAction* pAction = m_pMenu->menuAction();
	if(!m_pMenuCustom->actionGeometry(pAction).isValid())
		m_pMenuCustom->addMenu(m_pMenu);

	return m_pMenuCustom;
}

void CStockInfoWidget::onSetStockCode()
{
	QDialog dlg(this);
	QVBoxLayout layout(this);
	QLineEdit edit(this);
	QPushButton btnOk(this);
	dlg.setLayout(&layout);
	layout.addWidget(&edit);
	layout.addWidget(&btnOk);
	btnOk.setText(tr("ȷ��"));
	connect(&btnOk,SIGNAL(clicked()),&dlg,SLOT(accept()));

	if(QDialog::Accepted != dlg.exec())
		return;

	QString code = edit.text();
	setStockCode(code);
}

void CStockInfoWidget::getKeyWizData( const QString& keyword,QList<KeyWizData*>& listRet )
{
	foreach(CStockInfoItem* pItem,CDataEngine::getDataEngine()->getStockInfoList())
	{
		if(pItem->isMatch(keyword))
		{
			KeyWizData* pData = new KeyWizData;
			pData->cmd = CKeyWizard::CmdStock;
			pData->arg = pItem;
			pData->desc = QString("%1 %2").arg(pItem->getCode()).arg(pItem->getName());
			listRet.push_back(pData);
			if(listRet.size()>20)
				return;
		}
	}

	return CBaseWidget::getKeyWizData(keyword,listRet);
}

void CStockInfoWidget::keyWizEntered( KeyWizData* pData )
{
	if(pData->cmd == CKeyWizard::CmdStock)
	{
		setStockItem(reinterpret_cast<CStockInfoItem*>(pData->arg));
		return;
	}

	return CBaseWidget::keyWizEntered(pData);
}
