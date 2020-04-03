/****************************************************************************************
*								ͨ�Ӽ������ϣ�δ�������ù���							*	
****************************************************************************************/

/****************************************************************************************
*								 WIN95/98 ��Ʊ��������									*
*								 �����������ݹ㲥�о�����								*
*									     1998.3.5										*
*****************************************************************************************/

/****************************************************************************************
*					��ע���� HKEY_LOCAL_MACHINE/SOFTWARE/StockDrv ��					*
*					Driver  = "����DLLȫ·��"											*
*					Provider= "TongShi"													*
*																						*
*	������� LoadLibrary �� GetProcAddress �� FreeLibrary �ķ�ʽ���ù�Ʊ��������		*
*	��Ʊ�����������ļ��������λ���� ע��� HKEY_LOCAL_MACHINE/SOFTWARE/StockDrv		*
*	�� Driver ָ��																		*
*																						*
*																						*
*																						*
*	==========================		���ֽڶ������		===========================		*
*																						*
*																						*
****************************************************************************************/

#ifndef __STOCKDRV_100_H__
#define __STOCKDRV_100_H__

// ������ʽ���Ͷ���
#define RCV_WORK_MEMSHARE			2					// �ڴ湲��ֱ������
#define RCV_WORK_SENDMSG			4					// �汾 2 ����ʹ�õķ�ʽ	
// ����������ʽ����

#define	TS_StockDrv_IdTxt	"TongShi_StockDrv_1.00"		// ͨ����Ϣ��������


// ֤ȯ�г�
#define		SH_MARKET_EX			'HS'		// �Ϻ�
#define		SZ_MARKET_EX			'ZS'		// ����
#define     HK_MARKET_EX            'KH'        //����г�
#define     EB_MARKET_EX            'BE'        //���ڶ����г�


// �ļ���������
// �ṹ������ʽ���ļ�����
#define		FILE_HISTORY_EX			2			// ����������
#define		FILE_MINUTE_EX			4			// ������������
#define     FILE_POWER_EX           6
#define     FILE_HISTORY_MINUTE_EX        8          //����ʷ���������
#define     FILE_BASEINFO_EX             10          //����ȫ����Ʊ��������
#define     FILE_DISPARTBARGAINING_EX    12          //���ֱʽ�����ϸ����
#define     FILE_STOCKLABELLIST_EX       14          //����ȫ����Ʊ�����
#define     FILE_SYSTEMRUN_EX            16          //����ϵͳ���в���
#define     FILE_INDEXATTRIBUTE_EX       18          //���̺��̾� 
#define     FILE_LOGINAUTH_EX            20          //�ͻ�������Ȩ
#define     FILE_TECHNICINDEX            22          //�ͻ��˼���ָ��

#define		FILE_BASE_EX			0x1000		// Ǯ�����ݻ��������ļ�,m_szFileName�������ļ���
#define		FILE_NEWS_EX			0x1002		// ������,��������m_szFileName����Ŀ¼������
#define     FILE_HTML_EX            0x1004      //HTML �ļ���m_szFileNameΪURL
#define     FILE_ALLNEWSINFO_EX     0x1006      //�г�����ȫ��������Ϣ 
#define     FILE_NOTIFY_EX          0x1008      //֪ͨ��Ϣ
#define     FILE_TECHNIC_EX         0x1010      //����˼���ָ���ļ�    
#define     FILE_SOFTWARE_EX        0x2000      //�������      

#define		FILE_TYPE_RES			-1			// ����

// ��Ϣ������
#define		News_Sha_Ex				 2			// ��֤��Ϣ
#define		News_Szn_Ex				 4			// ��֤��Ϣ
#define		News_Fin_Ex				 6			// �ƾ�����
#define		News_TVSta_Ex			 8			// ����̨֪ͨ
#define     News_SzEb_Ex            10          // ���ڶ���
#define		News_Unknown_Ex			 -1			// δ֪�ṩ��

//Definition For nInfo of Function GetStockDrvInfo(int nInfo,void * pBuf);
#define		RI_IDSTRING				1			// ��������,����(LPCSTR)������
#define		RI_IDCODE				2			// ����
#define		RI_VERSION				3			// ��������汾
#define		RI_ERRRATE				4			// ȡ�ŵ�����
#define		RI_STKNUM				5			// ȡ���й�Ʊ�ܼ���
#define		RI_SUPPORTEXTHQ			 6

#define		STKLABEL_LEN			10			// �ɺ����ݳ���,�����г��ɺű������Ǯ��
#define		STKNAME_LEN				32			// ��������
#define     STKPINGYING_LEN          6


//----------------------------------------ϵͳ���в������-----------------------------------------------
typedef  struct  tagRCV_SYSTEMRUN_STRUCTEx
{
	BYTE  m_btBargainingTime[3];      //���̷����� 
	BYTE  m_btTodayInitialize;        //�����Ƿ��ʼ�� 
	BYTE  m_btCloseWork;              //�����Ƿ�����  
	long  m_BargainingDate;           //��������  
	float   m_fNewIndex[3];           //����ָ�� 
	float   m_fVolume[3];             //���³ɽ���
	float   m_fAmount[3];             //���³ɽ����
	float   m_fRedGreenArmy[3];       //���̺��̾�
}RCV_SYSTEMRUN_STRUCTEx;
//--------------------------------------------------------------------------
typedef struct tagRCV_LOGINAUTH_STRUCTEx
{
	BOOL  bAuthorizationState;             
	DWORD dwAuthorizationPurview;                 
}RCV_LOGINAUTH_STRUCTEx;

