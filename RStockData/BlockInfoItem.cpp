/************************************************************************/
/* �ļ����ƣ�BlockInfoItem.cpp
/* ����ʱ�䣺2013-01-08 09:28
/*
/* ��    �������ڹ���������
/*
/* ���¼������ʱ��Ϊ30�����һ��
/* ���㷽ʽ��ȡǰʮֻ��Ʊ���м�Ȩƽ��
/************************************************************************/
#include "StdAfx.h"
#include "BlockInfoItem.h"
#include "DataEngine.h"
#include "BlockCodeManager.h"
#include "Hz2Py.h"
#define	MAX_STOCK_IN_BLOCK	3000
#define UPDATE_BLOCK_TIME	10*1000

CBlockInfoItem::CBlockInfoItem( const QString& _file,CBlockInfoItem* parent/*=0*/ )
	: fIncrease(FLOAT_NAN)
	, fLTSZ(FLOAT_NAN)
	, fLTG(FLOAT_NAN)
	, fLastClose(FLOAT_NAN)
	, fOpenPrice(FLOAT_NAN)
	, fNewPrice(FLOAT_NAN)
	, fLowPrice(FLOAT_NAN)
	, fHighPrice(FLOAT_NAN)
	, m_pParent(parent)
	, blockFilePath(_file)
	, fLastMinVolume(0)
	, fLastMinAmount(0)
	, fLast5MinVolume(0)
	, fLast5MinAmount(0)
{
	pCurrentReport = new qRcvReportData;
	pCurrent5Min = new RBlockData;

	QFileInfo _info(blockFilePath);
	if(!_info.exists())
		return;
	blockFilePath = _file;
	blockName = _info.baseName();

	//���´ʿ���еļ�ƴ
	shortName = CHz2Py::getHzFirstLetter(blockName);


	/*���ð����롢�г����͡�Ψһ��ʶ*/
	qsCode = CBlockCodeManager::getBlockCode(getAbsPath());
	wMarket = BB_MARKET_EX;
	qsMarket = CDataEngine::getMarketStr(wMarket);
	qsOnly = qsCode+qsMarket;

	//��ʼ��Report����
	qRcvReportData* pReport = CDataEngine::getReportForInitBlock(qsOnly);
	if(pReport)
	{
		pCurrentReport->tmTime = pReport->tmTime;
		pCurrentReport->qsCode = qsCode;
		pCurrentReport->wMarket = BB_MARKET_EX;
		memcpy(&(pCurrentReport->fLastClose),&(pReport->fLastClose),sizeof(float)*27);

		fNewPrice = pCurrentReport->fNewPrice;
		fOpenPrice = pCurrentReport->fOpen;
		fHighPrice = pCurrentReport->fHigh;
		fLowPrice = pCurrentReport->fLow;
		fLastClose = pCurrentReport->fLastClose;
		fVolume = pCurrentReport->fVolume;
		fAmount = pCurrentReport->fAmount;
		if(fNewPrice>0.0 && fLastClose>0.0)
			fIncrease = (fNewPrice-fLastClose)*100.0/fLastClose;
	}
	else
	{
		pCurrentReport->tmTime = CDataEngine::getCurrentTime();
		pCurrentReport->qsCode = qsCode;
		pCurrentReport->wMarket = BB_MARKET_EX;
	}

	connect(&timerUpdate,SIGNAL(timeout()),this,SLOT(updateData()));
	timerUpdate.start(UPDATE_BLOCK_TIME);
	updateData();

	initStockItem();
}

CBlockInfoItem::~CBlockInfoItem(void)
{
	CDataEngine::getDataEngine()->removeBlockInfoItem(this);
	clearTmpData();
	delete pCurrentReport;
}

