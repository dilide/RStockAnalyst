/************************************************************************/
/* �ļ����ƣ�ColorBlockWidget.h
/* ����ʱ�䣺2012-11-19 10:59
/*
/* ��    ����������ʾ��ֻ��Ʊ�����ƣ��̳���CBaseWidget
/*           �����Ե�ʹ�ò�ͬ��ɫ������ʾ��Ʊ���������ƣ��Ӷ�����ͬ�������У�
/*           չ�ָ������Ϣ��
/************************************************************************/

#ifndef COLOR_BLOCK_WIDGET_H
#define COLOR_BLOCK_WIDGET_H
#include "BaseBlockWidget.h"
#include "BlockInfoItem.h"
#include "ColorManager.h"

class CColorBlockWidget : public CBaseBlockWidget
{
	Q_OBJECT
public:
	//��ʾ���ͣ���
	enum RShowType
	{
		ShowIncrease = 1,	//�Ƿ�
		ShowTurnRatio,		//������
		ShowVolumeRatio,	//����
	};

	enum RAsistIndex
	{
		IndexNewPriceCount = 1,	//���¸ߴ���
		IndexNewVolumeCount,	//�����¸ߴ���
		IndexIncrease,			//�����Ƿ�
		IndexVolumeRatio,		//����
		IndexTurnRatio,			//������
		IndexGuanDan,			//��ǰ�ҵ����
		Index5DayLine,			//���5���Ƿ��仯
		IndexCmpPrice,			//���5�����ڵļ۸�仯
	};



public:
	CColorBlockWidget(CBaseWidget* parent = 0);
	~CColorBlockWidget(void);

public:
	//���ظ�K��ͼ��������Ϣ
	virtual bool loadPanelInfo(const QDomElement& eleWidget);
	//�����K��ͼ��������Ϣ
	virtual bool savePanelInfo(QDomDocument& doc,QDomElement& eleWidget);


	//����ǰ���ڴ�
	virtual void clearTmpData();

	//���µ�ǰ������ʽ
	virtual void updateSortMode(bool bSelFirst);

protected:
	//���µ�ǰ�ĺ���������
	virtual void updateTimesH();

public:
	//ͨ������keyword��ȡ��Ҫ�ڰ�����������ʾ������
	virtual void getKeyWizData(const QString& keyword,QList<KeyWizData*>& listRet);
	//���̾��鴰��ȷ�Ϻ󴥷�
	virtual void keyWizEntered(KeyWizData* pData);

public slots:
	virtual void setBlock(const QString& block);

protected slots:
	void onSetCurrentBlock();								//���õ�ǰ��ʾ�İ��
	void onSetShowType();									//������ʾ����
	void onSetExpression();									//���õ�ǰ��ʾ�ı��ʽ
	void onSetTopStock1();									//�����ö���ʾ��Ʊ
	void onSetTopStock2();
	void onSetTopStock3();
	void onRemoveTopStock();								//�Ƴ��ö���ʾ��Ʊ
	void onRemoveStock();									//�ӵ�ǰ�����ɾ��ѡ�й�Ʊ
	void onFocusWhenMove();									//��������ƶ�ʱ�л���Ʊ
	void onSetAsistIndex();									//���õ�ǰ��ʾ�ĸ���ָ��



	virtual void updateColorBlockData();					//���µ�ǰ��Ҫ��ʾ������

protected:
	void setShowType(RShowType _t);
	int getTopCount();							//��ȡ�ö�Ԫ�ص�����

private:
	void clickedStock(CStockInfoItem* pItem);	//�������Ʊʱ����

protected:
	virtual void paintEvent(QPaintEvent* e);				//�����¼�
	virtual void mouseMoveEvent(QMouseEvent* e);			//����ƶ��¼�
	virtual void mousePressEvent(QMouseEvent* e);			//������¼�
	virtual void wheelEvent(QWheelEvent* e);				//����м������¼�
	virtual void keyPressEvent(QKeyEvent* e);				//���̲���

	//����ɫ��
	//vColor:ȡ0-21
	//vHeight:�ٷֱ�0-100%;
	//vWidth:�ٷֱ�0%-100%;
	void drawColocBlock(QPainter& p,int iY,
		QVector<float>& vColor,QVector<float>& vHeight,QVector<float>& vWidth
		,CColorItem* pClrItem);

	//�麯���������ؼ����Զ���˵���
	virtual QMenu* getCustomMenu();

	virtual void onBlockClicked(CBlockInfoItem* pBlock,int iCmd);
	/*��ػ�ͼ����*/
private:
	void drawHeader(QPainter& p,const QRect& rtHeader);			//����ͷ����Ϣ
	void drawClient(QPainter& p,const QRect& rtClient);			//����������
	void drawBottom(QPainter& p,const QRect& rtBottom);			//���Ƶײ�����

	void drawStock(QPainter& p,const QRect& rtCB,CStockInfoItem* pItem,CColorItem* pClrItem);	//���Ƶ�ֻ��Ʊ

	QRect rectOfStock(CStockInfoItem* pItem);					//��ȡĳֻ��Ʊ��ʾ��λ��
	CStockInfoItem* hitTestStock(const QPoint& ptPoint);		//����ĳ����ָ��Ĺ�Ʊ��Ϣ
	RStockData* hitTestCBItem(const QPoint& ptPoint);//����ĳ����ָ���ɫ����Ϣ

private:
	QMenu* m_pMenuShowType;					//���ƽ����е���ʾ����
	QAction* m_pActFocusWhenMove;			//����ƶ�ʱ�л���Ʊ�˵�
	QAction* m_pActRemoveStock;				//ɾ���ð���еĹ�Ʊ

	QList<RWidgetOpData> m_listShowOp;		//��ʾ�����б�
	QList<RWidgetOpData*> m_listAsistIndex;	//��ʾ����ָ��

	QMap<RAsistIndex,RAsistIndex> m_mapCurAsistIndex;	//��ǰ��ʾ�ĸ���ָ��

	CBlockInfoItem* m_pBlock;				//��ǰ�İ������
	QList<CStockInfoItem*> m_listStocks;	//��ǰ��ʾ�����й�Ʊ�б�

	QMap<CStockInfoItem*,int> m_mapStockIndex;	//�������ٲ���ĳֻ��Ʊ���ڵ�����
	CStockInfoItem* m_pSelectedStock;			//��ǰѡ�еĹ�Ʊ

	QMap<int ,QList<CStockInfoItem*>> m_mapTopStocks;	//�ö���Ʊ

	/*���ڻ��Ʋ����ĳ�Ա����*/
private:
	int m_iTitleHeight;						//ͷ���߶�
	int m_iBottomHeight;					//�ײ��ĸ߶�
	int m_iLeftLen;							//���Ŀ�������
	int m_iRightLen;						//�Ҳ�Ŀ�������
	int showStockIndex;						//��ǰ��ʾ����ʼλ�ã��У�

	QRect m_rtHeader;						//ͷ��Header����
	QRect m_rtBottom;						//�ײ�������������������Ϣ

	QTimer m_timerUpdateUI;					//������µ�timer

	//��������ʽ���ڵľ���
	QMap<int,QRect> m_mapSorts;

	bool m_bShowIncrease;					//��ʾ�Ƿ�����ɫ��
	bool m_bShowTurnRatio;					//��ʾ�����ʣ��߶ȣ�
	bool m_bShowVolumeRatio;				//��ʾ���ȣ���ȣ�

	bool m_bFocusWhenMove;					//������ƶ�ʱ���ù�Ʊ�л�
};

#endif	//COLOR_BLOCK_WIDGET_H