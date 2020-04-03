#ifndef _RSTOCK_FUNC_H_
#define _RSTOCK_FUNC_H_
#include <QtCore>
#include "AbstractStockItem.h"

#ifndef FLOAT_NAN
#define	FLOAT_NAN	(std::numeric_limits<float>::quiet_NaN())
#endif

/*
RStockAnalyst����չ��������
*/

#pragma   pack(push,1)					//�����ڴ���뷽ʽΪ 1�ֽ�
// ��������
enum RStockCircle			//��������
{
	AutoCircle = 0,			//�Զ�ģʽ
	FenShi = 1,				//��ʱģʽ��ֱ�����ߣ�
	Sec3 = 3,				//10��
	Sec6 = 6,				//30��
	Sec12 = 12,				//30��
	Min1 = 60,				//1����
	Min5 = 5*60,			//5����
	Min15 = 15*60,			//15����
	Min30 = 30*60,			//30����
	Min60 = 60*60,			//60����
	MinN,					//N����
	Day,					//����
	DayN,					//N����
	Week,					//����
	Month,					//����
	Month3,					//����
	Year,					//����
};

//��������
enum RWidgetType
{
	WidgetBasic = 0,				//����ͼ
	WidgetKLine,					//K��ͼ
	WidgetMarketTrend,				//�г�����ͼ
	WidgetSColorBlock,				//��Ʊ��ɫ��ͼ
	WidgetBColorBlock,				//����ɫ��ͼ
	WidgetStockInfo,				//��ֻ��Ʊ��������Ϣ
	WidgetRadar,					//�״����ͼ
	WidgetStockDeal,				//��������ͼ
	WidgetAllStock,					//���й�Ʊ��ʾͼ
	WidgetFlash,					//Flash�л�����
};


//���ڵķ���
enum RWidgetInsert
{
	InsertLeft =1,			//�����
	InsertRight,				//�Ҳ���
	InsertTop,				//�ϲ���
	InsertBottom,			//�²���
	InsertRemove,			//ɾ��
};

struct qRcvReportData
{
	time_t	tmTime;			//�ɽ�ʱ��
	WORD		wMarket;		//��Ʊ�г�����
	QString		qsCode;			//��Ʊ����
	QString		qsName;			//��Ʊ����

	float	fLastClose;			//����
	float	fOpen;				//��
	float	fHigh;				//���
	float	fLow;				//���
	float	fNewPrice;			//����
	float	fVolume;			//�ɽ���
	float	fAmount;			//�ɽ���
	float	fBuyPrice[3];		//�����1,2,3
	float	fBuyVolume[3];		//������1,2,3
	float	fSellPrice[3];		//������1,2,3
	float	fSellVolume[3];		//������1,2,3

	float	fBuyPrice4;			//�����4
	float	fBuyVolume4;		//������4
	float	fSellPrice4;		//������4
	float	fSellVolume4;		//������4

	float	fBuyPrice5;			//�����5
	float	fBuyVolume5;		//������5
	float	fSellPrice5;		//������5
	float	fSellVolume5;		//������5

	qRcvReportData()
	{
		tmTime = NULL;
		wMarket = NULL;
		memset(&fLastClose,0,sizeof(float)*27);
	}
	qRcvReportData(RCV_REPORT_STRUCTExV3* p)
	{
		tmTime = p->m_time;
		wMarket = p->m_wMarket;
		qsCode = QString::fromLocal8Bit(p->m_szLabel);
		qsName = QString::fromLocal8Bit(p->m_szName);

		//ֱ�ӿ���ʣ�������float����
		memcpy(&fLastClose,&p->m_fLastClose,sizeof(float)*27);
	}