//----------------------------------------��Ʊ�����-------------------------------------------------------
typedef  struct  tagRCV_STKLABEL_STRUCTEx
{
	WORD m_wMarket;                   //��Ʊ�г�����
	WORD m_wPos;                      //����һ�г������й�Ʊ����λ�� 
	char m_szLabel[STKLABEL_LEN];      //��Ʊ����
	char m_szName[STKNAME_LEN];       //��Ʊ����
	char m_szPingYing[STKPINGYING_LEN]; //��Ʊ����ƴ��
	float m_fLastClose;                    //����
	float m_fAverageVolumn;               //5����� 
}RCV_STKLABEL_STRUCTEx;
//-----------------------------------------------------
typedef struct tagRCV_INDEXATTRIBUTE_STRUCTEx
{
	time_t m_time;         
    short sec5;            
    short rp;              
    short dp;             
	short rsn;             
	short dnn;
}RCV_INDEXATTRIBUTE_STRUCTEx;
//---------------------------------------------------------------------------------------
typedef  struct tagRCV_BASEINFO_STRUCTEx
{
	char m_szLabel[STKLABEL_LEN];  //��Ʊ����
	float m_fZgb;                    //�ܹɱ�(���)
	float m_fGjg;                    //���ҹ�(���)
	float m_fFqrfrg;                  //�����˷��˹�(���)
	float m_fFrg;                    //���˹�(���)
	float m_fZgg;                   //ְ����(���)
	float m_fGzAg;                  //����A��(���)
	float m_fLtAg;                  //��ͨA��(���)
	float m_fHg;                    //�ȹ�(���)
	float m_fBg;                    //B��(���)
	float m_fZpg;                   //ת���(���)
	float m_fZzc;                   //���ʲ�(��Ԫ)
	float m_fLdzc;                  //�����ʲ�(��Ԫ)
	float m_fCqtz;                  //����Ͷ��(��Ԫ)
	float m_fGdzc;                  //�̶��ʲ�(��Ԫ)
	float m_fWxzc;                 //�����ʲ�(��Ԫ)
	float m_fLdfz;                  //������ծ(��Ԫ)
	float m_fCqfz;                 //���ڸ�ծ(��Ԫ)
	float m_fGgqy;                 //�ɶ�Ȩ��(��Ԫ)
	float m_fZbgjj;                 //�ʱ�������(��Ԫ)
	float m_fYygjj;                 //ӯ�๫����(��Ԫ)
	float m_fMgjz;                 //ÿ�ɾ�ֵ(Ԫ)
	float m_fGdqybl;               //�ɶ�Ȩ�����(%)
	float m_fMggjj;                //ÿ�ɹ�����(Ԫ)
	float m_fZyywsr;               //��Ӫҵ������(��Ԫ)
	float m_fZyywlr;               //��Ӫҵ������(��Ԫ)
	float m_fQtywlr;               //����ҵ������(��Ԫ)
	float m_fLrze;                 //�����ܶ�(��Ԫ)
	float m_fJlr;                  //������(��Ԫ)
	float m_fWfplr;               //δ��������(��Ԫ)
	float m_fMgsy;                //ÿ������(Ԫ)
	float m_fJzcsyl;                //���ʲ�������(%)
	float m_fMgwfplr;              //ÿ��δ��������(Ԫ)
	float m_fMgjzc;                //ÿ�ɾ��ʲ�(Ԫ)
}RCV_BASEINFO_STRUCTEx;
//////////////////////////////////////////////////////////////////////////////////
//��������ͷ
//����ͷ m_dwHeadTag == EKE_HEAD_TAG 
#define EKE_HEAD_TAG	0xffffffff

typedef struct	tagRCV_EKE_HEADEx
{
	DWORD	m_dwHeadTag;								// = EKE_HEAD_TAG
	WORD	m_wMarket;									// �г�����
	char	m_szLabel[STKLABEL_LEN];					// ��Ʊ����
}RCV_EKE_HEADEx;

