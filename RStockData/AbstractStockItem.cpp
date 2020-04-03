#include "StdAfx.h"
#include "AbstractStockItem.h"
#include "StockInfoItem.h"
#include "BlockInfoItem.h"
#include "DataEngine.h"

tagRStockData* getNMinDataByMin(QMap<time_t,tagRStockData*>& mapMins, QList<tagRStockData*>& listNMin, tagRStockData* pLastMin, tagRStockData* pCurMin, RStockCircle _c)
{
	if(listNMin.size()>0)
	{
		if(listNMin.last()->tmTime != pLastMin->tmTime)
		{
			//ɾ��֮ǰ������
			foreach(tagRStockData* pData,listNMin)
				delete pData;
			listNMin.clear();
		}
	}

	if(listNMin.size() == 0)
	{
		//���¼�������
		RStockData* pData = NULL;
		QMap<time_t,tagRStockData*>::iterator iter = mapMins.begin();
		while(iter!=mapMins.end())
		{
			if(pData == NULL)
			{
				pData = new RStockData(**iter);
			}
			else if((int)(pData->tmTime/_c) == (int)((*iter)->tmTime/_c))
			{
				pData->tmTime = (*iter)->tmTime;
				if(pData->fHigh<(*iter)->fHigh)
					pData->fHigh = (*iter)->fHigh;
				if(pData->fLow>(*iter)->fLow)
					pData->fLow = (*iter)->fLow;
				pData->fClose = (*iter)->fClose;
				pData->fVolume += (*iter)->fVolume;
				pData->fAmount += (*iter)->fAmount;
			}
			else
			{
				listNMin.append(pData);
				pData = new RStockData(**iter);
			}

			++iter;
		}

		if(pData)
		{
			if((int)(pData->tmTime/_c) == (int)(pCurMin->tmTime/_c))
			{
				pData->tmTime = pCurMin->tmTime;
				if(pData->fHigh<pCurMin->fHigh)
					pData->fHigh = pCurMin->fHigh;
				if(pData->fLow>pCurMin->fLow)
					pData->fLow = pCurMin->fLow;
				pData->fClose = pCurMin->fClose;
				pData->fVolume += pCurMin->fVolume;
				pData->fAmount += pCurMin->fAmount;

				return pData;
			}
			else
			{
				listNMin.append(pData);
				return new tagRStockData(*pCurMin);
			}
		}
		else
			return new tagRStockData(*pCurMin);
	}
	else
	{
		QMap<time_t,tagRStockData*>::iterator iter = mapMins.find(listNMin.last()->tmTime);
		if(iter==mapMins.end())
			return new tagRStockData(*pCurMin);
		else
		{
			++iter;
			tagRStockData* pData = NULL;
			while(iter!=mapMins.end())
			{
				if(pData == NULL)
				{
					pData = new RStockData(**iter);
				}
				else
				{
					pData->tmTime = (*iter)->tmTime;
					if(pData->fHigh<(*iter)->fHigh)
						pData->fHigh = (*iter)->fHigh;
					if(pData->fLow>(*iter)->fLow)
						pData->fLow = (*iter)->fLow;
					pData->fClose = (*iter)->fClose;
					pData->fVolume += (*iter)->fVolume;
					pData->fAmount += (*iter)->fAmount;
				}
				++iter;
			}

			if(pData)
			{
				if((int)(pData->tmTime/_c) == (int)(pCurMin->tmTime/_c))
				{
					pData->tmTime = pCurMin->tmTime;
					if(pData->fHigh<pCurMin->fHigh)
						pData->fHigh = pCurMin->fHigh;
					if(pData->fLow>pCurMin->fLow)
						pData->fLow = pCurMin->fLow;
					pData->fClose = pCurMin->fClose;
					pData->fVolume += pCurMin->fVolume;
					pData->fAmount += pCurMin->fAmount;

					return pData;
				}
				else
				{
					listNMin.append(pData);
					return new tagRStockData(*pCurMin);
				}
			}
			else
				return new tagRStockData(*pCurMin);
		}
	}
}

CAbstractStockItem::CAbstractStockItem(void)
	: pCurrentReport(NULL)
	, fLast5Volume(0.0)
	, pCurrent5Min(NULL)
	, pCurrent15Min(NULL)
	, pCurrent30Min(NULL)
	, pCurrent60Min(NULL)
	, m_bLoadHisMin(false)
{
	pCurrentMin = new RStockData();
}


