#include "StdAfx.h"
#include "RadarWatcher.h"
#include "StockInfoItem.h"
#include "BlockInfoItem.h"
#include "DataEngine.h"
#include <QtXml>

CRadarManager* CRadarManager::m_pSelf = 0;

CRadarManager* CRadarManager::getRadarManager()
{
	if(m_pSelf==0)
		m_pSelf = new CRadarManager();

	return m_pSelf;
}

QString CRadarManager::getTypeName( RadarType _t )
{
	switch(_t)
	{
	case BigVolumn:		//��ɽ���
		return QString("�ɽ���");
		break;
	case BigIncrease:	//�Ƿ�
		return QString("�Ƿ�");
		break;
	case MaxPrice:		//���¸�
		return QString("���¸�");
		break;
	case MinPrice:		//���µ�
		return QString("���µ�");
		break;
	}

	return QString("δ֪");
}

CRadarWatcher* CRadarManager::createRadarWatcher( CBlockInfoItem* pBlock, RadarType _t,int iSec,float _hold,CBlockInfoItem* pDestBlock/*=NULL*/,int iId/*=-1*/ )
{
	QList<int> listKey = m_listWatchers.keys();
	int iMaxId = iId;
	if(iMaxId<0)
	{
		iMaxId = 0;
		//�������µ�id
		foreach(int i,listKey)
		{
			if(i>iMaxId)
				iMaxId = i;
		}
		++iMaxId;
	}

	CRadarWatcher* pWatcher = NULL;
	switch(_t)
	{
	case BigVolumn:
		pWatcher = new CVolumnWatcher(iMaxId,pBlock,iSec,_hold,pDestBlock);
		break;
	case BigIncrease:
		pWatcher = new CIncreaseWatcher(iMaxId,pBlock,iSec,_hold,pDestBlock);
		break;
	case MaxPrice:
		pWatcher = new CMaxPriceWatcher(iMaxId,pBlock,iSec,_hold,pDestBlock);
		break;
	case MinPrice:
		pWatcher = new CMinPriceWatcher(iMaxId,pBlock,iSec,_hold,pDestBlock);
		break;
	}

	if(pWatcher)
	{
		m_listWatchers[pWatcher->getId()] = pWatcher;
	}

	return pWatcher;
}

QList<CRadarWatcher*> CRadarManager::getRadarWatchers()
{
	return m_listWatchers.values();
}

CRadarWatcher* CRadarManager::getWatcher( const int& _id )
{
	if(m_listWatchers.contains(_id))
		return m_listWatchers[_id];

	return 0;
}

void CRadarManager::removeWatcher( const int& _id )
{
	if(m_listWatchers.contains(_id))
	{
		CRadarWatcher* pWatcher = m_listWatchers[_id];
		m_listWatchers.remove(_id);
		if(pWatcher)
		{
			delete pWatcher;
		}
	}
}

void CRadarManager::appendRadar( RRadarData* pRadar )
{
	m_listRadar.append(pRadar);
	emit radarAlert(pRadar);
}

void CRadarManager::loadRadars()
{
	QFile file(qApp->applicationDirPath()+"/config/radars.xml");
	if(!file.open(QFile::ReadOnly))
		return;
	QDomDocument doc;
	doc.setContent(file.readAll());
	file.close();

	QDomElement eleRoot = doc.firstChildElement("ROOT");
	if(!eleRoot.isElement())
		return;
	QDomElement eleRadar = eleRoot.firstChildElement("radar");
	while(eleRadar.isElement())
	{
		int iId = eleRadar.attribute("id").toInt();
		RadarType iType = static_cast<RadarType>(eleRadar.attribute("type").toInt());
		float fHold = eleRadar.attribute("hold").toFloat();
		int iSec = eleRadar.attribute("sec").toInt();
		QString qsCode = eleRadar.attribute("block");
		CBlockInfoItem* pBlock = CDataEngine::getDataEngine()->getStockBlock(qsCode);
		QString qsDestCode = eleRadar.attribute("dest");
		CBlockInfoItem* pDestBlock = CDataEngine::getDataEngine()->getStockBlock(qsDestCode);

		createRadarWatcher(pBlock,iType,iSec,fHold,pDestBlock,iId);

		eleRadar = eleRadar.nextSiblingElement("radar");
	}
}