//-------------------------------------������ʷ�����K������----------------------------------------
typedef union tagRCV_HISMINUTE_STRUCTEx
{
	struct
	{
		time_t  m_time;                  //UCT
		float  m_fOpen;			          //����
		float	m_fHigh;			          //���
		float  m_fLow;                    //���
		float  m_fClose;                   //����
		float  m_fVolume;                 //��
		float  m_fAmount;                 //��
		float   m_fActiveBuyVol;          //����������û�м���m_fActiveBuyVol=0
	};
	RCV_EKE_HEADEx  m_head;
}RCV_HISMINUTE_STRUCTEx;
//------------------------------------------------����ϸ����-----------------------------------------------
typedef  union  tagRCV_DISPBARGAINING_STRUCTEx
{
	struct
	{
		time_t m_time;                      //�ɽ�ʱ�� 
		float  m_fNewPrice;                 // ����
		float  m_fVolume;                  // �ɽ���,�Թ�Ϊ��λ,�ɵĺ�������Ʊ���Ͷ���
		float  m_fAmount;                  //�ɽ���,��ԪΪ��λ
		float  m_fBuyPrice[3];              // �����1,2,3
		float  m_fBuyVolume[3];            // ������1,2,3
		float  m_fSellPrice[3];              // ������1,2,3
		float m_fSellVolume[3];             // ������1,2,3
	};
	RCV_EKE_HEADEx  m_head;
}RCV_DISPBARGAINING_STRUCTEx;
//-----------------------------�����Ȩ���ݽṹ---------------------------------------------------------
typedef union tagRCV_POWER_STRUCTEx
{
	struct
	{
		time_t m_time;             //UCT
		float m_fGive;             //ÿ����  
		float m_fPei;              //ÿ����   
		float m_fPeiPrice;          //��ɼ�   
		float m_fProfit;            //ÿ�ɺ��� 
	};
	RCV_EKE_HEADEx  m_head;
}RCV_POWER_STRUCTEx;

//////////////////////////////////////////////////////////////////////////////////
//��������
//ע:
//		ĳЩ�ֶο���Ϊ0,���ϴε����ݴ���
typedef struct tagRCV_REPORT_STRUCTEx
{
	WORD	m_cbSize;									// �ṹ��С
	time_t	m_time;										// ����ʱ��
	WORD	m_wMarket;									// ��Ʊ�г�����
	char	m_szLabel[STKLABEL_LEN];					// ��Ʊ����,��'\0'��β
	char	m_szName[STKNAME_LEN];						// ��Ʊ����,��'\0'��β
	
	float	m_fLastClose;								// ����
	float	m_fOpen;									// ��
	float	m_fHigh;									// ���
	float	m_fLow;										// ���
	float	m_fNewPrice;								// ����
	float	m_fVolume;									// �ɽ���,�Թ�Ϊ��λ,�ɵĺ�������Ʊ���Ͷ���
	float	m_fAmount;									// �ɽ���,��ԪΪ��λ

	float	m_fBuyPrice[3];								// �����1,2,3
	float	m_fBuyVolume[3];							// ������1,2,3
	float	m_fSellPrice[3];							// ������1,2,3
	float	m_fSellVolume[3];							// ������1,2,3

	
	float	m_fBuyPrice4;								// �����4
	float	m_fBuyVolume4;								// ������4
	float	m_fSellPrice4;								// ������4
	float	m_fSellVolume4;								// ������4

} RCV_REPORT_STRUCTEx;


//============================�ɽṹ==============
typedef struct tagRCV_REPORT_STRUCTEx_OLD
{
	WORD	m_wMarket;									// ��Ʊ�г�����
	char	m_szLabel[STKLABEL_LEN];					// ��Ʊ����,��'\0'��β
	char	m_szName[STKNAME_LEN];						// ��Ʊ����,��'\0'��β
	
	float	m_fLastClose;								// ����
	float	m_fOpen;									// ��
	float	m_fHigh;									// ���
	float	m_fLow;										// ���
	float	m_fNewPrice;								// ����
	float	m_fVolume;									// �ɽ���,�Թ�Ϊ��λ,�ɵĺ�������Ʊ���Ͷ���
	float	m_fAmount;									// �ɽ���,��ԪΪ��λ

	float	m_fBuyPrice[3];								// �����1,2,3
	float	m_fBuyVolume[3];							// ������1,2,3
	float	m_fSellPrice[3];							// ������1,2,3
	float	m_fSellVolume[3];							// ������1,2,3
} RCV_REPORT_STRUCTEx_OLD;
//============================�ɽṹ===================
//--------------------------------����������ݽṹ-------------------------------------------------------
typedef struct tagRCV_WIDOFEREPORT_STRUCTEx
{
	RCV_REPORT_STRUCTEx *pMarketReport;
	float m_fActiveBuyVolumn;   //����������  
	float m_fConsignTotalBuyVolumn;  //ί�������� 
	float m_fConsignTotalSellVolumn; //ί��������
}RCV_WIDOFEREPORT_STRUCTEx;


//������������
//	ע:
//		ÿһ���ݽṹ��Ӧͨ�� m_time == EKE_HEAD_TAG,�ж��Ƿ�Ϊ m_head,Ȼ����������
typedef union tagRCV_HISTORY_STRUCTEx
{
	struct
	{
		time_t	m_time;				//UCT
		float	m_fOpen;			//����
		float	m_fHigh;			//���
		float	m_fLow;				//���
		float	m_fClose;			//����
		float	m_fVolume;			//��
		float	m_fAmount;			//��
		union
		{
			struct
			{
				WORD  m_wAdvance;    //����,��������Ч
				WORD  m_wDecline;     //����,��������Ч
			};
			float   m_fActiveBuyVol;      //��������
		};
	};
	RCV_EKE_HEADEx	m_head;
}RCV_HISTORY_STRUCTEx;