CAbstractStockItem::~CAbstractStockItem(void)
{
	{
		//��շֱ�����
		QMap<time_t,qRcvFenBiData*>::iterator iter = mapFenBis.begin();
		while(iter!=mapFenBis.end())
		{
			delete iter.value();
			++iter;
		}
		mapFenBis.clear();
	}

	{
		//��ս���1��������
		QMap<time_t,RStockData*>::iterator iter = mapMinDatas.begin();
		while(iter!=mapMinDatas.end())
		{
			delete iter.value();
			++iter;
		}
		mapMinDatas.clear();
	}

	{
		//��ս���5��������
		QMap<time_t,RStockData*>::iterator iter = mapToday5MinDatas.begin();		//5������ʷ����
		while(iter!=mapToday5MinDatas.end())
		{
			delete iter.value();
			++iter;
		}
		mapToday5MinDatas.clear();
	}

	{
		//�����ʷ1��������
		QMap<time_t,RStockData*>::iterator iter = mapMinDatas.begin();		//5������ʷ����
		while(iter!=mapMinDatas.end())
		{
			delete iter.value();
			++iter;
		}
		mapMinDatas.clear();
	}

	{
		//�����������
		foreach(qRcvHistoryData* pData,listHistories)
		{
			delete pData;
		}
		listHistories.clear();
	}
}

void CAbstractStockItem::initStockItem()
{
	//��ȡ��ȥ5�յĳɽ����������ڼ������ȵ���Ϣ
	QList<qRcvHistoryData*> list = CDataEngine::getDataEngine()->getHistoryList(this,5);
	foreach(qRcvHistoryData* pHis,list)
	{
		fLast5Volume = fLast5Volume+pHis->fVolume;
		delete pHis;
	}
	list.clear();
}

qRcvReportData* CAbstractStockItem::getCurrentReport() const
{
	return pCurrentReport;
}

QList<qRcvFenBiData*> CAbstractStockItem::getTodayFenBiList()
{
	QList<qRcvFenBiData*> listRet;
	time_t tmBegin = (((CDataEngine::getCurrentTime())/(3600*24))*3600*24);
	QMap<time_t,qRcvFenBiData*>::iterator iter = mapFenBis.lowerBound(tmBegin);
	while(iter!=mapFenBis.end())
	{
		listRet.append(*iter);
		++iter;
	}
	return listRet;
}

QList<qRcvFenBiData*> CAbstractStockItem::getFenBiList()
{
	//��ȡ�ֱ����ݣ�δ�깤
	return mapFenBis.values();
}

QList<qRcvHistoryData*> CAbstractStockItem::getHistoryList()
{
	if(listHistories.size()<1)
	{
		listHistories = CDataEngine::getDataEngine()->getHistoryList(this);
	}
	return listHistories;
}

QList<qRcvHistoryData*> CAbstractStockItem::getLastHistory( int count )
{
	if(listHistories.size()<1)
	{
		listHistories = CDataEngine::getDataEngine()->getHistoryList(this);
	}
	if(listHistories.size()>count)
	{
		return listHistories.mid(listHistories.size()-count);
	}
	return listHistories;
}

void CAbstractStockItem::appendHistorys( const QList<qRcvHistoryData*>& list )
{
	QList<qRcvHistoryData*> listHistory;
	int iCountFromFile = -1;
	bool bRemoveList = true;		//�Ƿ����ɾ��listHistory�е�����

	if(list.size()>130)
	{
		//��ȡ������ʷ����
		bRemoveList = false;
		listHistory = getHistoryList();
	}
	else
	{
		//����ȡ5������
		iCountFromFile = list.size();
		if(iCountFromFile<5)
			iCountFromFile = 5;
		listHistory = CDataEngine::getDataEngine()->getHistoryList(this,iCountFromFile);
	}

	QMap<time_t,qRcvHistoryData*> mapHistorys;		//��������
	foreach(qRcvHistoryData* p,listHistory)
	{
		if(mapHistorys.contains(p->time))
		{
			qRcvHistoryData* pBefore = mapHistorys[p->time];
			if(pBefore!=p)
				delete pBefore;
		}
		mapHistorys[p->time] = p;
	}
	foreach(qRcvHistoryData* p,list)
	{
		if(mapHistorys.contains(p->time))
		{
			qRcvHistoryData* pBefore = mapHistorys[p->time];
			if(pBefore!=p)
				delete pBefore;
		}
		mapHistorys[p->time] = p;
	}


	listHistory.clear();
	listHistory = mapHistorys.values();
	{
		//���5�յ�ȫ���ɽ������������ȵļ��㣩
		fLast5Volume = 0.0;
		for (int i=1;i<=5;++i)
		{
			int iIndex = listHistory.size()-i;
			if(iIndex<0)
				break;
			fLast5Volume = fLast5Volume + listHistory[iIndex]->fVolume;
		}
		updateItemInfo();
	}

	CDataEngine::getDataEngine()->exportHistoryData(this,listHistory,iCountFromFile);

	if(bRemoveList)
	{
		QMap<time_t,qRcvHistoryData*>::iterator iter = mapHistorys.begin();
		while(iter!=mapHistorys.end())
		{
			delete iter.value();
			++iter;
		}
		mapHistorys.clear();
	}
	else
	{
		listHistories = listHistory;
	}

	emit stockItemHistoryChanged(qsOnly);
}