void CBlockInfoItem::initStockItem()
{
	CDataEngine::getDataEngine()->setBlockInfoItem(this);
	clearTmpData();
	QFileInfo _info(blockFilePath);
	if(!_info.exists())
		return;

	if(_info.isFile())
	{
		QFile file(blockFilePath);
		if(file.open(QFile::ReadOnly))
		{
			QString qsType = file.readLine().trimmed();
			if(qsType == "RegExp")
			{
				QString qsExp = file.readLine().trimmed();
				if(!qsExp.isEmpty())
				{
					QRegExp _exp(qsExp);
					_exp.setPatternSyntax(QRegExp::Wildcard);
					QList<CStockInfoItem*> listStocks = CDataEngine::getDataEngine()->getStockInfoList();
					foreach(CStockInfoItem* p,listStocks)
					{
						if(_exp.exactMatch(p->getCode()) && !p->isIndex())
						{
							addStock(p);
						}
					}
					qsRegExp = qsExp;
				}
			}
			else
			{
				file.seek(0);
				while(!file.atEnd())
				{
					QString code = file.readLine();
					code = code.trimmed();
					if(!code.isEmpty())
					{
						CStockInfoItem* pStock = CDataEngine::getDataEngine()->getStockInfoItem(code);
						if(pStock==0)
							pStock = CDataEngine::getDataEngine()->getStockInfoItemByCode(code);
						addStock(pStock);
					}
				}
			}
			file.close();
		}
	}
	else if(_info.isDir())
	{
		QDir dir(blockFilePath);
		QFileInfoList listEntity = dir.entryInfoList(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot);
		foreach(const QFileInfo& _f,listEntity)
		{
			CBlockInfoItem* pChild = new CBlockInfoItem(_f.absoluteFilePath(),this);
			appendBlock(pChild);
		}
	}

	return CAbstractStockItem::initStockItem();
}

QList<tagRStockData*> CBlockInfoItem::getToday5MinList()
{
	QList<tagRStockData*> list = mapToday5MinDatas.values();
	if(pCurrent5Min->tmTime>0 && (!mapToday5MinDatas.contains(pCurrent5Min->tmTime)))
	{
		list.push_back(pCurrent5Min);
	}
	return list;
}

void CBlockInfoItem::recalcMinData()
{
	updateData();
	//���¼���5��������
	if(_isnan(fLTG) || fLTG<0.01)
	{
		//һ���Լ����ֵ
		//��������ͨ��
		double dLTG = 0.0;			//��ͨ�ɣ���

		int iCount = stocksInBlock.size();
		for(int i=0;i<iCount;++i)
		{
			CStockInfoItem* pStock = stocksInBlock[i];
			float fLTAG = pStock->getBaseInfo()->fLtAg;		//��ͨ��
			dLTG += fLTAG;
		}

		fLTG = dLTG;
	}

	if(fLTG<0.01)
		return;


	//��9��25��ʼ����
	time_t tmNow = CDataEngine::getDataEngine()->getCurrentTime();
	tmNow = tmNow/60*60+60;			//����ȡ��
	time_t tmBegin = ((tmNow/(3600*24))*3600*24)+3600*(9-8)+60*30;	//9��30����
	time_t tmCurrent = tmBegin+59;		//���㵽09:34:59

	//��һ���ڵļ۸񣨳�ʼ��ʱ���������̼ۣ�
	QMap<CStockInfoItem*,float> mapLastPrice;
	foreach(CStockInfoItem* pStock,stocksInBlock)
	{
		mapLastPrice[pStock] = pStock->getLastClose();
	}

	while(tmCurrent<tmNow)
	{
		if(tmCurrent>(tmBegin+120*60)&&tmCurrent<(tmBegin+210*60))
		{
			tmCurrent+=60;
			continue;
		}

		double dNew = 0.0;			//���¼�
		double dLow = 0.0;			//��ͼ�
		double dHigh = 0.0;			//��߼�
		double dOpen = 0.0;			//���̼�
		double dVolume = 0.0;		//�ɽ���
		double dAmount = 0.0;		//�ɽ���

		RStockData* pBlockData = new RStockData();
		pBlockData->tmTime = tmCurrent;

		//�����ʱ���ڵ�����
		foreach(CStockInfoItem* pStock,stocksInBlock)
		{
			float _last = pStock->getLastClose();		//ʹ���������̼�

			RStockData* pData = pStock->getTodayMinData(tmCurrent);
			if(pData)
			{
				float _new = pData->fClose;							//�������¼ۼ����Ƿ�
		//		float _last = mapLastPrice.value(pStock,_new);		//Ĭ��ʹ�ñ��μ۸�

				if(_new>0.1)
				{
					float fLTAG = pStock->getBaseInfo()->fLtAg;		//��ͨ��
					dNew += pData->fClose*fLTAG;
					dLow += pData->fLow*fLTAG;
					dHigh += pData->fHigh*fLTAG;
					dOpen += pData->fOpen*fLTAG;
					dVolume += pData->fVolume;
					dAmount += pData->fAmount;

					if(_last>0.1)
					{
						//�����ǵ�����
						if(_new>_last)
							++pBlockData->wAdvance;
						else if(_new<_last)
							++pBlockData->wDecline;
					}
				}

				//���¼۸����������
				mapLastPrice[pStock] = _new;
			}
		}

		pBlockData->fAmount = dAmount;
		pBlockData->fVolume = dVolume;
		pBlockData->fOpen = dOpen/fLTG;
		pBlockData->fClose = dNew/fLTG;
		pBlockData->fHigh = dHigh/fLTG;
		pBlockData->fLow = dLow/fLTG;
		appendMinData(pBlockData);

		//
		tmCurrent += 60;
	}

	emit stockItemFenBiChanged(qsOnly);
}