//�����ʱ������
//	ע:
//		ÿһ���ݽṹ��Ӧͨ�� m_time == EKE_HEAD_TAG,�ж��Ƿ�Ϊ m_head,Ȼ����������
typedef union tagRCV_MINUTE_STRUCTEx
{
	struct{
		time_t	m_time;				// UCT
		float	m_fPrice;
		float	m_fVolume;
		float	m_fAmount;
	};
	RCV_EKE_HEADEx	m_head; 
}RCV_MINUTE_STRUCTEx;

//////////////////////////////////////////////////////////////////////////////////
// �ļ��������ݰ�ͷ
//  עһ:
//	m_wDataType == FILE_BASE_EX
//				m_dwAttrib = ��Ʊ֤ȯ�г�,m_szFileName�������ļ���
//				m_FileTime = �����������ļ���������
//  m_wDataType == FILE_NEWS_EX
//				m_dwAttrib = ��Ϣ��Դ,m_szFileName ����Ŀ¼���ļ���,Ŀ¼��Ϊ��Ϣ��Դ
//				m_dwSerialNo = ���к�
//				��:  "�Ͻ�����Ϣ\\0501Z012.TXT","������\\XLD0001.TXT"
//  ע��:
//		�����ļ�ѭ������,ÿ���ļ���Ψһ�����к�,�Ա����ظ�����
typedef struct tagRCV_FILE_HEADEx
{
	DWORD	m_dwAttrib;							// �ļ�������
	DWORD	m_dwLen;							// �ļ�����
	union {
		DWORD	m_dwSerialNo;					// ���к�,�Թ���
		time_t	m_FileTime;						// �ļ�����ʱ��
	};
	char	m_szFileName[_MAX_PATH];			// �ļ��� or URL
} RCV_FILE_HEADEx;

//////////////////////////////////////////////////////////////////////////////////
//  ����֪ͨ��Ϣ
//	wParam = RCV_WPARAM;
//  lParam ָ�� RCV_DATA�ṹ;
//	���� 1 �Ѿ�����, 0 δ������ܴ���,Ŀǰ�÷���ֵ������

#define RCV_REPORT			0x3f001234
#define RCV_FILEDATA		0x3f001235

// עһ:
//	  ��¼����ʾ�������ݺͲ�������(���� Header)�����ݰ���,���ļ���������, = 1
// ע��:
//	  �� m_bDISK = FALSE, m_pData Ϊ��Ϣ���������ݻ�����ָ��
//
//		 ******** ���ݹ���,�����޸����� **********
//
//		 m_bDISK = TRUE,  m_pData Ϊ���ļ��Ĵ����ļ���,һ�����ֻ��
//		 ��������ȴ��ļ��ô��̷�ʽ
typedef struct tagRCV_DATA
{
	int					m_wDataType;			// �ļ�����
	int					m_nPacketNum;			// ��¼��,�μ�עһ
	RCV_FILE_HEADEx		m_File;					// �ļ��ӿ�
	BOOL				m_bDISK;				// �ļ��Ƿ��Ѵ��̵��ļ�
	union
	{
		RCV_REPORT_STRUCTEx *m_pReport;                   //��������
		RCV_WIDOFEREPORT_STRUCTEx *m_pWideReport;         //������������� 
		RCV_HISTORY_STRUCTEx *m_pDay;                     //�������� 
		RCV_MINUTE_STRUCTEx *m_pMinute;                   //�������ʱ����   
		RCV_POWER_STRUCTEx *m_pPower;                     //��Ȩ����
		RCV_DISPBARGAINING_STRUCTEx *m_pDispBargaining;  //��ϸ����
		RCV_BASEINFO_STRUCTEx *m_pBaseInfo;               //��������
		RCV_STKLABEL_STRUCTEx *m_pStkLabel;               //������ձ�
		RCV_HISMINUTE_STRUCTEx *m_pHisMinute;             //����ʷ�������
		RCV_SYSTEMRUN_STRUCTEx *m_pSystemRun;            //ϵͳ���в���
		RCV_INDEXATTRIBUTE_STRUCTEx *m_pIndexAttribute;   //���̺��̾� 
		RCV_LOGINAUTH_STRUCTEx *m_pLoginAuth;             //������Ȩ״̬       
		void *m_pData;                                    //�ļ��ڴ�ָ��   
	};
} RCV_DATA;

//==================ͬ����ͨѶ��ӿ������ݵ���ṹ=====================
typedef struct RCV_COMMINTERFACE_STRUCTEx
{
	RCV_DATA *m_pRcvData;                       //���鴫������
	DWORD m_dwTransEnvironment;                 //���鴫�ͻ������� 
} RCV_COMMINTERFACE_STRUCTEx;