void CRadarManager::saveRadars()
{
	QDomDocument doc("template");
	QDomElement eleRoot = doc.createElement("ROOT");
	doc.appendChild(eleRoot);
	QMap<int,CRadarWatcher*>::iterator iter = m_listWatchers.begin();
	while (iter!=m_listWatchers.end())
	{
		CRadarWatcher* pWatcher = iter.value();
		QDomElement eleRadar = doc.createElement("radar");
		eleRadar.setAttribute("id",iter.key());
		eleRadar.setAttribute("type",pWatcher->getType());
		eleRadar.setAttribute("hold",pWatcher->getHold());
		eleRadar.setAttribute("sec",pWatcher->getSec());
		CBlockInfoItem* pBlock = pWatcher->getBlock();
		eleRadar.setAttribute("block",pBlock==0 ? "" : pBlock->getOnly());

		CBlockInfoItem* pDestBlock = pWatcher->getDestBlock();
		eleRadar.setAttribute("dest",pDestBlock==0 ? "" : pDestBlock->getOnly());

		eleRoot.appendChild(eleRadar);
		++iter;
	}

	QFile file(qApp->applicationDirPath()+"/config/radars.xml");
	if(!file.open(QFile::Truncate|QFile::WriteOnly))
	{
		return;
	}
	file.write(doc.toByteArray());
	file.close();
}

void CRadarManager::releaseRadars()
{
	QMap<int,CRadarWatcher*>::iterator iter = m_listWatchers.begin();
	while (iter!=m_listWatchers.end())
	{
		delete iter.value();
		++iter;
	}
	m_listWatchers.clear();

	foreach(RRadarData* pData,m_listRadar)
	{
		delete pData;
	}
	m_listRadar.clear();
	delete m_pSelf;
	m_pSelf = NULL;
}



CRadarWatcher::CRadarWatcher( int _id,CBlockInfoItem* pBlock,RadarType _t,int iSec,float _hold,CBlockInfoItem* pDestBlock )
	: m_id(_id)
	, m_pWatcherBlock(pBlock)
	, m_type(_t)
	, m_iSec(iSec)
	, m_fHold(_hold)
	, m_pDestBlock(pDestBlock)
{
	if(m_pWatcherBlock)
	{
		//���ü��ӵĹ�Ʊ�б�
		QList<CStockInfoItem*> list = m_pWatcherBlock->getStockList();
		foreach(CStockInfoItem* pItem,list)
		{
			connect(pItem,SIGNAL(stockItemReportComing(CStockInfoItem*)),this,SLOT(onStockReportComing(CStockInfoItem*)));
		}
	}
}


CRadarWatcher::~CRadarWatcher(void)
{
}

void CRadarWatcher::setBlock( CBlockInfoItem* _b )
{
	if(m_pWatcherBlock==_b)
		return;

	if(m_pWatcherBlock)
	{
		//ɾ��֮ǰ���ӵĹ�Ʊ�б�
		QList<CStockInfoItem*> list = m_pWatcherBlock->getStockList();
		foreach(CStockInfoItem* pItem,list)
		{
			disconnect(pItem,SIGNAL(stockItemReportComing(CStockInfoItem*)),this,SLOT(onStockReportComing(CStockInfoItem*)));
		}
	}

	m_pWatcherBlock = _b;

	if(m_pWatcherBlock)
	{
		//�����¼��ӵĹ�Ʊ�б�
		QList<CStockInfoItem*> list = m_pWatcherBlock->getStockList();
		foreach(CStockInfoItem* pItem,list)
		{
			connect(pItem,SIGNAL(stockItemReportComing(CStockInfoItem*)),this,SLOT(onStockReportComing(CStockInfoItem*)));
		}
	}
}

void CRadarWatcher::setDestBlock( CBlockInfoItem* _b )
{
	m_pDestBlock = _b;
}



CVolumnWatcher::CVolumnWatcher( int _id,CBlockInfoItem* pBlock,int iSec,float _hold,CBlockInfoItem* pDestBlock )
	: CRadarWatcher(_id,pBlock,BigVolumn,iSec,_hold,pDestBlock)
{

}