void CBlockInfoItem::recalc5MinData()
{
	updateData();
	//���¼���5��������
	if(_isnan(fLTG) || fLTG<0.01)
	{
		//һ���Լ����ֵ
		//��������ͨ��
		double dLTG = 0.0;			//��ͨ�ɣ���

		int iCount = stocksInBlock.size();
		for(int i=0;i<iCount;++i)
		{
			CStockInfoItem* pStock = stocksInBlock[i];
			float fLTAG = pStock->getBaseInfo()->fLtAg;		//��ͨ��
			dLTG += fLTAG;
		}

		fLTG = dLTG;
	}

	if(fLTG<0.01)
		return;


	//��9��25��ʼ����
	time_t tmNow = CDataEngine::getDataEngine()->getCurrentTime();
	tmNow = tmNow/300*300+300;			//����ȡ��
	time_t tmBegin = ((tmNow/(3600*24))*3600*24)+3600*(9-8)+60*30;	//9��30����
	time_t tmCurrent = tmBegin+299;		//���㵽09:34:59

	//��һ���ڵļ۸񣨳�ʼ��ʱ���������̼ۣ�
	QMap<CStockInfoItem*,float> mapLastPrice;
	foreach(CStockInfoItem* pStock,stocksInBlock)
	{
		mapLastPrice[pStock] = pStock->getLastClose();
	}

	while(tmCurrent<tmNow)
	{
		if(tmCurrent>(tmBegin+120*60)&&tmCurrent<(tmBegin+210*60))
		{
			tmCurrent+=300;
			continue;
		}

		double dNew = 0.0;			//���¼�
		double dLow = 0.0;			//��ͼ�
		double dHigh = 0.0;			//��߼�
		double dOpen = 0.0;			//���̼�
		double dVolume = 0.0;		//�ɽ���
		double dAmount = 0.0;		//�ɽ���

		RBlockData* pBlockData = new RBlockData();
		pBlockData->tmTime = tmCurrent;

		//�����ʱ���ڵ�����
		foreach(CStockInfoItem* pStock,stocksInBlock)
		{
			float _last = pStock->getLastClose();		//ʹ���������̼�

			RStockData* pData = pStock->getToday5MinData(tmCurrent);
			if(pData)
			{
				float _new = pData->fClose;							//�������¼ۼ����Ƿ�
				//		float _last = mapLastPrice.value(pStock,_new);		//Ĭ��ʹ�ñ��μ۸�

				if(_new>0.1)
				{
					float fLTAG = pStock->getBaseInfo()->fLtAg;		//��ͨ��
					dNew += pData->fClose*fLTAG;
					dLow += pData->fLow*fLTAG;
					dHigh += pData->fHigh*fLTAG;
					dOpen += pData->fOpen*fLTAG;
					dVolume += pData->fVolume;
					dAmount += pData->fAmount;

					if(_last>0.1)
					{
						//�����ǵ�����
						if(_new>_last)
							++pBlockData->wAdvance;
						else if(_new<_last)
							++pBlockData->wDecline;

						int _index = ((float(_new-_last))*100.0)/_last + 0.5;
						if(_index>9)
						{
							++(pBlockData->fIncrease[0]);
						}
						else if(_index>0)
						{
							++(pBlockData->fIncrease[10-_index]);
						}
						else if(_index<0&&_index>-10)
						{
							++(pBlockData->fIncrease[9-_index]);
						}
						else if(_index<-9)
						{
							++(pBlockData->fIncrease[19]);
						}
						else
						{
							++(pBlockData->fIncrease[20]);
						}
					}
				}

				//���¼۸����������
				mapLastPrice[pStock] = _new;
			}
		}

		pBlockData->fAmount = dAmount;
		pBlockData->fVolume = dVolume;
		pBlockData->fOpen = dOpen/fLTG;
		pBlockData->fClose = dNew/fLTG;
		pBlockData->fHigh = dHigh/fLTG;
		pBlockData->fLow = dLow/fLTG;
		appendToday5MinData(pBlockData);

		//
		tmCurrent += 300;
	}

	emit stockItemFenBiChanged(qsOnly);
}


