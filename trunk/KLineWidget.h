/************************************************************************/
/* �ļ����ƣ�KLineWidget.h
/* ����ʱ�䣺2012-11-08 10:59
/*
/* ��    ����������ʾK��ͼ�Ĵ��ڣ��̳���CBaseWidget
/************************************************************************/

#ifndef K_LINE_WIDGET_H
#define K_LINE_WIDGET_H
#include <QtGui>
#include "CoordXBaseWidget.h"
#include "StockInfoItem.h"


class CKLineWidget : public CCoordXBaseWidget
{
	Q_OBJECT
public:
	enum FenShiCoordY			//��ʱͼY��Ļ��Ʒ�ʽ
	{
		Percent10 = 0,				//10���ٷֱ�
		PercentAuto = 1,			//����Ӧ�ٷֱ�
	};
	enum FenShiAsis				//��ʱͼ�ϵĸ���ͼ
	{
		FenShiVol = 0,			//����������ָ�꣩
		FenShiVolRatio,			//����
	};
public:
	CKLineWidget(CBaseWidget* parent = 0);
	~CKLineWidget(void);

public:
	//���ظ�K��ͼ��������Ϣ
	virtual bool loadPanelInfo(const QDomElement& eleWidget);
	//�����K��ͼ��������Ϣ
	virtual bool savePanelInfo(QDomDocument& doc,QDomElement& eleWidget);

	//��������
	virtual void updateData();
	//����ǰ���ڴ�
	virtual void clearTmpData();

public:
	//ͨ������keyword��ȡ��Ҫ�ڰ�����������ʾ������
	virtual void getKeyWizData(const QString& keyword,QList<KeyWizData*>& listRet);
	//���̾��鴰��ȷ�Ϻ󴥷�
	virtual void keyWizEntered(KeyWizData* pData);

public slots:
	virtual void setStockCode(const QString& only);
	virtual void setBlock(const QString& block);
	void updateMinLine(const QString& only);
	void updateDayLine(const QString& only);

protected:
	void setStockItem(CAbstractStockItem* pItem);

	//���µ�ǰ�ĺ���������
	virtual void updateTimesH();

	//��ȡ���ݶ�ά��ͨ��������ǰ�����ڡ�
	QMap<time_t,RStockData*>* getColorBlockMap(CAbstractStockItem* pItem);

protected:
	virtual void paintEvent(QPaintEvent* e);				//�����¼�
	virtual void mouseMoveEvent(QMouseEvent* e);			//����ƶ��¼�
	virtual void leaveEvent(QEvent* e);						//����Ƴ��¼�
	virtual void mousePressEvent(QMouseEvent* e);			//������¼�
	virtual void mouseDoubleClickEvent(QMouseEvent *e);		//���˫���¼�
	virtual void keyPressEvent(QKeyEvent* e);				//���̰����¼������ڴ����ݼ�����

	//�麯���������ؼ����Զ���˵���
	virtual QMenu* getCustomMenu();

protected slots:
	void onSetStockCode();								//�����Ի������û��ֶ������Ʊ����
	void onSetExpression();								//�����Ի������û��ֶ�������ʽ
	void onClickedAddShow();							//������ʾ����
	void onClickedSubShow();							//������ʾ����
	void onAddDeputy();									//���Ӹ�ͼ
	void onAddVolume();									//�Ƿ���ʾ����ͼ
	void onRemoveDeputy();								//ɾ����ͼ
	void onSetSizes();									//��������ͼ����ʾ����

	void onAdd2Block();									//��ӵ����
	void onAdd2NewBlock();								//��ӵ��½����

	void onSetYCoordAuto();								//�����Զ���ʾY��ķ�Χ
	void onSetYCoordPercent();							//����Y���԰ٷֱȷ�ʽ��ʾ

protected:
	void setShowCount(int _iShow);						//���õ�ǰ����ʾ����

private:
	void drawTitle(QPainter& p,const QRect& rtTitle);	//����ͷ��
	void drawShowBtns(QPainter& p,const QRect& rtBtns);	//�������½ǵ�������ť
	void drawExpArgs(QPainter& p,const QRect& rtClient,
		const QString& e,QList<uint>& lsColor);

	virtual void drawCoordX(QPainter& p,const QRectF& rtCoordX,float fItemWidth);	//����X������
	//����Y�ᣬ��Ҫ����K��ͼ
	virtual void drawCoordY(QPainter& p,const QRectF rtCoordY, float fMax, float fMin, bool bPercent = false);

	//���Ʒ�ʱͼ
	void drawFenShi(QPainter& p, QRect rtClient);
private:
	void resetTmpData();					//���¼������ݡ�

private:
	QMenu* m_pMenuAdd2Block;				//��ѡ���˵�
	QMenu* m_pMenuYCoordMode;				//Y����ʾ��ʽ

private:
	CAbstractStockItem* m_pStockItem;		//��ǰK��ͼ�Ĺ�Ʊ����ָ��
	QMap<time_t,RStockData*>* m_mapData;	//����������ʾ������
	int m_iShowCount;						//��Ҫ��ʾ�����ݸ��������ȣ�������ӦС��listItems��size��

	int m_iCurExp;							//��ǰѡ�еı��ʽ
	bool m_bShowMax;						//�Ƿ������ʾ��Ŀǰֻ�Ը�ͼ��Ч��

	int m_iTitleHeight;						//ͷ���߶�
	int m_iCoorYWidth;						//Y������Ŀ��
	int m_iCoorXHeight;						//X������ĸ߶�
	float m_fItemWidth;						//����Item�Ŀ��
	int m_iMainLinerHeight;					//��ͼ�ĸ߶�

	QVector<int> m_vSizes;					//��ʾ����(�ܺ�Ϊ100)
	QVector<QString> m_vExps;				//��ʾ�ı��ʽ

	QRect m_rtAddShow;						//������ʾ�����İ�ť����
	QRect m_rtSubShow;						//������ʾ�����İ�ť����

	QRect m_rtTitle;						//����Title������
	QRect m_rtClient;						//����K��ͼ������

	//�����������ڵľ���
	QMap<int,QRect> m_mapCircles;

	bool m_bLock;							//�����л�ʱ�ļ���
	QDateTime m_tmLastUpdate;				//���һ�θ��µ�ʱ��

	bool m_bYCoordPercent;					//Y����������������ռ۰ٷֱ���ʾ
	bool m_bYCoordAuto;						//�Ƿ�Ϊ�Զ���ʾY�᷶Χ

	//��ʱͼ������
	int m_iFenShiCount;						//���Ʒ�ʱͼ������
	FenShiCoordY m_iFenShiCoordY;			//��ʱͼY��Ļ��Ʒ�ʽ����
	FenShiAsis m_iFenShiAsis;				//��ʱͼ�ĸ�����
};


#endif	//K_LINE_WIDGET_H