	void resetItem(RCV_REPORT_STRUCTExV3* p)
	{
		tmTime = p->m_time;
		wMarket = p->m_wMarket;
		qsCode = QString::fromLocal8Bit(p->m_szLabel);
		qsName = QString::fromLocal8Bit(p->m_szName);

		//ֱ�ӿ���ʣ�������float����
		memcpy(&fLastClose,&p->m_fLastClose,sizeof(float)*27);
	}
	void resetItem(qRcvReportData* p)
	{
		tmTime = p->tmTime;
		wMarket = p->wMarket;
		qsCode = p->qsCode;
		qsName = p->qsName;

		//ֱ�ӿ���ʣ�������float����
		memcpy(&fLastClose,&p->fLastClose,sizeof(float)*27);
	}
};

struct qRcvHistoryData
{
	time_t	time;			//UCT
	float	fOpen;			//����
	float	fHigh;			//���
	float	fLow;				//���
	float	fClose;			//����
	float	fVolume;			//��
	float	fAmount;			//��
	WORD	wAdvance;			//����,��������Ч
	WORD	wDecline;			//����,��������Ч

	qRcvHistoryData()
	{

	}
	qRcvHistoryData(RCV_HISTORY_STRUCTEx* p)
	{
		memcpy(&time,&p->m_time,sizeof(qRcvHistoryData));
	}
	qRcvHistoryData(qRcvReportData* p)
	{
		time = QDateTime(QDateTime::fromTime_t(p->tmTime).date()).toTime_t();
		fAmount = p->fAmount;
		fClose = p->fNewPrice;
		fHigh = p->fHigh;
		fLow = p->fLow;
		fOpen = p->fOpen;
		fVolume = p->fVolume;
	}
};

struct qRcvBaseInfoData
{
	char code[STKLABEL_LEN];
	WORD wMarket;

	float fZgb;                    //�ܹɱ�(���)			0
	float fGjg;                    //���ҹ�(���)
	float fFqrfrg;                  //�����˷��˹�(���)
	float fFrg;                    //���˹�(���)
	float fZgg;                   //ְ����(���)
	float fGzAg;                  //����A��(���)			5
	float fLtAg;                  //��ͨA��(���)
	float fHg;                    //�ȹ�(���)
	float fBg;                    //B��(���)
	float fZpg;                   //ת���(���)
	float fZzc;                   //���ʲ�(��Ԫ)			10
	float fLdzc;                  //�����ʲ�(��Ԫ)
	float fCqtz;                  //����Ͷ��(��Ԫ)
	float fGdzc;                  //�̶��ʲ�(��Ԫ)
	float fWxzc;                 //�����ʲ�(��Ԫ)
	float fLdfz;                  //������ծ(��Ԫ)		15
	float fCqfz;                 //���ڸ�ծ(��Ԫ)
	float fGgqy;                 //�ɶ�Ȩ��(��Ԫ)
	float fZbgjj;                 //�ʱ�������(��Ԫ)
	float fYygjj;                 //ӯ�๫����(��Ԫ)
	float fMgjz;                 //ÿ�ɾ�ֵ(Ԫ)			20
	float fGdqybl;               //�ɶ�Ȩ�����(%)
	float fMggjj;                //ÿ�ɹ�����(Ԫ)
	float fZyywsr;               //��Ӫҵ������(��Ԫ)
	float fZyywlr;               //��Ӫҵ������(��Ԫ)
	float fQtywlr;               //����ҵ������(��Ԫ)		25
	float fLrze;                 //�����ܶ�(��Ԫ)
	float fJlr;                  //������(��Ԫ)
	float fWfplr;               //δ��������(��Ԫ)
	float fMgsy;                //ÿ������(Ԫ)
	float fJzcsyl;                //���ʲ�������(%)		30
	float fMgwfplr;              //ÿ��δ��������(Ԫ)
	float fMgjzc;                //ÿ�ɾ��ʲ�(Ԫ)			32

	qRcvBaseInfoData()
	{
		memset(code,0,sizeof(qRcvBaseInfoData));
	}