QString CBlockInfoItem::getAbsPath()
{
	if(m_pParent)
	{
		return m_pParent->getAbsPath()+"|"+blockName;
	}
	else
	{
		return blockName;
	}
}

CBlockInfoItem* CBlockInfoItem::querySubBlock( const QStringList& _parent )
{
	QStringList listBlocks = _parent;
	if(listBlocks.size()<0)
		return 0;

	if(blocksInBlock.contains(listBlocks[0]))
	{
		CBlockInfoItem* pBlockItem = blocksInBlock[listBlocks[0]];
		if(listBlocks.size()==1)
			return pBlockItem;
		else
		{
			listBlocks.removeAt(0);
			return pBlockItem->querySubBlock(listBlocks);
		}
	}
	return 0;
}

bool CBlockInfoItem::isChildOf( CBlockInfoItem* parent)
{
	return getAbsPath().indexOf(parent->getAbsPath())>-1;
}

void CBlockInfoItem::addStockInfo( CStockInfoItem* _p )
{
	if(!m_pParent)
		return;
	if(m_pParent!=CDataEngine::getDataEngine()->getCustomBlock())
		return;
	addStock(_p);
	saveBlockFile();
}

void CBlockInfoItem::removeStockInfo( CStockInfoItem* _p )
{
	if(!m_pParent)
		return;
	if(m_pParent!=CDataEngine::getDataEngine()->getCustomBlock())
		return;
	removeStock(_p);
	saveBlockFile();
}

void CBlockInfoItem::addBlockInfo( CBlockInfoItem* _p )
{
	if(this!=CDataEngine::getDataEngine()->getCustomBlock())
		return;
	appendBlock(_p);
}

void CBlockInfoItem::removeBlockInfo( CBlockInfoItem* _p )
{
	if(this!=CDataEngine::getDataEngine()->getCustomBlock())
		return;
	removeBlock(_p);
	QFile::remove(_p->getFilePath());
}


QString CBlockInfoItem::getFilePath()
{
	return blockFilePath;
}


QList<CAbstractStockItem*> CBlockInfoItem::getAbsStockList()
{
	QList<CAbstractStockItem*> list;
	foreach(CBlockInfoItem* _p,blocksInBlock)
		list.push_back(_p);
	foreach(CStockInfoItem* _p,stocksInBlock)
		list.push_back(_p);

	return list;
}

QList<CStockInfoItem*> CBlockInfoItem::getStockList()
{
	return stocksInBlock;
}

QList<CBlockInfoItem*> CBlockInfoItem::getBlockList()
{
	return blocksInBlock.values();
}

int CBlockInfoItem::getStockCount() const
{
	return stocksInBlock.size();
}

int CBlockInfoItem::getBlockCount() const
{
	return blocksInBlock.size();
}

bool CBlockInfoItem::hasBlocks()
{
	return blocksInBlock.size()>0 ? true : false;
}

bool CBlockInfoItem::hasBlock( const QString& _code )
{
	if(blocksInBlock.contains(_code))
		return true;
	return false;
}