void CAbstractStockItem::appendMinData( tagRStockData* pData )
{
	if(mapMinDatas.contains(pData->tmTime))
	{
		tagRStockData* pBefore = mapMinDatas[pData->tmTime];
		mapMinDatas.remove(pBefore->tmTime);
		delete pBefore;
	}
	mapMinDatas.insert(pData->tmTime,pData);
	return;
}

void CAbstractStockItem::appendToday5MinData( tagRStockData* pData )
{
	if(mapToday5MinDatas.contains(pData->tmTime))
	{
		tagRStockData* pBefore = mapToday5MinDatas[pData->tmTime];
		mapToday5MinDatas.remove(pBefore->tmTime);
		delete pBefore;
	}
	mapToday5MinDatas.insert(pData->tmTime,pData);
	return;
}

void CAbstractStockItem::appendFenBis( const QList<qRcvFenBiData*>& list )
{
	{
		//���ɾ������ķֱ����ݣ��ѷ�����
		//if(mapFenBis.size()>0&&list.size()>0)
		//{
		//	QDate myDate = QDateTime::fromTime_t(mapFenBis.begin().value()->tmTime).date();
		//	QDate theDate = QDateTime::fromTime_t(list.first()->tmTime).date();
		//	if(theDate>myDate)
		//	{
		//		//������������ݵ�ʱ����ڵ�ǰ��ʱ�䣬����յ�ǰ������
		//		foreach(qRcvFenBiData* p,mapFenBis.values())
		//			delete p;

		//		mapFenBis.clear();
		//	}
		//}
	}

	//׷�ӷֱ�����
	foreach(qRcvFenBiData* p,list)
	{
		bool bInsert = true;
		if(mapFenBis.contains(p->tmTime))
		{
			QList<qRcvFenBiData*> listV = mapFenBis.values(p->tmTime);
			foreach(qRcvFenBiData* p1,listV)
			{
				if(p1->fVolume == p->fVolume)
				{
					if(p1->fBuyPrice[0]>0.0)
					{
						//���֮ǰ�����ݲ������ҵ����ݣ���ɾ��֮ǰ��
						delete p1;
						mapFenBis.remove(p->tmTime);
						break;
					}
					else
					{
						//ɾ�����ڵ�����
						bInsert = false;
						delete p;
						break;
					}
				}
			}
		}
		if(bInsert)
			mapFenBis.insert(p->tmTime,p);
	}

	emit stockItemFenBiChanged(qsOnly);
}

QList<tagRStockData*> CAbstractStockItem::getMinList()
{
	loadHisMinData();
	QList<tagRStockData*> listMins = mapMinDatas.values();
	
	if(pCurrentMin->tmTime>0)
	{
		listMins.append(pCurrentMin);
	}

	return listMins;
}