	qRcvBaseInfoData( float* fVal )
	{
		fVal = fVal+1;
		fZgb = fVal[0];
		fGjg = fVal[1];
		fFqrfrg = fVal[2];
		fFrg = fVal[3];
		fBg = fVal[4];

		fHg = fVal[5];
		fLtAg = fVal[6];
		fZgg = fVal[7];

		fZzc = fVal[9]/10;

		fLdzc = fVal[10]/10;
		fGdzc = fVal[11]/10;
		fWxzc = fVal[12]/10;
		fCqtz = fVal[13]/10;
		fLdfz = fVal[14]/10;

		fCqfz = fVal[15]/10;
		fZbgjj = fVal[16]/10;
		fMggjj = fVal[17];
		fGgqy = fVal[18]/10;
		fZyywsr = fVal[19]/10;

		fZyywlr = fVal[20]/10;		//?
		fQtywlr = fVal[21]/10;
		fZyywlr = fVal[22]/10;		//?

		fLrze = fVal[27]/10;

		fJlr = fVal[29]/10;

		fWfplr = fVal[30]/10;
		fMgwfplr  = fVal[31];
		fMgsy = fVal[32];
		fMgjzc = fVal[33];

		fGdqybl  = fVal[35];
		fJzcsyl = fVal[36];
	}
};

struct qRcvPowerData
{
	time_t	tmTime;				// UCT
	float	fGive;			// ÿ����
	float	fPei;				// ÿ����
	float	fPeiPrice;		// ��ɼ�,���� m_fPei!=0.0f ʱ��Ч
	float	fProfit;			// ÿ�ɺ���

	qRcvPowerData(RCV_POWER_STRUCTEx* p)
	{
		memcpy(&tmTime,&p->m_time,sizeof(qRcvPowerData));
	}
};

//RCV_FENBI_STRUCTEx
struct qRcvFenBiData
{
	time_t	tmTime;				// hhmmss ����93056 ����9:30:56���̿�����
	float	fPrice;			// ����
	float	fVolume;			// �ɽ���
	float	fAmount;			// �ɽ���

	float	fBuyPrice[5];		// �����1,2,3,4,5
	float	fBuyVolume[5];		// ������1,2,3,4,5
	float	fSellPrice[5];		// ������1,2,3,4,5
	float	fSellVolume[5];		// ������1,2,3,4,5
	qRcvFenBiData()
	{
		memset(&tmTime,0,sizeof(qRcvFenBiData));
	}
	qRcvFenBiData(RCV_FENBI_STRUCTEx* p, const long& lDate)
	{
		//��ԭ����ʱ��ת��Ϊtime_t
		long l = lDate;
		int iD = l%100;
		l = l/100;
		int iM = l%100;
		l = l/100;
		int iY = l;

		l = p->m_lTime;
		int iS = l%100;
		l = l/100;
		int iMin = l%100;
		l = l/100;
		int iH = l;

		QDateTime tmT(QDate(iY,iM,iD),QTime(iH,iMin,iS));
		tmTime = tmT.toTime_t();
		fPrice = p->m_fNewPrice;
		fVolume = p->m_fVolume;
		fAmount = p->m_fAmount;

		memcpy(&fBuyPrice[0],&p->m_fBuyPrice[0],sizeof(float)*20);
	}
	qRcvFenBiData(RCV_MINUTE_STRUCTEx* p)
	{
		memset(&tmTime,0,sizeof(qRcvFenBiData));
		//ͨ���������ݹ��� �ֱ�����
		tmTime = p->m_time;
		fPrice = p->m_fPrice;
		fVolume = p->m_fVolume;
		fAmount = p->m_fAmount;
	}
	qRcvFenBiData(qRcvReportData* p)
	{
		tmTime = p->tmTime;
		fPrice = p->fNewPrice;
		fVolume = p->fVolume;
		fAmount = p->fAmount;

		memcpy(&fBuyPrice[0],&p->fBuyPrice[0],sizeof(float)*3);
		memcpy(&fBuyVolume[0],&p->fBuyVolume[0],sizeof(float)*3);
		memcpy(&fSellPrice[0],&p->fSellPrice[0],sizeof(float)*3);
		memcpy(&fSellVolume[0],&p->fSellVolume[0],sizeof(float)*3);
		fBuyPrice[3] = p->fBuyPrice4; fBuyPrice[4] = p->fBuyPrice5;
		fBuyVolume[3] = p->fBuyVolume4; fBuyVolume[4] = p->fBuyVolume5;
		fSellPrice[3] = p->fSellPrice4; fSellPrice[4] = p->fSellPrice5;
		fSellVolume[3] = p->fSellVolume4; fSellVolume[4] = p->fSellVolume5;
	}
};