bool CBlockInfoItem::appendStocks( QList<CStockInfoItem*> list )
{
	QStringList listCodes;
	foreach(CStockInfoItem* pItem,list)
		listCodes.push_back(pItem->getOnly());

	if(appendStocks(listCodes))
	{
		foreach(CStockInfoItem* pItem,list)
		{
			if(!stocksInBlock.contains(pItem))
				stocksInBlock.push_back(pItem);
		}
		return true;
	}
	else
		return false;
}

bool CBlockInfoItem::appendStocks( QList<QString> list )
{
	if(blockFilePath.isEmpty())
		return false;

	QFile file(blockFilePath);
	if(!file.open(QFile::Append|QFile::WriteOnly))
		return false;
	foreach(const QString& e,list)
	{
		file.write(QString(e+"\r\n").toAscii());
	}
	file.close();

	return true;
}

bool CBlockInfoItem::removeStocks( QList<CStockInfoItem*> list )
{
	QStringList listCodes;
	foreach(CStockInfoItem* pItem,list)
		listCodes.push_back(pItem->getOnly());

	if(removeStocks(listCodes))
	{
		foreach(CStockInfoItem* pItem,list)
		{
			stocksInBlock.removeOne(pItem);
		}
		return true;
	}
	else
		return false;
}

bool CBlockInfoItem::removeStocks( QList<QString> list )
{
	if(blockFilePath.isEmpty())
		return false;

	QMap<QString,QString> mapStocks;
	{
		//��ȡ�ļ��еĹ�Ʊ����
		QFile file(blockFilePath);
		if(!file.open(QFile::ReadOnly))
			return false;

		while(!file.atEnd())
		{
			QString code = file.readLine();
			code = code.trimmed();
			if(!code.isEmpty())
			{
				mapStocks[code] = code;
			}
		}
		file.close();
	}
	{
		//ɾ����Ӧ�Ĺ�Ʊ����
		foreach(const QString& e,list)
		{
			mapStocks.remove(e);
		}
	}
	{
		//����д��ȥ
		QFile file(blockFilePath);
		if(!file.open(QFile::Truncate|QFile::WriteOnly))
			return false;

		QMap<QString,QString>::iterator iter = mapStocks.begin();
		while(iter!=mapStocks.end())
		{
			file.write(QString(iter.value()+"\r\n").toAscii());
			++iter;
		}
		file.close();
	}

	return true;
}

bool CBlockInfoItem::appendBlock( CBlockInfoItem* pBlock )
{
	blocksInBlock[pBlock->getBlockName()] = pBlock;
	return true;
}

bool CBlockInfoItem::removeBlock( CBlockInfoItem* pBlock )
{
	if(blocksInBlock.contains(pBlock->getBlockName()))
	{
		blocksInBlock.remove(pBlock->getBlockName());
		return true;
	}
	return false;
}


void CBlockInfoItem::stockFenbiChanged( const QString& /*_code*/ )
{

}

void CBlockInfoItem::stockHistoryChanged( const QString& /*_code*/ )
{

}

