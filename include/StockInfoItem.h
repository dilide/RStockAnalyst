/************************************************************************/
/* �ļ����ƣ�StockInfoItem.h
/* ����ʱ�䣺2012-11-08 09:28
/*
/* ��    �������ڴ洢��ֻ��Ʊ����
/************************************************************************/
#ifndef STOCK_INFO_ITEM_H
#define STOCK_INFO_ITEM_H
#include <QtCore>
#include "rsd_global.h"
#include "RStockFunc.h"

class RSDSHARED_EXPORT CStockInfoItem : public CAbstractStockItem
{
	Q_OBJECT
public:
	CStockInfoItem(const QString& code, WORD market);
	CStockInfoItem(const qRcvBaseInfoData& info);
	~CStockInfoItem(void);

public:
	//����Report����
	void setReport(qRcvReportData* p);
	void setReport(RCV_REPORT_STRUCTExV3* p);

	//�����Ȩ����
	QList<qRcvPowerData*> getPowerList();
	void appendPowers(const QList<qRcvPowerData*>& list);

	//����ֱ�����
	virtual void appendFenBis(const QList<qRcvFenBiData*>& list);

	//���侺������
	virtual void appendJingJias(qRcvFenBiData* pJingJia);

	//��ȡ����5��������
	virtual QList<tagRStockData*> getToday5MinList();	//׷����󲻹�5���ӵ�����

	//��ȡĳʱ���5��������
	RStockData* getTodayMinData(const time_t& tmTime);
	//���¼��㵱��1Min����
	virtual void recalcMinData();

	//���¼��㵱��5Min����
	virtual void recalc5MinData();

	//����F10����
	void setBaseInfo(const qRcvBaseInfoData& info);
	qRcvBaseInfoData* getBaseInfo(){ return &baseInfo; }

	//�������5�յ��ܳɽ���
	void setLast5Volume(float fValue){ fLast5Volume = fValue; }

public:
	/*�������ֶΣ�ֻ��*/
	QString getName() const;		//��Ʊ����
	float getIncrease() const;	//�Ƿ�
	float getVolumeRatio() const;	//����
	float getTurnRatio() const;	//������
	float getLastClose() const;	//��һ�����̼�
	float getOpenPrice() const;	//���տ��̼�
	float getHighPrice() const;	//������߼�
	float getLowPrice() const;	//������ͼ�
	float getNewPrice() const;	//���¼�
	float getTotalVolume() const;	//��ȡ����
	float getTotalAmount() const;	//��ȡ����
	float getNowVolume() const;	//��ȡ����
	float getIncSpeed() const;	//����
	float getPriceFluctuate() const;//�ǵ����۸񲨶�
	float getAmplitude() const;	//���
	float getAvePrice() const;	//����
	float getPERatio() const;		//��ӯ��
	float getLTSZ() const;			//��ͨ��ֵ
	float getZGB() const;			//�ܹɱ�
	float getSellVOL() const;		//������
	float getBuyVOL() const;		//������
	float getBIDVOL() const;		//ί����
	float getASKVOL() const;		//ί����
	float getBIDVOL3() const;		//ί����
	float getASKVOL3() const;		//ί����
	float getCommRatio() const;	//ί��
	float getCommSent() const;	//ί��

	float getLast5Volume(); //��ȡ��ȥ5�յĳɽ��������ڼ������ȣ�

	float getMgjzc();		//ÿ�ɾ��ʲ�
	float getMgsy();		//��ȡÿ������
	float getLtag();		//��ͨA��

	QVector<int> getLast5CmpPrices();		//��ȡ���5���ӵļ۸�仯
	/*�жϹؼ���_key�Ƿ�ƥ��*/
	bool isMatch(const QString& _key);

	virtual bool isIndex(){ return m_bIsIndex; }
signals:
	void stockItemReportComing(CStockInfoItem*);//�������ݸ���

protected:
	virtual void updateItemInfo();
	void resetBuySellVOL();		//���¼�������������

private:
	QString qsName;					//��Ʊ����
	float fIncrease;				//�Ƿ�
	float fVolumeRatio;				//����
	float fTurnRatio;				//������

	qRcvReportData* pLastReport;	//�����Report
	float fLastMinVolume;			//���5�������ݵĳɽ���
	float fLastMinAmount;			//���5�������ݵĳɽ���

	RStockData* pCurrent5Min;		//��ǰ��5��������
	float fLast5MinVolume;			//���5�������ݵĳɽ���
	float fLast5MinAmount;			//���5�������ݵĳɽ���


	float fNowVolume;				//����
	float fIncreaseSpeed;			//�����ٶ�  (NewPrice-OldPrice)/OldPrice
	float fPriceFluctuate;			//�ǵ����۸񲨶�
	float fAmplitude;				//���
	float fAvePrice;				//����

	float fPERatio;					//��ӯ��
	float fLTSZ;					//��ͨ��ֵ
	float fZGB;						//�ܹɱ�
	float fSellVOL;					//������
	float fBuyVOL;					//������

	float fBuyVolume;				//ί����
	float fSellVolume;				//ί����
	float fBuyVolume3;				//3��ί����
	float fSellVolume3;				//3��ί����
	float fCommRatio;				//ί��
	float fCommSent;				//ί��

	QList<QList<QChar>> shortName;	//��ƴ��

	float fLastCmpPrice;			//��һ�������Ƚϵļ۸�

	bool m_bIsIndex;				//�Ƿ���ָ����

private:
	QMap<time_t,qRcvPowerData*> mapPowers;			//��Ȩ����
	QMultiMap<time_t,qRcvFenBiData*> mapJingJias;	//9:25ǰ�ľ�������
	qRcvBaseInfoData baseInfo;
	QVector<int> vCmpPrices;						//�����ȽϺ���ǵ���
};

#endif	//STOCK_INFO_ITEM_H