/************************************************************************/
/* �ļ����ƣ�RadarWatcher.h
/* ����ʱ�䣺2013-06-05 19:51
/*
/* ��    �����״�����࣬�����������ݵ��쳣
/************************************************************************/
#ifndef	RADAR_WATCHER_H
#define	RADAR_WATCHER_H
#include <QtCore>
#include "rsd_global.h"

class CStockInfoItem;
class CBlockInfoItem;
class CRadarWatcher;

enum RadarType
{
	BigVolumn = 1,		//��ɽ���
	BigIncrease = 2,	//�Ƿ�
	MaxPrice = 4,		//���¸�
	MinPrice = 8,		//���µ�
};

typedef struct tagRRadarData
{
	RadarType tpType;				//�״�����
	time_t tmTime;					//����ʱ��
	int iWatcher;					//��Դ�״�ID
	CStockInfoItem* pStock;			//��Ʊָ��
	QString qsDesc;					//������Ϣ
} RRadarData;


class RSDSHARED_EXPORT CRadarWatcher : public QObject
{
	Q_OBJECT
public:
	CRadarWatcher(int _id,CBlockInfoItem* pBlock,RadarType _t,int iSec,float _hold,CBlockInfoItem* pDestBlock);
	~CRadarWatcher(void);

public:
	//��ȡ���״��id
	int getId(){ return m_id; }
	RadarType getType(){return m_type;}
	float getHold(){ return m_fHold; }
	void setHold(float _hold){ m_fHold=_hold; }
	int getSec(){ return m_iSec; }
	void setSec(int _sec){ m_iSec = _sec; }
	CBlockInfoItem* getBlock(){ return m_pWatcherBlock; }
	void setBlock(CBlockInfoItem* _b);
	CBlockInfoItem* getDestBlock(){ return m_pDestBlock; }
	void setDestBlock(CBlockInfoItem* _b);

protected slots:
	virtual void onStockReportComing(CStockInfoItem* pItem) = 0;

protected:
	int m_iSec;									//���Ӽ��
	float m_fHold;								//��ֵ
	int m_id;									//�״�id
	CBlockInfoItem* m_pDestBlock;				//������İ��

private:
	RadarType m_type;							//�״�����
	CBlockInfoItem* m_pWatcherBlock;			//���ӵİ��
};

class RSDSHARED_EXPORT CRadarManager : public QObject
{
	Q_OBJECT
public:
	static CRadarManager* getRadarManager();
	static QString getTypeName(RadarType _t);

public:
	/*
		�����״����
		pBlock ���ӵİ��
		_t �״�����
		iSec ���ӵ�ʱ����
		_hold ��ֵ
	*/
	CRadarWatcher* createRadarWatcher(CBlockInfoItem* pBlock,
		RadarType _t,int iSec,float _hold,CBlockInfoItem* pDestBlock=NULL,int iId=-1);
	
	//��ȡ���еļ����״�
	QList<CRadarWatcher*> getRadarWatchers();

	//��ȡָ��ID�ļ����״�
	CRadarWatcher* getWatcher(const int& _id);

	//ɾ��ָ��ID�ļ����״�
	void removeWatcher(const int& _id);

	//׷�Ӳ����ļ�����Ϣ
	void appendRadar(RRadarData* pRadar);

	//���ؼ����״�
	void loadRadars();
	//��������״�
	void saveRadars();
	//���������״�
	void releaseRadars();

signals:
	void radarAlert(RRadarData* pRadar);

protected:
	CRadarManager(){};
	~CRadarManager(){};

private:
	QMap<int,CRadarWatcher*> m_listWatchers;	//���еļ�����
	QList<RRadarData*> m_listRadar;				//Ԥ���״�����

private:
	static CRadarManager* m_pSelf;
};


/*�ɽ������ӣ����ӳɽ����ı仯*/
class CVolumnWatcher : public CRadarWatcher
{
	struct tagVolumnData
	{
		time_t tmTime;		//����γ�ʱ��
		float fLastV;		//��һ�εĳɽ���
		float fNewV;		//ʱ������ʾ�ĳɽ���
	};
	Q_OBJECT
public:
	CVolumnWatcher(int _id,CBlockInfoItem* pBlock,int iSec,float _hold,CBlockInfoItem* pDestBlock);
	~CVolumnWatcher(void);
protected slots:
	void onStockReportComing(CStockInfoItem* pItem);

private:
	QMap<CStockInfoItem*,tagVolumnData*> m_mapVolume;		//��һ�ֳɽ���
};

class CIncreaseWatcher : public CRadarWatcher
{
	struct tagPriceData
	{
		time_t tmTime;		//����γ�ʱ��
		float fMaxPrice;	//�����е���߼۸�
		float fNewPrice;	//ʱ������ʾ�ļ۸�
	};
	Q_OBJECT
public:
	CIncreaseWatcher(int _id,CBlockInfoItem* pBlock,int iSec,float _hold,CBlockInfoItem* pDestBlock);
	~CIncreaseWatcher(void);
protected slots:
	void onStockReportComing(CStockInfoItem* pItem);

private:
	QMap<CStockInfoItem*,tagPriceData*> m_mapPrice;		//��һ�ֳɽ���
};

class CMaxPriceWatcher : public CRadarWatcher
{
	struct tagPriceData
	{
		time_t tmTime;		//����γ�ʱ��
		float fMaxPrice;	//�����е���߼۸�
		float fNewPrice;	//ʱ������ʾ�ļ۸�
	};
	Q_OBJECT
public:
	CMaxPriceWatcher(int _id,CBlockInfoItem* pBlock,int iSec,float _hold,CBlockInfoItem* pDestBlock);
	~CMaxPriceWatcher(void);
protected slots:
	void onStockReportComing(CStockInfoItem* pItem);

private:
	QMap<CStockInfoItem*,tagPriceData*> m_mapPrice;		//��һ�ֳɽ���
};

class CMinPriceWatcher : public CRadarWatcher
{
	struct tagPriceData
	{
		time_t tmTime;		//����γ�ʱ��
		float fMinPrice;	//�����е���߼۸�
		float fNewPrice;	//ʱ������ʾ�ļ۸�
	};
	Q_OBJECT
public:
	CMinPriceWatcher(int _id,CBlockInfoItem* pBlock,int iSec,float _hold,CBlockInfoItem* pDestBlock);
	~CMinPriceWatcher(void);
protected slots:
	void onStockReportComing(CStockInfoItem* pItem);

private:
	QMap<CStockInfoItem*,tagPriceData*> m_mapPrice;		//��һ�ֳɽ���
};

#endif	//RADAR_WATCHER_H