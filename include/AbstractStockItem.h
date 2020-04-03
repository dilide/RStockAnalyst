/************************************************************************/
/* �ļ����ƣ�AbstractStockItem.h
/* ����ʱ�䣺2013-05-20 14:14
/*
/* ��    ������Ʊ���ݵĴ������
/************************************************************************/
#ifndef ABSTRACT_STOCK_ITEM_H
#define ABSTRACT_STOCK_ITEM_H
#include <QtCore>
#include "rsd_global.h"

struct qRcvFenBiData;
struct qRcvReportData;
struct qRcvHistoryData;
struct tagRStockData;
struct tagRcvBlockData;

class RSDSHARED_EXPORT CAbstractStockItem : public QObject
{
	Q_OBJECT
public:
	CAbstractStockItem(void);
	~CAbstractStockItem(void);

public:
	virtual void initStockItem();							//��ʼ����Ʊ

	//����Report����
	virtual qRcvReportData* getCurrentReport() const;

	//����ֱ�����
	virtual QList<qRcvFenBiData*> getTodayFenBiList();
	virtual QList<qRcvFenBiData*> getFenBiList();
	virtual void appendFenBis(const QList<qRcvFenBiData*>& list);

	//������������
	virtual QList<qRcvHistoryData*> getHistoryList();
	virtual QList<qRcvHistoryData*> getLastHistory(int count);
	virtual void appendHistorys(const QList<qRcvHistoryData*>& list);

	//������ʷ��������
	virtual void appendMinData(tagRStockData* pData);			//�����������
	virtual void appendToday5MinData(tagRStockData* pData);		//���䵱��5��������
	virtual QList<tagRStockData*> getMinList();			//׷����󲻹�1���ӵ�����
	virtual QList<tagRStockData*> getMinList(int _c);	//��ȡ�������µ���ʷ����
	virtual void recalcMinData() = 0;					//���¼��㵱��1Min����

	//��ȡ���յ�5��������
	virtual QList<tagRStockData*> getToday5MinList() = 0;
	virtual tagRStockData* getToday5MinData( const time_t& tmTime );//��ȡָ��ʱ�������
	virtual void recalc5MinData() = 0;				//���¼��㵱��5Min����

public:
	/*�������ֶΣ�ֻ��*/
	virtual QString getOnly() const;		//Ψһ��ʶ 000009SZ
	virtual QString getCode() const;		//��Ʊ����
	virtual WORD getMarket() const;			//��Ʊ�г�
	virtual QString getMarketName() const;	//��Ʊ�г�����

	virtual QString getName() const = 0;		//��Ʊ����
	virtual float getIncrease() const = 0;		//�Ƿ�
	virtual float getVolumeRatio() const = 0;	//����
	virtual float getTurnRatio() const = 0;		//������
	virtual float getLastClose() const = 0;		//��һ�����̼�
	virtual float getOpenPrice() const = 0;		//���տ��̼�
	virtual float getHighPrice() const = 0;		//������߼�
	virtual float getLowPrice() const = 0;		//������ͼ�
	virtual float getNewPrice() const = 0;		//���¼�
	virtual float getTotalVolume() const = 0;	//��ȡ����
	virtual float getTotalAmount() const = 0;	//��ȡ����
	virtual float getNowVolume() const = 0;		//��ȡ����
	virtual float getIncSpeed() const = 0;		//����
	virtual float getPriceFluctuate() const = 0;//�ǵ����۸񲨶�
	virtual float getAmplitude() const = 0;		//���
	virtual float getAvePrice() const = 0;		//����
	virtual float getPERatio() const = 0;		//��ӯ��
	virtual float getLTSZ() const = 0;			//��ͨ��ֵ
	virtual float getZGB() const = 0;			//�ܹɱ�
	virtual float getSellVOL() const = 0;		//������
	virtual float getBuyVOL() const = 0;		//������
	virtual float getBIDVOL() const = 0;		//ί����
	virtual float getBIDVOL3() const = 0;		//ί����3
	virtual float getASKVOL3() const = 0;		//ί����3
	virtual float getASKVOL() const = 0;		//ί����
	virtual float getCommRatio() const = 0;		//ί��
	virtual float getCommSent() const = 0;		//ί��

	virtual float getLast5Volume() = 0;			//��ȡ��ȥ5�յĳɽ��������ڼ������ȣ�

	virtual float getMgjzc() = 0;				//ÿ�ɾ��ʲ�
	virtual float getMgsy() = 0;				//��ȡÿ������
	virtual float getLtag(){ return -1.0; }		//��ͨA��

	virtual int getNewHighPriceCount();			//��ȡ�۸��¸ߴ�������һ����Ϊ��
	virtual int getNewHighVolumeCount();		//��ȡ�ɽ������¸ߴ�������һ����Ϊ��


	/*�жϹؼ���_key�Ƿ�ƥ��*/
	virtual bool isMatch(const QString& _key) = 0;

	bool isInstanceOfStock();
	bool isInstanceOfBlock();

	virtual bool isIndex() = 0;							//�Ƿ���ָ��

protected:
	virtual void updateItemInfo() = 0;
	void loadHisMinData();
signals:
	void stockItemReportChanged(const QString&);	//�������ݸ���
	void stockItemHistoryChanged(const QString&);	//��ʷ���ݸ���
	void stockItemFenBiChanged(const QString&);		//�ֱ����ݸ���


protected:
	QString qsOnly;									//Ψһ��ʶ
	QString qsCode;									//����
	WORD wMarket;									//�г�����
	QString qsMarket;								//�г���������
	QMap<time_t,qRcvFenBiData*> mapFenBis;			//�ֱ�����
	QMap<time_t,tagRStockData*> mapToday5MinDatas;	//����5������ʷ����

	QMap<time_t,tagRStockData*> mapMinDatas;		//1������ʷ���ݣ�һ��洢7���
	QList<tagRStockData*> list5MinDatas;			//5������ʷ���ݣ�һ��洢100���
	QList<tagRStockData*> list15MinDatas;			//15������ʷ���ݣ�һ���5�������ݼ���ó�
	QList<tagRStockData*> list30MinDatas;			//30������ʷ���ݣ�һ���5�������ݼ���ó�
	QList<tagRStockData*> list60MinDatas;			//60������ʷ���ݣ�һ���5�������ݼ���ó�
	QList<qRcvHistoryData*> listHistories;			//��ʷ����
	qRcvReportData* pCurrentReport;					//��ǰ��Report


	tagRStockData* pCurrentMin;		//��ǰ��1��������
	tagRStockData* pCurrent5Min;	//��ǰ��5��������
	tagRStockData* pCurrent15Min;	//��ǰ��15��������
	tagRStockData* pCurrent30Min;	//��ǰ��30��������
	tagRStockData* pCurrent60Min;	//��ǰ��60��������

	float fLast5Volume;				//��ȥ5�յĳɽ����������ڼ������ȣ�
	bool m_bLoadHisMin;				//�Ƿ��Ѿ�������ʷ��������
};

#endif //ABSTRACT_STOCK_ITEM_H