void CBlockInfoItem::updateData()
{
	//��ȡ��ǰʱ��
	time_t tmNow = CDataEngine::getDataEngine()->getCurrentTime();
	tmNow = tmNow/60*60+59;		//��tmNow����Ϊ��ǰ5�������ڵ����ʱ��
	/*
	���¼������ʱ��Ϊ1���Ӹ���һ��
	���㷽ʽ��ȡǰʮֻ��Ʊ���м�Ȩƽ��
	*/
	if((QDateTime::currentDateTime().toTime_t()-tmNow)>70)
	{
		//�����ǰʱ�������ʱ��������70��(ȡ10����ӳ�)��ֱ�Ӻ���
		if(!_isnan(fLastClose))
			return;
	}

	if(_isnan(fLTG) || fLTG<0.01)
	{
		//һ���Լ����ֵ
		//��������ͨ��
		double dLTG = 0.0;			//��ͨ�ɣ���

		int iCount = stocksInBlock.size();
		for(int i=0;i<iCount;++i)
		{
			CStockInfoItem* pStock = stocksInBlock[i];
			float fLTAG = pStock->getBaseInfo()->fLtAg;		//��ͨ��
			dLTG += fLTAG;
		}

		fLTG = dLTG;
	}
	if(fLTG<0.01)
		return;

	if(pCurrentMin->tmTime>0)
	{
		if((tmNow/60) > (pCurrentMin->tmTime/60))
		{
			//׷�ӵ�5���������У������¶Ե�ǰ5�������ݷ����ڴ�
			appendMinData(pCurrentMin);
			pCurrentMin = new RStockData;

			//������1�������ݽ��б���
			fLastMinVolume = fVolume;
			fLastMinAmount = fAmount;
		}
	}

	if(pCurrent5Min->tmTime>0)
	{
		if(tmNow/300 > (pCurrent5Min->tmTime/300))
		{
			//׷�ӵ�5���������У������¶Ե�ǰ5�������ݷ����ڴ�
			appendMinData(pCurrent5Min);
			pCurrent5Min = new RBlockData;
			//������5�������ݽ��б���
			mapLast5Price = mapLastPrice;
			fLast5MinVolume = fVolume;
			fLast5MinAmount = fAmount;
		}
	}


	double dNew = 0.0;			//���¼�
	double dLow = 0.0;			//��ͼ�
	double dHigh = 0.0;			//��߼�
	double dLastClose = 0.0;	//�������̼�
	double dOpen = 0.0;			//���տ��̼�

	fVolume = 0.0;				//�ɽ���
	fAmount = 0.0;				//�ɽ���
	float fReport[20];			//Report��20������
	memset(&fReport,0,sizeof(float)*20);
	memset(&(pCurrent5Min->fIncrease[0]),0,sizeof(float)*21);
	pCurrentMin->wAdvance = 0;
	pCurrentMin->wDecline = 0;
	pCurrent5Min->wAdvance = 0;
	pCurrent5Min->wAdvance = 0;
	int iCount = stocksInBlock.size();
	for(int i=0;i<iCount;++i)
	{
		CStockInfoItem* pStock = stocksInBlock[i];
		float _new = pStock->getNewPrice();					//�������¼ۼ����Ƿ�
		float _open = pStock->getOpenPrice();				//��ȡ���̼�
		float _last = pStock->getLastClose()/*mapLast5Price.value(pStock,_open)*/;	//Ĭ��ʹ�ÿ��̼�
		float _close = pStock->getLastClose();
		if(_new>0.1)
		{
			float fLTAG = pStock->getBaseInfo()->fLtAg;		//��ͨ��
			dNew += pStock->getNewPrice()*fLTAG;
			dLow += pStock->getLowPrice()*fLTAG;
			dHigh += pStock->getHighPrice()*fLTAG;
			dLastClose += pStock->getLastClose()*fLTAG;
			dOpen += _open*fLTAG;
			fVolume += pStock->getTotalVolume();
			fAmount += pStock->getTotalAmount();

			if(_last>0.1)
			{
				//�����ǵ�����
				if(_new>_last)
				{
					++pCurrentMin->wAdvance;
					++pCurrent5Min->wAdvance;
				}
				else if(_new<_last)
				{
					++pCurrentMin->wDecline;
					++pCurrent5Min->wAdvance;
				}

				int _index = ((float(_new-_last))*100.0)/_last + 0.5;
				if(_index>9)
				{
					++(pCurrent5Min->fIncrease[0]);
				}
				else if(_index>0)
				{
					++(pCurrent5Min->fIncrease[10-_index]);
				}
				else if(_index<0&&_index>-10)
				{
					++(pCurrent5Min->fIncrease[9-_index]);
				}
				else if(_index<-9)
				{
					++(pCurrent5Min->fIncrease[19]);
				}
				else
				{
					++(pCurrent5Min->fIncrease[20]);
				}
			}
			if(_close>0.1)
			{
				//����report���ǵ�����
				int _index = float((_new-_close)/_close)*100.0 + 0.5;
				if(_index>9)
				{
					++fReport[0];
				}
				else if(_index>0)
				{
					++fReport[10-_index];
				}
				else if(_index<0&&_index>-10)
				{
					++fReport[9-_index];
				}
				else
				{
					++fReport[19];
				}
			}

			mapLastPrice[pStock] = _new;
		}
	}

	fNewPrice = dNew/fLTG;
	fLowPrice = dLow/fLTG;
	fHighPrice = dHigh/fLTG;
	fOpenPrice = dOpen/fLTG;
	fLastClose = dLastClose/fLTG;
	fLTSZ = dNew;

	//�Ƿ�
	if(fNewPrice>0.0 && fLastClose>0.0)
		fIncrease = (fNewPrice-fLastClose)*100.0/fLastClose;
	{
		//�������ݸ���ǰ��1�������ݽ�������
		if(pCurrentMin->tmTime>0)
		{
//			pCurrentMin->tmTime = tmNow;
			pCurrentMin->fClose = fNewPrice;
			if(pCurrentMin->fHigh<fNewPrice)
				pCurrentMin->fHigh = fNewPrice;
			if(pCurrentMin->fLow>fNewPrice)
				pCurrentMin->fLow = fNewPrice;
			pCurrentMin->fAmount = fAmount - fLastMinAmount;
			pCurrentMin->fVolume = fVolume - fLastMinVolume;
		}
		else
		{
			pCurrentMin->tmTime = tmNow;
			pCurrentMin->fClose = fNewPrice;
			pCurrentMin->fHigh = fNewPrice;
			pCurrentMin->fLow = fNewPrice;
			pCurrentMin->fOpen = fNewPrice;
			pCurrentMin->fAmount = fAmount - fLastMinAmount;
			pCurrentMin->fVolume = fVolume - fLastMinVolume;
		}
	}
	{
		//�������ݸ���ǰ��1�������ݽ�������
		if(pCurrent5Min->tmTime>0)
		{
			//pCurrent5Min->tmTime = tmNow;
			pCurrent5Min->fClose = fNewPrice;
			if(pCurrent5Min->fHigh<fNewPrice)
				pCurrent5Min->fHigh = fNewPrice;
			if(pCurrent5Min->fLow>fNewPrice)
				pCurrent5Min->fLow = fNewPrice;
			pCurrent5Min->fAmount = fAmount - fLast5MinAmount;
			pCurrent5Min->fVolume = fVolume - fLast5MinVolume;
		}
		else
		{
			pCurrent5Min->tmTime = tmNow;
			pCurrent5Min->fClose = fNewPrice;
			pCurrent5Min->fHigh = fNewPrice;
			pCurrent5Min->fLow = fNewPrice;
			pCurrent5Min->fOpen = fNewPrice;
			pCurrent5Min->fAmount = fAmount - fLast5MinAmount;
			pCurrent5Min->fVolume = fVolume - fLast5MinVolume;
		}
	}

	pCurrentReport->tmTime = CDataEngine::getCurrentTime();
	pCurrentReport->fOpen = fOpenPrice;
	pCurrentReport->fNewPrice = fNewPrice;
	pCurrentReport->fLow = fLowPrice;
	pCurrentReport->fHigh = fHighPrice;
	pCurrentReport->fLastClose = fLastClose;
	pCurrentReport->fAmount = fAmount;
	pCurrentReport->fVolume = fVolume;
	memcpy(&pCurrentReport->fBuyPrice[0],&fReport[0],80);

	emit stockItemReportChanged(qsOnly);
	emit stockItemFenBiChanged(qsOnly);
}