////////////////////////////////////////////////////////
//�ͻ���Ӧ�ò��������󲿷�
//
typedef enum {
	SHZS=0,                           //��ָ֤�� 
    SHAG=1,                             //��֤A�� 
	SHBG=2,                             //��֤B�� 
	SZZS=3,                              //����ָ��
	SZAG=4,                              //����A��
	SZBG=5,                              //����B��
	SHZQ=6,                              //��֤ծȯ
	SZZQ=7,                              //����ծȯ
	SHJIJIN=8,                              //��֤jijin 
	SZJIJIN=9,                              //��֤jijin 
	EBZS=10,                              //����ָ��   
	EBAG=11,                              //�����Ʊ
	STKTYPE=12,                          //���ָ��  
	CHOOSESTK=20,                           //��ѡ�� 
	QBGP=30                              //ȫ����Ʊ
};
//---------------------------------------------------------------------------
typedef enum ClientTransmitAttributes{
	LoginAuthorization=0,            //������Ȩ 
    SystemRunAttribute,              //ϵͳ��������  
	InitStockCodeList,               //ȡ�ó�ʼ��ȫ����Ʊ��������   
    IndexRealtimeData,               //ȡ��ĳһ����ָ����ʱ��������1A0001��2A01   
	StockRealtimeData,               //ȡ��ĳһ��Ʊ��ʱ��������
	StockMinuteData,                 //ȡ��ĳһ��Ʊ�����������
    StockRealtimeDataKLine,          //ȡ��ĳһ��Ʊ��K�߽��漴ʱ��������
	StockDisPartData,                //ȡ��ĳһ��Ʊ�ֱ���ϸ����
    StockTypesBaseRealTimeData,      //ȡ��ĳһ���Ʊ������������ ĳһ���Ʊȫ����ߡ���͡���������  
    StockTypesRealTimeMarketData,    //ȡ��ĳһ���Ʊ��������   ĳһ���Ʊȫ�����¡��ɽ��������
    StockTypesRealTimeMarketWithMinute,      //ȡ��ĳһ���Ʊ��ʱ��������   ĳһ����ȫ����Ʊ��ʱ����
	HistoryDayLineData,              //���ڱ�ʾ��ʷ�������� 
	HistoryMinuteLineData,           //���ڱ�ʾ��ʷ��ʱ���� 
	HistoryDisPartBargainingData,    //���ڱ�ʾ��ʷ��ϸ����
	WideExRightData,                     //���ڱ�ʾ��Ȩ����
	WideFinancingData,                   //���ڱ�ʾ��������
	WideStockBaseInfo,                   //���ڱ�ʾ��������
    WideShInfomation,                    //���ڱ�ʾ��֤����
	WideSzInfomation,                    //���ڱ�ʾ��֤����  
    WideEbInfomation,                    //���ڱ�ʾ���幫��  
	WideZjInfomation,                    //���ڱ�ʾ�ƾ�����
	WideTechnicIndex,                    //���ڱ�ʾ����ָ�� 
	WideNotifyInfomation                 //���ڱ�ʾ֪ͨ��Ϣ
};
typedef struct tagTransmitStockAttribute
{
	WORD m_wMarket;                                  //��Ʊ�г�����
	char m_szTransmitStockLabel[8];                  //���͹�Ʊ����
	DWORD m_dwTransmitCounts;                        //��ʾ��¼��     
}TransmitStockAttribute, *PTransmitStockAttribute;   //���͹�Ʊ����ָ��
//========================================
typedef struct tagTRANS_PACKAGE_STRUCTEx
{
	ClientTransmitAttributes  m_TransmitType;    //�ͻ��������ݰ�����  
	DWORD m_dwTransmitStockType;                 //���ڱ�ʾ�������  
	union
	{
		struct
		{
			TransmitStockAttribute *m_pTransmitStockAttribute;
			DWORD m_dwTransmitStockCount;                        //��ʾ���͹�Ʊ��
		};
		DWORD m_dwTransmitStockTypeCounts;                       //��ʾ��¼��,���ڰ��
	};
} TRANS_PACKAGE_STRUCTEx;
/*�ṹ˵����m_TransmitType���ڱ�ʾ�ͻ����������������
m_dwTransmitStockType���ڱ�ʾ��鴫��
m_dwTransmitStockType== 	SHZS��ʾ��ָ֤�� 
m_dwTransmitStockType== SHAG,��ʾ��֤A�� 
m_dwTransmitStockType==SHBG,��ʾ��֤B�� 
m_dwTransmitStockType==SHZQ��ʾ��֤ծȯ
m_dwTransmitStockType==SZZS��ʾ����ָ��
m_dwTransmitStockType==SZAG,��ʾ����A��
m_dwTransmitStockType==SZBG,��ʾ����B��
m_dwTransmitStockType==SZZQ��ʾ����ծȯ
m_dwTransmitStockType==EBZS��ʾ����ָ��   
m_dwTransmitStockType==EBAG,��ʾ�����Ʊ
������ʾ��һ��Ʊ����Ʊ����
m_pTransmitStockAttributeΪ�ṹ����ָ�룬���ڱ�ʾ��һ��Ʊ����Ʊ��������˵��
����m_szTransmitStockLabelΪ��Ʊ���룬m_dwTransmitCounts��ʾ������˵�����ڲ�ͬ�������ݴ����н���ʾ��ͬ����
m_dw TransmitStockCountΪ��ʾ��Ʊ����
m_dwTransmitStockTypeCounts���ڱ�ʾ��鴫��ʱ��������˵����
*/