CVolumnWatcher::~CVolumnWatcher( void )
{
}

void CVolumnWatcher::onStockReportComing( CStockInfoItem* pItem )
{
	qRcvReportData* pReport = pItem->getCurrentReport();
	if(!pReport)
		return;

	tagVolumnData* pData = m_mapVolume[pItem];
	if(pData)
	{
		if(_isnan(pData->fLastV))
		{
			if((pReport->tmTime-pData->tmTime)>m_iSec)
			{
				pData->tmTime = pReport->tmTime;
				pData->fLastV = pData->fNewV;
				pData->fNewV = pReport->fVolume;
			}
		}
		else
		{
			//��ȥһ�����ڵĳɽ���
			float fLastV = pData->fNewV-pData->fLastV;
			float fNewV = pReport->fVolume-pData->fNewV;
			if((fNewV-fLastV)/fLastV > m_fHold)
			{
				//������ֵ�������׳�����
				RRadarData* pRadar = new RRadarData;
				pRadar->pStock = pItem;
				pRadar->tmTime = pReport->tmTime;
				pRadar->tpType = BigVolumn;
				pRadar->qsDesc = QString("��ʳɽ������֣�������һ����:%1").arg((fNewV-fLastV)/fLastV);
				pRadar->iWatcher = m_id;
				if(m_pDestBlock)
				{
					m_pDestBlock->addStockInfo(pItem);
				}
				CRadarManager::getRadarManager()->appendRadar(pRadar);
			}

			if((pReport->tmTime-pData->tmTime)>m_iSec)
			{
				pData->tmTime = pReport->tmTime;
				pData->fLastV = pData->fNewV;
				pData->fNewV = pReport->fVolume;
			}
		}
	}
	else
	{
		pData = new tagVolumnData();
		pData->tmTime = pReport->tmTime;
		pData->fLastV = FLOAT_NAN;
		pData->fNewV = pReport->fVolume;
		m_mapVolume[pItem] = pData;
	}
}

CIncreaseWatcher::CIncreaseWatcher( int _id,CBlockInfoItem* pBlock,int iSec,float _hold,CBlockInfoItem* pDestBlock )
	: CRadarWatcher(_id,pBlock,BigIncrease,iSec,_hold,pDestBlock)
{

}

CIncreaseWatcher::~CIncreaseWatcher( void )
{

}

void CIncreaseWatcher::onStockReportComing( CStockInfoItem* pItem )
{
	qRcvReportData* pReport = pItem->getCurrentReport();
	if(!pReport)
		return;

	tagPriceData* pData = m_mapPrice[pItem];
	if(pData)
	{
		//��ȥһ�����ڵ���߼�
		if(pData->fMaxPrice>0.0)
		{
			if((pReport->fNewPrice-pData->fMaxPrice)/pData->fMaxPrice > m_fHold)
			{
				//������ֵ�������׳�����
				RRadarData* pRadar = new RRadarData;
				pRadar->pStock = pItem;
				pRadar->tmTime = pReport->tmTime;
				pRadar->tpType = BigIncrease;
				pRadar->qsDesc = QString("��ĳɽ��۳��֣�������һ����:%1").arg((pReport->fNewPrice-pData->fMaxPrice)/pData->fMaxPrice);
				pRadar->iWatcher = m_id;
				if(m_pDestBlock)
				{
					m_pDestBlock->addStockInfo(pItem);
				}
				CRadarManager::getRadarManager()->appendRadar(pRadar);
			}
		}

		if((pReport->tmTime-pData->tmTime)>m_iSec)
		{
			pData->tmTime = pReport->tmTime;
			pData->fMaxPrice = pData->fNewPrice;
			pData->fNewPrice = pReport->fNewPrice;
		}
		else if(pReport->fNewPrice>pData->fNewPrice)
		{
			//ȡ�������е����ֵ��
			pData->fNewPrice = pReport->fNewPrice;
		}
	}
	else
	{
		pData = new tagPriceData();
		pData->tmTime = pReport->tmTime;
		pData->fMaxPrice = pReport->fNewPrice;
		pData->fNewPrice = pReport->fNewPrice;
		m_mapPrice[pItem] = pData;
	}
}


