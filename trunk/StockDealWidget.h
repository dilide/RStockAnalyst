#ifndef STOCK_DEAL_WIDGET_H
#define STOCK_DEAL_WIDGET_H
#include "CoordXBaseWidget.h"

enum RDealWidgetType
{
	DealKind = 1,		//����
	DealIncrese,		//����
	DealOrder,			//�ҵ�
	DealOrder2,			//�ҵ���2����ʾ��ʽ
};

class CStockDealWidget : public CCoordXBaseWidget
{
	Q_OBJECT
public:
	CStockDealWidget(CBaseWidget* parent = 0);
	~CStockDealWidget(void);


protected:
	//���ظ�K��ͼ��������Ϣ
	virtual bool loadPanelInfo(const QDomElement& eleWidget);
	//�����K��ͼ��������Ϣ
	virtual bool savePanelInfo(QDomDocument& doc,QDomElement& eleWidget);
	//ͨ������keyword��ȡ��Ҫ�ڰ�����������ʾ������
	virtual void getKeyWizData(const QString& keyword,QList<KeyWizData*>& listRet);
	//���̾��鴰��ȷ�Ϻ󴥷�
	virtual void keyWizEntered(KeyWizData* pData);


protected slots:
	void onSetDealType();
	void onSetColorMode();									//���������ɫģʽ

protected:
	//���õ�ǰ��ʾ�Ĺ�Ʊ
	virtual void setStockCode(const QString& code);

	//������ʾ����
	void setDealType(RDealWidgetType _t);

	//������ɫģʽ
	void setColorMode(const QString& mode);

	//�������ݣ���ʱʹ��
	virtual void updateData();

	//
	virtual void mousePressEvent(QMouseEvent* e);

	//��ͼ
	virtual void paintEvent(QPaintEvent* e);


	//�麯���������ؼ����Զ���˵���
	virtual QMenu* getCustomMenu();

protected:
	void drawTitle(QPainter& p);
	void drawClient(QPainter& p);
	void drawBottom(QPainter& p);

private:
	int m_iTitleHeight;		//����߶�
	int m_iBottomHeight;	//�ײ��߶�

	int m_iItemWidth;		//����ɫ��Ŀ��
	int m_iItemHeight;	//����ɫ��ĸ߶�

	QRect m_rtTitle;		//��������
	QRect m_rtClient;		//��ͼ����
	QRect m_rtBottom;		//�ײ�����

	RDealWidgetType m_typeWidget; //��ǰ���ڵ���ʾ����

	//�����������ڵľ���
	QMap<int,QRect> m_mapDealTypes;

	QString m_qsColorMode;					//��ǰ��ɫģʽ
private:
	QMenu* m_pMenuDealType;					//�ɽ�����ͼ
	QMenu* m_pMenuColorMode;				//��ɫģʽѡ��˵�

private:
	CStockInfoItem* m_pCurStock;			//��ǰ�Ĺ�Ʊ
	QMap<time_t,int> m_mapTimes;			//��ǰ��Ҫ��ʾ������ʱ�䣨�������꣩


	QList<RWidgetOpData> m_listDealType;	//��ʾ���ͱ�
};


#endif	//STOCK_DEAL_WIDGET_H