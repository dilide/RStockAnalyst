/************************************************************************/
/* �ļ����ƣ�BlockInfoItem.h
/* ����ʱ�䣺2013-01-08 09:28
/*
/* ��    �������ڹ���������
/************************************************************************/
#ifndef	BLOCK_INFO_ITEM_H
#define BLOCK_INFO_ITEM_H
#include <QtCore>
#include "StockInfoItem.h"
#include "rsd_global.h"

class RSDSHARED_EXPORT CBlockInfoItem : public CAbstractStockItem
{
	Q_OBJECT
public:
	CBlockInfoItem(const QString& _file,CBlockInfoItem* parent=0);						//���캯����ֻ�������ƣ�Ȼ��ͨ������ȥ��ȡ�ļ��е��б�
	~CBlockInfoItem(void);

protected:
	//��ʼ���ӽڵ�
	virtual void initStockItem();

public:
	//��ȡ����5��������
	virtual QList<tagRStockData*> getToday5MinList();	//׷����󲻹�5���ӵ�����

	//���¼��㵱��1Min����
	virtual void recalcMinData();

	//���¼��㵱��5��������
	virtual void recalc5MinData();

public:
	QString getBlockName() const{ return blockName; }			//��ȡ�������
	QString getAbsPath();										//��ȡ���ľ���·��
	QList<CAbstractStockItem*> getAbsStockList();				//��ȡȫ���б�
	QList<CStockInfoItem*> getStockList();						//��ȡ��Ʊ�б�
	QList<CBlockInfoItem*> getBlockList();						//��ȡ����б�

	int getStockCount() const;									//��ȡ��Ʊ������
	int getBlockCount() const;									//��ȡ��������

	bool hasBlocks();											//���ظð���Ƿ����Ӱ��
	bool hasBlock(const QString& _code);						//�Ƿ������block

	bool appendStocks(QList<CStockInfoItem*> list);		//��������ӹ�Ʊ
	bool removeStocks(QList<CStockInfoItem*> list);		//�Ӱ����ɾ����Ʊ

	CBlockInfoItem* querySubBlock(const QStringList& _parent);	//�����Ӱ��
	bool isChildOf(CBlockInfoItem* parent);

	CBlockInfoItem* parentBlock(){ return m_pParent;}

	void addStockInfo(CStockInfoItem* _p);
	void removeStockInfo(CStockInfoItem* _p);

	void addBlockInfo(CBlockInfoItem* _p);
	void removeBlockInfo(CBlockInfoItem* _p);

	QString getFilePath();					//��ȡ�˰����ļ�·��

public:
	/*�������ֶΣ�ֻ��*/
	QString getName() const;		//��Ʊ����
	float getIncrease() const;		//�Ƿ�
	float getVolumeRatio() const;	//����
	float getTurnRatio() const;		//������
	float getLastClose() const;		//��һ�����̼�
	float getOpenPrice() const;		//���տ��̼�
	float getHighPrice() const;		//������߼�
	float getLowPrice() const;		//������ͼ�
	float getNewPrice() const;		//���¼�
	float getTotalVolume() const;	//��ȡ����
	float getTotalAmount() const;	//��ȡ����
	float getNowVolume() const;		//��ȡ����
	float getIncSpeed() const;		//����
	float getPriceFluctuate() const;//�ǵ����۸񲨶�
	float getAmplitude() const;		//���
	float getAvePrice() const;		//����
	float getPERatio() const;		//��ӯ��
	float getLTSZ() const;			//��ͨ��ֵ
	float getZGB() const;			//�ܹɱ�
	float getSellVOL() const;		//������
	float getBuyVOL() const;		//������
	float getBIDVOL() const;		//ί����
	float getASKVOL() const;		//ί����
	float getBIDVOL3() const;		//ί����
	float getASKVOL3() const;		//ί����
	float getCommRatio() const;		//ί��
	float getCommSent() const;		//ί��

	float getLast5Volume();			//��ȡ��ȥ5�յĳɽ��������ڼ������ȣ�

	float getMgjzc();				//ÿ�ɾ��ʲ�
	float getMgsy();				//��ȡÿ������


	/*�жϹؼ���_key�Ƿ�ƥ��*/
	bool isMatch(const QString& _key);

	virtual bool isIndex(){ return true; }

protected:
	virtual void updateItemInfo(){}

private:
	bool appendStocks(QList<QString> list);				//��������ӹ�Ʊ
	bool removeStocks(QList<QString> list);				//�Ӱ����ɾ����Ʊ
	bool appendBlock(CBlockInfoItem* pBlock);			//��Ӱ��
	bool removeBlock(CBlockInfoItem* pBlock);			//�Ƴ����


protected slots:
	void stockFenbiChanged(const QString& _code);	//ĳֻ��Ʊ�ķֱ����ݷ����ı�
	void stockHistoryChanged(const QString& _code);	//ĳֻ��Ʊ����ʷ���ݷ����ı�
	void updateData();								//��������

private:
	void addStock(CStockInfoItem* p);				//��ӹ�Ʊ
	void removeStock(CStockInfoItem* p);			//ɾ����Ʊ
	void saveBlockFile();							//���浽����ļ�
	void clearTmpData();							//������ʱ�ڴ档

private:
	QString blockName;							//�������
	QString blockFilePath;						//������ݵĴ洢·��
	QList<CStockInfoItem*> stocksInBlock;		//�ð���µĹ�Ʊ��Ϣ
	QMap<QString,CBlockInfoItem*> blocksInBlock;//�ð���µ��Ӱ��

	QTimer timerUpdate;


	CBlockInfoItem* m_pParent;

	RBlockData* pCurrent5Min;		//��ǰ��5��������

	QMap<CStockInfoItem*,float> mapLastPrice;	//�����бȽϵ�����
	QMap<CStockInfoItem*,float> mapLast5Price;	//���µ�5���Ӽ۸�

	float fLastMinVolume;						//���5���ӵĳɽ���
	float fLastMinAmount;						//���5���ӵĳɽ���

	float fLast5MinVolume;						//���5���ӵĳɽ���
	float fLast5MinAmount;						//���5���ӵĳɽ���

	QString qsRegExp;							//ƥ���������ʽ
private:
	float fLastClose;				//�������̼�
	float fOpenPrice;				//���տ��̼�
	float fNewPrice;				//���¼�
	float fLowPrice;				//��ͼ�
	float fHighPrice;				//��߼�
	float fVolume;					//�ɽ���
	float fAmount;					//�ɽ���

	float fIncrease;				//�Ƿ�

	float fLTSZ;					//��ͨ��ֵ
	float fLTG;						//����ͨ��

	QList<QList<QChar>> shortName;	//��ƴ��
};

#endif	//BLOCK_INFO_ITEM_H