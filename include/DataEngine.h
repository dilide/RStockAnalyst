#ifndef DATA_ENGINE_H
#define DATA_ENGINE_H

#include "StockInfoItem.h"
#include "BlockInfoItem.h"
#include "rsd_global.h"
#include "RStockFunc.h"

#define	R_TIME_ZONE	8
typedef void (*SplashMessage)(const QString&,int);

class RSDSHARED_EXPORT CDataEngine : public QObject
{
	Q_OBJECT
public:
	CDataEngine(void);
	~CDataEngine(void);
	static CDataEngine* getDataEngine();
	static void releaseDataEngine();

public:
	//ͨ���г����ͻ�ȡ�г�����
	static QString getMarketStr(WORD wMarket);
	//��������ʱ�����ݵĳ�ʼ��
	static void importData();
	//�����˳�ʱ�����ݵ��Զ�����
	static void exportData();
	//�Ƿ����ڼ�������
	static bool isLoading();


	/*���ݵ��뺯��*/
	//����ָ������Ϣ
	static int importIndexStocks(const QString& qsFile);
	//F10���ݵ���,����ֵΪ������������������Ӻ�׺��Ϊ.fin���ļ����롣
	static int importBaseInfoFromFinFile(const QString& qsFile);
	//F10���ݵ��룬�ӱ�Ӧ��֧�ֵ����ݵ��룬��׺��Ϊ .rsafin
	static int importBaseInfo(const QString& qsFile);
	/*����Reports����*/
	static int importReportsInfo(const QString& qsFile);
	/*����ֱ�����*/
	static int importFenBisData(const QString& qsFile);
	/*����������*/
	static int importBlocksData(const QString& qsPath);

	//Ϊ��ʼ���İ���ȡReport
	static qRcvReportData* getReportForInitBlock(const QString& qsOnly);

	//��ȡ��Ʊ�����Ƿ�Ϊָ����
	static bool isIndexStock(const QString& qsCode);

	/*���ݵ�������*/
	/*���������������ݣ�F10����*/
	static int exportBaseInfo(const QString& qsFile);
	/*����Reports����*/
	static int exportReportsInfo(const QString& qsFile);
	/*�����ֱ�����*/
	static int exportFenBisData(const QString& qsFile);
	/*���̺���������*/
	static int exportCloseData();

public:
	static bool isStockOpenDay(time_t tmDay);	//�ж�tmDay�Ƿ��У�ĳ������ڣ�����ʱ�䣩
	static bool isStockOpenTime(time_t tmMin);	//�ж�tmMin�Ƿ��У�ĳ���ʱ�䣬�������ڣ�
	static time_t getOpenSeconds();				//��ȡÿ��Ŀ���ʱ�䣨�룩��һ��Ϊ4Сʱ
	static time_t getOpenSeconds(time_t tmTime);//��ȡ���������tmTime�Ŀ���ʱ�䣨�룩
	static time_t getCurrentTime();				//��ȡ���һ��report���ݵ�ʱ��
	static void setCurrentTime(const time_t& t);//�������һ��report���ݵ�ʱ��

	static QMap<time_t,int> getTodayTimeMap(RStockCircle _c, time_t tmNow = 0);		//��ȡ�����ʱ����
	static QMap<time_t,int> getHistoryTimeMap(RStockCircle _c,int iCount);			//��ʷ���ݵ�ʱ����
	/*���ֱ����ݰ���ʱ��������*/
	static QMap<time_t,RStockData*>* getColorBlockItems(const QMap<time_t,int>& mapTimes, const QList<qRcvFenBiData*>& minutes);
	/*�����߰���ʱ��������*/
	static QMap<time_t,RStockData*>* getColorBlockItems(const QMap<time_t,int>& mapTimes, const QList<qRcvHistoryData*>& minutes);