/* ��Ϣ������� DEMO
LONG OnStkDataOK(UINT wParam,LONG lParam)
{
RCV_DATA	*		pHeader;
int			i;

	pHeader = (RCV_DATA *)lParam;

//	���ڴ����ٶ�������������,��ý� pHeader->m_pData ָ������ݱ���,��������

	switch( wParam )
	{
	case RCV_REPORT:						// �����������÷�ʽ,��Ʊ����
		for(i=0; i<pHeader->m_nPacketNum; i++)
		{
			pHeader->m_pReport[i] ...
			// ���ݴ���
		}
		break;

	case RCV_FILEDATA:						// �����������÷�ʽ,�ļ�
		switch(pHeader->m_wDataType)
		{
		case FILE_HISTORY_EX:				// ����������
			break;

		case FILE_MINUTE_EX:				// ������������
			break;

		case FILE_BASE_EX:					// Ǯ�����ݻ��������ļ�,m_szFileName�������ļ���
			break;

		case FILE_NEWS_EX:					// ������,��������m_szFileName����Ŀ¼������
			break;
		}
		break;

	default:
		return 0;							// unknown data
	}
	return 1;
}
*/

//////////////////////////////////////////////////////////////////////////////////
//APIs
#ifdef __cplusplus
extern "C"{
#endif

//////////////////////////////////////////////////////////////////////////////////
// ע�ắ��

// ��Ʊ��ʼ��
// ��ڲ���:
//		hWnd			������Ϣ�Ĵ��ھ��
//		Msg				�û��Զ�����Ϣ
//		nWorkMode		�ӿڹ�����ʽ,Ŀǰֻ��Ϊ RCV_WORK_MEMSHARE
// ���ز���:
//		 1				�ɹ�
//		-1				ʧ��
// ע:
//		ע���,��������������ڷ�����Ϣ; ����ע��,ͨ����ѯ��ʽ���ȡ������
//		����Ʊ����û����,��������Ʊ���ճ���
int WINAPI	Stock_Init(HWND hWnd,UINT Msg,int nWorkMode);

// �˳�,ֹͣ������Ϣ
// ��ڲ���:
//		hWnd			������Ϣ�Ĵ��ھ��,ͬ Stock_Init �ĵ�����ڲ���
//	���ز���:
//		 1				�ɹ�	
//		-1				ʧ��
int WINAPI 	Stock_Quit(HWND hWnd);

//////////////////////////////////////////////////////////////////////////////////
// ����ӿ�       
      
// ȡ�ѽ��յ��Ĺ�Ʊ����
int WINAPI 	GetTotalNumber();													

// �����ȡ��Ʊ����(��չ)
// ��ڲ���:	
//			nNo			���
//			pBuf		��Ź�Ʊ���ݵĻ�����
// ���ز���:
//		    NoStockData	�޹�Ʊ����	
// ע:
//			�ú����ṩ��Ʊ���ݵ���Ҫ����;�������������ʱ,���Կ��ٽ������ݿ��
int WINAPI 	GetStockByNoEx(int nNo,RCV_REPORT_STRUCTEx * pBuf);		

// �ɹɺ�ȡ��Ʊ����(��չ)
// ��ڲ���:	
//			pszStockCode��Ʊ����	
//			nMarket		֤ȯ�г�
//			pBuf		��Ź�Ʊ���ݵĻ�����
// ���ز���:
//		    NoStockData	�޹�Ʊ����	
// ע:
//			�ú����ṩ��Ʊ���ݵ���Ҫ����;�������������ʱ,���Կ��ٽ������ݿ��
int WINAPI 	GetStockByCodeEx(char * pszStockCode,int nMarket,RCV_REPORT_STRUCTEx * pBuf);

// ������ճ���,��������
// ��ڲ���:
//			bSetup		TRUE		��ʾ����,��������
// ���ز���:
//			 1			�ɹ�
//			-1			ʧ��
int	WINAPI  SetupReceiver(BOOL bSetup);

//	ȡ�ù�Ʊ������Ϣ
//	��ڲ���:
//			nInfo		����
//			pBuf		������
//	���ڲ���:
//			nInfo == RI_IDSTRING,	���������ַ�������, pBuf Ϊ�����ַ���
//									��:	"TongShi_StockDrv_1.00"
//			nInfo == RI_IDCODE,		������Ϣ�� ID ��, pBuf Ϊ�ַ�����ʽ�� ID ��
//									��:	0x78001234	  "78001234"
//			nInfo == RI_VERSION,	������Ϣ���汾��, pBuf Ϊ�ַ����汾
//									��:  1.00		  "1.00"		
//			nInfo == RI_ERRRATE,	ȡ�ŵ�����,
//			nInfo == RI_STKNUM,		ȡ���й�Ʊ�ܼ���
DWORD WINAPI  GetStockDrvInfo(int nInfo,void * pBuf);

#ifdef __cplusplus
}
#endif

#endif // __STOCKDRV_100_H__



#ifndef __RAINSTOCKDRV_H__
#define __RAINSTOCKDRV_H__