// ��������
typedef struct tagRStockData
{
	time_t	tmTime;			//UCT
	float	fOpen;			//���� ע����ʱ�����д����ݱ�ʾ�����ڿ�ʼ��
	float	fHigh;			//���
	float	fLow;				//���
	float	fClose;			//���� ע����ʱ�����д����ݱ�ʾ���������ļ۸�
	float	fVolume;			//��
	float	fAmount;			//��
	WORD	wAdvance;			//����,��������Ч
	WORD	wDecline;			//����,��������Ч

	tagRStockData()
	{
		memset(&tmTime,0,sizeof(tagRStockData));
	}
	tagRStockData(RCV_HISTORY_STRUCTEx* p)
	{
		memcpy(&tmTime,&p->m_time,sizeof(tagRStockData));
	}
	tagRStockData(const qRcvReportData* p)
	{
		tmTime = QDateTime(QDateTime::fromTime_t(p->tmTime).date()).toTime_t();
		fAmount = p->fAmount;
		fClose = p->fNewPrice;
		fHigh = p->fHigh;
		fLow = p->fLow;
		fOpen = p->fOpen;
		fVolume = p->fVolume;
	}
} RStockData;


//���ڰ�����ݵĴ��
typedef struct tagRcvBlockData : public tagRStockData
{
	float	fIncrease[21];	//21���ǵ���
	tagRcvBlockData()
	{
		memset(&tmTime,0,sizeof(tagRcvBlockData));
	}
} RBlockData;

// ��չ����,���������ֱʳɽ����ݵ�������
typedef union tagRStockDataEx
{
	struct
	{
		float m_fBuyPrice[5];		//��1--��5��
		float m_fBuyVol[5];			//��1--��5��
		float m_fSellPrice[5];		//��1--��5��        
		float m_fSellVol[5];		//��1--��5�� 
	};
	float m_fDataEx[20];
} RStockDataEx;


// ��Ȩ����
typedef struct tagRSplitData
{
	time_t	m_time;	//ʱ��,UCT
	float	m_fHg;	//���
	float	m_fPg;	//���
	float	m_fPgj;	//��ɼ�
	float	m_fHl;	//����
} RSpliteData;


// ���ýӿ���Ϣ���ݽṹ
typedef struct tagRCalcInfo
{
	DWORD dwVersion;		//�汾
	RStockCircle emCircle;		//��������
	CAbstractStockItem* pItem;

	QMap<time_t,RStockData*>* mapData;	//��������,����ΪNULL
	QMap<time_t,RStockDataEx*>* mapDataEx;	//��չ����,�ֱʳɽ�������,ע��:����Ϊ NULL

	tagRCalcInfo()
	{
		memset(&dwVersion,0,sizeof(tagRCalcInfo));
	}
} RCalcInfo;


typedef class QPainter QPainter;
typedef class QRect QRect;
typedef struct tagRDrawInfo
{
	DWORD dwVersion;
	QPainter* pPainter;
	int iEndIndex;				//��������������
	float fItemWidth;			//�������ݵĻ��ƿ��
	float fMin;
	float fMax;
	int iCurColor;				//��ǰʹ�õ���ɫ����
	QList<uint> lsColors;		//��������Ҫ����ɫ��
	QRectF rtClient;

	tagRDrawInfo()
	{
		memset(&dwVersion,0,28);
		fMin = 1.0;
		fMax = -1.0;
	}
} RDrawInfo;

#pragma   pack(pop)					//ȥ���ڴ���뷽ʽ����

#endif	//_RSTOCK_FUNC_H_