	static SplashMessage messageShowed;
public:
	QString getStockBlockDir() const{ return m_qsBlocksDir; }

public:
	//��ȡ�������
	QList<CBlockInfoItem*> getTopLevelBlocks();	//��ȡ�������б�
	QList<CBlockInfoItem*> getStockBlocks();	//��ȡ���а���б�
	CBlockInfoItem* getStockBlock(const QString& qsCode);		//ͨ��������ƻ�ȡ���
	bool isHadBlock(const QString& block);		//�Ƿ����ĳ���
	void setBlockInfoItem(CBlockInfoItem* _p);						//���ð������
	void removeBlockInfoItem(CBlockInfoItem* _p);				//ɾ���������
	CBlockInfoItem* getCustomBlock();				//��ȡ����ѡ��顱

	//��ȡ��ֻ��Ʊ����
	QList<CStockInfoItem*> getStocksByMarket(WORD wMarket);			//�����г����ͻ�ȡ��Ʊ�б�
	QList<CStockInfoItem*> getStockInfoList();
	CStockInfoItem* getStockInfoItem(const QString& qsOnly);
	CStockInfoItem* getStockInfoItemByCode(const QString& qsCode);
	void setStockInfoItem(CStockInfoItem* p);

	//��ȡ��Ʊ������飩����
	CAbstractStockItem* getStockItem(const QString& qsCode);
	QList<CAbstractStockItem*> getStockItems();

	//��������
	void appendNews(const QString& title, const QString& content);		//�������
	//F10����
	void appendF10(const QString& title, const QString& content);		//���F10����
	//��ʾF10����
	bool showF10Data(const QString& code);

	/*������������*/
	bool exportHistoryData(CAbstractStockItem* pItem, const QList<qRcvHistoryData*>& list);
	bool exportHistoryData(CAbstractStockItem* pItem, const QList<qRcvHistoryData*>& list, int iOffset);
	/*��ȡĳֻ��Ʊ����������*/
	QList<qRcvHistoryData*> getHistoryList(CAbstractStockItem* pItem);
	/*��ȡĳֻ��Ʊ���count������������*/
	QList<qRcvHistoryData*> getHistoryList(CAbstractStockItem* pItem, int count);

	/*�������̺���5��������*/
	bool exportMinData(CAbstractStockItem* pItem);
	/*��ȡ5��������*/
	void importMinData(CAbstractStockItem* pItem, QMap<time_t,RStockData*>& mapDatas);


	/*�����ֱ�����*/
	bool exportFenBiData(const QString& qsCode, const long& lDate, const QList<qRcvFenBiData*>& list);

private:
	QMap<QString,CStockInfoItem*> m_mapStockInfos;			//��Ʊ����
	QMap<QString,CBlockInfoItem*> m_mapBlockInfos;			//�������
	QList<CBlockInfoItem*> m_listTopLevelBlocks;			//������

	CBlockInfoItem* m_pBlockCustom;							//����ѡ��顱

private:
	static CDataEngine* m_pDataEngine;
	static time_t m_tmCurrentDay;
	static time_t m_tmCurrent;					//���һ��report���ݵ�ʱ��
	QString m_qsHistroyDir;						//�������ݴ洢��·�� AppDir/data/history/...
	QString m_qsBlocksDir;						//������ݵĴ洢·�� AppDir/data/blocks/...
	QString m_qsNewsDir;						//�������ݵĴ洢·�� AppDir/data/news/...
	QString m_qsF10Dir;							//F10���ݵĴ洢·�� AppDir/data/F10/...
	QString m_qsMin;							//5�������ݵĴ洢·�� AppDir/data/5Min/...
	QString m_qsFenBiDir;						//�ֱ����ݵĴ洢·�� AppDir/data/FenBi/Date/...



	//���ڴ洢��ʼ���صİ��Report���ݣ�����ʼ����ɺ�ɾ��������
	static QMap<QString,qRcvReportData*> m_mapReportForBlock;
	static bool m_bLoading;						//�Ƿ����ڼ��ء�����
	static QMap<QString,int> m_mapIndexStocks;	//���ص�ָ����
};


#endif	//DATA_ENGINE_H