/*******************************��ʱ����չͨ�ӿ��ӿڲ���*******************

    ��չԭ����ȫ����ͨ�ӿ��Ĺ��ܣ���֤ԭ�й����������������ڸ�����������
    ��չ��Ŀ������ȡ��ʱ���ݣ�
              ����ȡ�ֱ�����
              ȡ��Ʊ����
              ȡF10
    Ϊʵ����չ�ӿڵĹ��ܣ��ڴ���ʱע�����¼��㣬 
    1>��չ���ֵ����ݵ�λ֪ͨͨ����ϢDA_SERVICE_MSG_EXT����ͨ��SetExtMsg�Զ��壩֪ͨ���
    2>����ͨ����Ϣ֪ͨ������ݽṹΪSTRUCT_REXT_HEAD��n�����ݽṹ��Ԫ
    -----------------------------
    |    STRUCT_REXT_HEAD       |
    -----------------------------
    |                           |
    |       ��չ���ݽṹ        |
    |                           |
    -----------------------------
***************************************************************************/

#define DA_SERVICE_MSG_EXT      WM_APP + 255    //��չ�ӿ�
#define	DA_SERVICE_MSG			WM_APP + 256  

#define RCV_EXT_TRADEDATA   0X5A11      //������3�ķֱ�����  STCP_QBD_STRUCT
#define RCV_EXT_KDATA       0X5A21      //������������  RITK_STRUCT 
#define RCV_EXT_TRADEDATAV2 0X5A15      //������4�ķֱ�����, STCP_QBD6_STRUCT
#define RCV_EXT_STOCKTABLE  0x5A22      //��Ʊ�����    RCV_STOCK_CELLEx


typedef struct _REXT_HEAD_
{
    TCHAR       m_Code[STKLABEL_LEN];
    TCHAR       m_Name[STKLABEL_LEN];
    WORD        m_wMarket;  // �г�����
    WORD        m_Service;  //  ��������
                            /*
                            #define RCV_EXT_TRADEDATA   0X5A11      //������3�ķֱ�����  STCP_QBD_STRUCT
                            #define RCV_EXT_KDATA       0X5A21      //������������  RITK_STRUCT 
                            #define RCV_EXT_TRADEDATAV2 0X5A15      //������4�ķֱ�����, STCP_QBD6_STRUCT
                            #define RCV_EXT_STOCKTABLE  0x5A22      //��Ʊ�����    RCV_STOCK_CELLEx
                            */
    int         m_num;  //���ױ���
    time_t      m_mt;   //����ʱ��
    DWORD       m_unused[4];
}STRUCT_REXT_HEAD;

typedef struct _STCP_QBD_
{
    time_t          fTime ;             //ʱ��(��ȷ����)fTime       
    float           fPrice ;            //��ǰ�ɽ���         fPrice     
    float           fVolumn ;           //�ܳɽ���           fVolumn    
    float           fAmount;            //�ܳɽ���           fAmount    
    float           fBuyP[3];           //�����             fBuyP[3]   
    float           fBuyV[3];           //������              
    float           fSellP[3];          //������             
    float           fSellV[3];          //������
} STCP_QBD_STRUCT ;       // 

typedef struct _STCP_QBD6_
{
    time_t          fTime ;                   //ʱ��(��ȷ����)fTime       
    float           fPrice ;            //��ǰ�ɽ���         fPrice     
    float           fVolumn ;           //�ܳɽ���           fVolumn    
    float           fAmount;            //�ܳɽ���           fAmount    
    float           fBuyP[4];           //�����             fBuyP[3]   
    float           fBuyV[4];           //������              
    float           fSellP[4];          //������             
    float           fSellV[4];          //������
} STCP_QBD6_STRUCT ;       //


//K�����ݽṹ
typedef struct _RITK_
{
    time_t  m_time;             //UCT   
    float   m_fOpen;            //����
    float   m_fHigh;            //���
    float   m_fLow;             //���
    float   m_fClose;           //����
    float   m_fVolume;          //��
    float   m_fAmount;          //��
}RITK_STRUCT ;  



//��Ʊ�����Ԫ�ṹ��
typedef  struct tagRCV_STOCK_CELLEx
{
    WORD    m_wMarket;              // ��Ʊ�г�����
    char    m_szLabel[STKLABEL_LEN];// ��Ʊ����,��'\0'��β
    char    m_szName[STKNAME_LEN];  // ��Ʊ����,��'\0'��β
}RCV_STOCK_CELLEx ;

//////////////////////////////////////////////////////////////////////////////////
//APIs