void CBlockInfoItem::addStock( CStockInfoItem* _p )
{
	if(!_p)
		return;

	if(!stocksInBlock.contains(_p))
	{
		stocksInBlock.push_back(_p);
		connect(_p,SIGNAL(stockItemHistoryChanged(const QString&)),this,SLOT(stockHistoryChanged(const QString&)));
		connect(_p,SIGNAL(stockItemFenBiChanged(const QString&)),this,SLOT(stockFenbiChanged(const QString&)));
	}
}

void CBlockInfoItem::removeStock( CStockInfoItem* _p )
{
	if(!_p)
		return;
	stocksInBlock.removeOne(_p);
	disconnect(_p,SIGNAL(stockItemHistoryChanged(const QString&)),this,SLOT(stockHistoryChanged(const QString&)));
	disconnect(_p,SIGNAL(stockItemFenBiChanged(const QString&)),this,SLOT(stockFenbiChanged(const QString&)));
}

void CBlockInfoItem::saveBlockFile()
{
	if(!qsRegExp.isEmpty())
		return;
	QFile file(blockFilePath);
	if(!file.open(QFile::WriteOnly|QFile::Truncate))
		return;
	foreach(CStockInfoItem* pStock,stocksInBlock)
	{
		file.write(QString("%1\r\n").arg(pStock->getOnly()).toAscii());
	}
	file.close();
	return;
}