CMaxPriceWatcher::CMaxPriceWatcher( int _id,CBlockInfoItem* pBlock,int iSec,float _hold,CBlockInfoItem* pDestBlock )
	: CRadarWatcher(_id,pBlock,MaxPrice,iSec,_hold,pDestBlock)
{

}

CMaxPriceWatcher::~CMaxPriceWatcher( void )
{

}

void CMaxPriceWatcher::onStockReportComing( CStockInfoItem* pItem )
{
	qRcvReportData* pReport = pItem->getCurrentReport();
	if(!pReport)
		return;

	tagPriceData* pData = m_mapPrice[pItem];
	if(pData)
	{
		if((pReport->tmTime-pData->tmTime)>m_iSec)
		{
			pData->tmTime = pReport->tmTime;
			pData->fMaxPrice = pData->fNewPrice;
			pData->fNewPrice = pReport->fNewPrice;
		}

		if(!_isnan(pData->fMaxPrice))
		{
			//��ȥһ�����ڵ���߼�
			if(pReport->fNewPrice > pData->fMaxPrice && pReport->fNewPrice > pData->fNewPrice)
			{
				//������ֵ�������׳�����
				RRadarData* pRadar = new RRadarData;
				pRadar->pStock = pItem;
				pRadar->tmTime = pReport->tmTime;
				pRadar->tpType = MaxPrice;
				pRadar->qsDesc = QString("���¸ߣ��ɽ���:%1").arg(pReport->fNewPrice);
				pRadar->iWatcher = m_id;
				if(m_pDestBlock)
				{
					m_pDestBlock->addStockInfo(pItem);
				}
				CRadarManager::getRadarManager()->appendRadar(pRadar);
			}
		}
		if(pReport->fNewPrice>pData->fNewPrice)
		{
			pData->fNewPrice = pReport->fNewPrice;
		}
	}
	else
	{
		pData = new tagPriceData();
		pData->tmTime = pReport->tmTime;
		pData->fMaxPrice = FLOAT_NAN;
		pData->fNewPrice = pReport->fNewPrice;
		m_mapPrice[pItem] = pData;
	}
}

CMinPriceWatcher::CMinPriceWatcher( int _id,CBlockInfoItem* pBlock,int iSec,float _hold,CBlockInfoItem* pDestBlock )
	: CRadarWatcher(_id,pBlock,MinPrice,iSec,_hold,pDestBlock)
{

}

CMinPriceWatcher::~CMinPriceWatcher( void )
{

}

void CMinPriceWatcher::onStockReportComing( CStockInfoItem* pItem )
{
	qRcvReportData* pReport = pItem->getCurrentReport();
	if(!pReport)
		return;

	tagPriceData* pData = m_mapPrice[pItem];
	if(pData)
	{
		if((pReport->tmTime-pData->tmTime)>m_iSec)
		{
			pData->tmTime = pReport->tmTime;
			pData->fMinPrice = pData->fNewPrice;
			pData->fNewPrice = pReport->fNewPrice;
		}

		if(!_isnan(pData->fMinPrice))
		{
			//��ȥһ�����ڵ���߼�
			if((pReport->fNewPrice < pData->fMinPrice) && (pReport->fNewPrice < pData->fNewPrice))
			{
				//������ֵ�������׳�����
				RRadarData* pRadar = new RRadarData;
				pRadar->pStock = pItem;
				pRadar->tmTime = pReport->tmTime;
				pRadar->tpType = MinPrice;
				pRadar->qsDesc = QString("���¸ߣ��ɽ���:%1").arg(pReport->fNewPrice);
				pRadar->iWatcher = m_id;
				if(m_pDestBlock)
				{
					m_pDestBlock->addStockInfo(pItem);
				}
				CRadarManager::getRadarManager()->appendRadar(pRadar);
			}
		}
		if(pReport->fNewPrice<pData->fNewPrice)
		{
			pData->fNewPrice = pReport->fNewPrice;
		}
	}
	else
	{
		pData = new tagPriceData();
		pData->tmTime = pReport->tmTime;
		pData->fMinPrice = FLOAT_NAN;
		pData->fNewPrice = pReport->fNewPrice;
		m_mapPrice[pItem] = pData;
	}
}