#ifdef __cplusplus
extern "C"{
#endif

/************************������չ��Ϣ��־********************
******************************************************/
int WINAPI SetExtMsg(UINT msg);


/************************ȡ����K������********************
    scode: ��Ʊ���룬 ��'\0'��β
    mt   : ����������ʼ�����գ� ʱ�侫�ȵ��죬Сʱ���ֲ��ƣ� mt ��0��ʾȡ��������
    ���� : 1    �ɹ�
           -1   ʧ��
    ����������ͨ�ӿ��ĸ�ʽ����
******************************************************/
int WINAPI GetKData(LPCSTR scode, time_t mt);

int WINAPI QueryKData(LPCSTR scode, int nMarket, time_t mt);
/************************ȡ��ʱ����********************
    scode: ��Ʊ���룬 ��'\0'��β
    mt   : ��ʱ���������׽����գ� ʱ�侫�ȵ��죬Сʱ���ֲ���
    ���� : 1    �ɹ�
           -1   ʧ��
    ��ʱ������ͨ�ӿ��ĸ�ʽ����
******************************************************/
int  WINAPI GetMinData(LPCSTR scode, time_t mt);


int  WINAPI QueryMinData(LPCSTR scode, int nMarket, time_t mt);
/************************ȡ���׷ֱ�����********************
    scode: ��Ʊ���룬 ��'\0'��β
    mt   : ��ʱ���������׽����գ� ʱ�侫�ȵ��죬Сʱ���ֲ���
    ���� : 1    �ɹ�
           -1   ʧ��
    �ֱ����ݸ�ʽ������, ������׼���ú�ͨ����ϢDA_SERVICE_MSG_EXT֪ͨ������׼���á�

******************************************************/
int  WINAPI GetTradeData(LPCSTR scode, time_t mt);

int  WINAPI QueryTradeData(LPCSTR scode, int nMarket, time_t mt);

/************************ȡ���Ź�������********************
    channel: ����Ƶ���� ��'\0'��β
    fmt: ������Ŀǰ��Ч
    ���� : 1    �ɹ�
           -1   ʧ��
        
**********************************************************/  
int  WINAPI GetNews(LPCSTR  channel = NULL,  time_t fmt =0 );

/************************ȡ���ɻ���������********************
    scode: ��Ʊ���룬 ��'\0'��β
    fmt  : ����
    ���� : 1    �ɹ�
           -1   ʧ��
        
**********************************************************/  
int  WINAPI GetBaseInfo(LPCSTR  scode,  time_t fmt);

int  WINAPI QueryBaseInfo(LPCSTR scode, int nMarket, time_t mt);

/***********************ȡ���й�Ʊ����***********************
     ͨ����Ϣ���ش����
     STRUCT_REXT_HEAD   *pHead  pHead->m_Service == RCV_EXT_STOCKTABLE ;
     RCV_STOCK_CELLEx   * pBuf ;
     
*************************************************************/
int  WINAPI GetStockTable();

/***********************ȡ������ʱ��***********************
     ���ڿͻ������������ʱ��ͬ��
     ���أ�TRUE �ɹ�
           FALSE ʧ��
*************************************************************/
BOOL  WINAPI GetServerTime(time_t *ptime);



#ifdef __cplusplus
}
#endif

#endif // __RAINSTOCKDRV_H__



/* ͼ�Ŀ�������Ϣ�������ʾ��
LONG OnStkDataOK(UINT wParam,LONG lParam)
{
RCV_DATA    *       pHeader;
int         i;

    pHeader = (RCV_DATA *)lParam;

//  ���ڴ����ٶ�������������,��ý� pHeader->m_pData ָ������ݱ���,��������

    switch( wParam )
    {
    case RCV_REPORT:                        // �����������÷�ʽ,��Ʊ����
        for(i=0; i<pHeader->m_nPacketNum; i++)
        {
            pHeader->m_pReport[i] ...
            // ���ݴ���
        }
        break;

    case RCV_FILEDATA:                      // �����������÷�ʽ,�ļ�
        switch(pHeader->m_wDataType)
        {
        case FILE_HISTORY_EX:               // ����������
            break;

        case FILE_MINUTE_EX:                // ������������
            break;

        case FILE_BASE_EX:                  // Ǯ�����ݻ��������ļ�,m_szFileName�������ļ���
            break;

        case FILE_NEWS_EX:                  // ������,��������m_szFileName����Ŀ¼������
            break;
        }
        break;

    default:
        return 0;                           // unknown data
    }
    return 1;
}
*/

/******************************************��ʱ����չ���ݴ���ʾ��******************************************
void OnExtService(UINT wFileType, LONG lPara)
{
    STRUCT_REXT_HEAD *pHead = (STRUCT_REXT_HEAD*)lPara ;
    ASSERT(wFileType == pHead->m_Service);
    CString msg ;
    if ( wFileType == RCV_EXT_TRADEDATA)
    {
        msg.Format("%.6s %.8s %d �ֱ�" , pHead->m_Code, pHead->m_Name, pHead->m_num);
        STCP_QBD_STRUCT *pk = (STCP_QBD_STRUCT *) (lPara + sizeof(STRUCT_REXT_HEAD));

    }else if ( wFileType == RCV_EXT_TRADEDATAV2){
        msg.Format("��%.6s %.8s %d �ֱ�" , pHead->m_Code, pHead->m_Name, pHead->m_num);
        STCP_QBD6_STRUCT *pk = (STCP_QBD6_STRUCT *) (lPara + sizeof(STRUCT_REXT_HEAD));
    }else if ( wFileType == RCV_EXT_KDATA)
    {
        msg.Format("%.6s %.8s %d K��" , pHead->m_Code, pHead->m_Name, pHead->m_num);
        RITK_STRUCT *pk = (RITK_STRUCT *) (lPara + sizeof(STRUCT_REXT_HEAD));
    }else if (wFileType == RCV_EXT_STOCKTABLE)
    {
        msg.Format("��Ʊ�����" );
        RCV_STOCK_CELLEx *pk = (RCV_STOCK_CELLEx *) (lPara + sizeof(STRUCT_REXT_HEAD));
    }
}
*/