void CBlockInfoItem::clearTmpData()
{
	foreach(CStockInfoItem* _p,stocksInBlock)
	{
		disconnect(_p,SIGNAL(stockItemHistoryChanged(const QString&)),this,SLOT(stockHistoryChanged(const QString&)));
		disconnect(_p,SIGNAL(stockItemFenBiChanged(const QString&)),this,SLOT(stockFenbiChanged(const QString&)));
	}
	stocksInBlock.clear();

	foreach(CBlockInfoItem* _p,blocksInBlock)
	{
		delete _p;
	}
	blocksInBlock.clear();
}


QString CBlockInfoItem::getName() const
{
	return blockName;
}

float CBlockInfoItem::getIncrease() const
{
	//����
	return fIncrease;
}

float CBlockInfoItem::getVolumeRatio() const
{
	//
	return 0.0;
}

float CBlockInfoItem::getTurnRatio() const
{
	return 0.0;
}

float CBlockInfoItem::getLastClose() const
{
	return fLastClose;
}

float CBlockInfoItem::getOpenPrice() const
{
	return fOpenPrice;
}

float CBlockInfoItem::getHighPrice() const
{

	return fHighPrice;
}

float CBlockInfoItem::getLowPrice() const
{
	return fLowPrice;
}

float CBlockInfoItem::getNewPrice() const
{
	return fNewPrice;
}

float CBlockInfoItem::getTotalVolume() const
{

	return 0.0;
}

float CBlockInfoItem::getTotalAmount() const
{

	return 0.0;
}

float CBlockInfoItem::getNowVolume() const
{

	return 0.0;
}

float CBlockInfoItem::getIncSpeed() const
{

	return 0.0;
}

float CBlockInfoItem::getPriceFluctuate() const
{

	return 0.0;
}

float CBlockInfoItem::getAmplitude() const
{

	return 0.0;
}

float CBlockInfoItem::getAvePrice() const
{

	return fNewPrice;
}

float CBlockInfoItem::getPERatio() const
{

	return 0.0;
}

float CBlockInfoItem::getLTSZ() const
{

	return 0.0;
}

float CBlockInfoItem::getZGB() const
{

	return 0.0;
}

float CBlockInfoItem::getSellVOL() const
{

	return 0.0;
}

float CBlockInfoItem::getBuyVOL() const
{

	return 0.0;
}

float CBlockInfoItem::getBIDVOL() const
{

	return 0.0;
}

float CBlockInfoItem::getASKVOL() const
{

	return 0.0;
}

float CBlockInfoItem::getBIDVOL3() const
{

	return 0.0;
}

float CBlockInfoItem::getASKVOL3() const
{

	return 0.0;
}

float CBlockInfoItem::getCommRatio() const
{

	return 0.0;
}

float CBlockInfoItem::getCommSent() const
{

	return 0.0;
}

float CBlockInfoItem::getLast5Volume()
{

	return 0.0;
}

float CBlockInfoItem::getMgjzc()
{

	return 0.0;
}

float CBlockInfoItem::getMgsy()
{

	return 0.0;
}

bool CBlockInfoItem::isMatch( const QString& _key )
{
	//�жϴ����Ƿ�ƥ��
	if(qsCode.indexOf(_key)>-1)
		return true;
	
	//�ж����Ƽ�ƴ�Ƿ�ƥ��
	for (int i = 0; i < _key.size(); ++i)
	{
		if(i>=shortName.size())
			return false;
		QList<QChar> _l = shortName[i];
		bool bMatch = false;
		foreach(const QChar& _c,_l)
		{
			if(_c == _key[i])
			{
				bMatch = true;
				break;
			}
		}
		if(!bMatch)
			return false;
	}
	return true;
}