QList<tagRStockData*> CAbstractStockItem::getMinList( int _c )
{
	if(_c>Min60)
		return QList<tagRStockData*>();

	/*
	Min5 = 5*60,			//5����
	Min15 = 15*60,			//15����
	Min30 = 30*60,			//30����
	Min60 = 60*60,			//60����
	*/
	loadHisMinData();

	QList<tagRStockData*> listRet;
	RStockData* pLastMin = 0;
	if(mapMinDatas.size()>0)
	{
		pLastMin = (mapMinDatas.end()-1).value();
	}
	else
	{
		return QList<tagRStockData*>()<<pCurrentMin;
	}

	if(_c<=Min1)
	{
		listRet = mapMinDatas.values();

		//׷����󲻹�1���ӵ�����
		if(pCurrentMin->tmTime>0)
		{
			listRet.append(pCurrentMin);
		}
	}
	else if(_c==Min5)
	{
		if(pCurrent5Min)
			delete pCurrent5Min;

		pCurrent5Min = getNMinDataByMin(mapMinDatas,list5MinDatas,pLastMin,pCurrentMin,Min5);
		listRet = list5MinDatas;
		if(pCurrent5Min->tmTime>0)
			listRet.append(pCurrent5Min);
	}
	else if(_c==Min15)
	{
		if(pCurrent15Min)
			delete pCurrent15Min;

		pCurrent15Min = getNMinDataByMin(mapMinDatas,list15MinDatas,pLastMin,pCurrentMin,Min15);
		listRet = list15MinDatas;
		if(pCurrent15Min->tmTime>0)
			listRet.append(pCurrent15Min);
	}
	else if(_c==Min30)
	{
		if(pCurrent30Min)
			delete pCurrent30Min;

		pCurrent30Min = getNMinDataByMin(mapMinDatas,list30MinDatas,pLastMin,pCurrentMin,Min30);
		listRet = list30MinDatas;
		if(pCurrent30Min->tmTime>0)
			listRet.append(pCurrent30Min);
	}
	else if(_c==Min60)
	{
		if(pCurrent60Min)
			delete pCurrent60Min;

		pCurrent60Min = getNMinDataByMin(mapMinDatas,list60MinDatas,pLastMin,pCurrentMin,Min60);
		listRet = list60MinDatas;
		if(pCurrent60Min->tmTime>0)
			listRet.append(pCurrent60Min);
	}

	return listRet;
}


QString CAbstractStockItem::getOnly() const
{
	return qsOnly;
}

QString CAbstractStockItem::getCode() const
{
	return qsCode;
}

WORD CAbstractStockItem::getMarket() const
{
	return wMarket;
}

QString CAbstractStockItem::getMarketName() const
{
	return qsMarket;
}


bool CAbstractStockItem::isInstanceOfStock()
{
	CStockInfoItem* p = dynamic_cast<CStockInfoItem*>(this);
	return p ? true : false;
}

bool CAbstractStockItem::isInstanceOfBlock()
{
	CBlockInfoItem* p = dynamic_cast<CBlockInfoItem*>(this);
	return p ? true : false;
}

int CAbstractStockItem::getNewHighPriceCount()
{
	int iCount = 0;

	QMap<time_t,qRcvFenBiData*>::iterator iter=mapFenBis.begin();
	float fHighPrice = getOpenPrice();

	float fPrice = fHighPrice;
	int iTime = 0;
	while (iter!=mapFenBis.end())
	{
		int iT = (iter.key())/60;
		if(iT>iTime)
		{
			if(fPrice>fHighPrice)
			{
				iCount++;
				fHighPrice = fPrice;
			}
			iTime = iT;
		}
		fPrice = (*iter)->fPrice;

		++iter;
	}

	return iCount;
}

int CAbstractStockItem::getNewHighVolumeCount()
{
	int iCount = 0;

	QMap<time_t,qRcvFenBiData*>::iterator iter=mapFenBis.begin();
	if(iter==mapFenBis.end())
		return iCount;

	qRcvFenBiData* pLast = 0;
	qRcvFenBiData* pCur = 0;

	float fHighVolume = 0;
	int iTime = iter.key()/60;

	while (iter!=mapFenBis.end())
	{
		int iT = (iter.key())/60;
		if(iT>iTime)
		{
			if(pLast && pCur)
			{
				if((pCur->fVolume-pLast->fVolume)>fHighVolume)
				{
					iCount++;
					fHighVolume = (pCur->fVolume-pLast->fVolume);
				}
			}
			else if(pCur)
			{
				fHighVolume = pCur->fVolume;
			}
			else
			{
				fHighVolume = (*iter)->fVolume;
			}
			pLast = pCur;
			iTime = iT;
		}

		pCur = *iter;
		++iter;
	}

	return iCount;
}

RStockData* CAbstractStockItem::getToday5MinData( const time_t& tmTime )
{
	if(mapToday5MinDatas.contains(tmTime))
		return mapToday5MinDatas[tmTime];

	return 0;
}

void CAbstractStockItem::loadHisMinData()
{
	if(m_bLoadHisMin==false)
	{
		CDataEngine::getDataEngine()->importMinData(this,mapMinDatas);
		m_bLoadHisMin = true;
	}
}
