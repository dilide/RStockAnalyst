/************************************************************************/
/* �ļ����ƣ�AllStockWidget.h
/* ����ʱ�䣺2013-07-05 14:07
/*
/* ��    �����������Թ�Ʊ�Ŀ���Ԥ����ʾ
/************************************************************************/

#ifndef All_STOCK_WIDGET_H
#define All_STOCK_WIDGET_H
#include "BaseWidget.h"

class CAllStockWidget : public CBaseWidget
{
	Q_OBJECT

public:
	enum RSortType
	{
		SortByCode = 1,		//����Ʊ��������
		SortByZGB,			//���ܹɱ�����
		SortByIncrease,		//���Ƿ�����
		SortByTurnRatio,	//������������
		SortByVolumeRatio,	//����������
	};
public:
	CAllStockWidget(CBaseWidget* parent = 0, RWidgetType type = WidgetAllStock);
	~CAllStockWidget(void);


public:
	void setCircle(int _cr);
	void setSortType(RSortType _st);

signals:
	void stockFocus(CStockInfoItem*);

protected:
	virtual void paintEvent(QPaintEvent*);
	virtual void mouseMoveEvent(QMouseEvent* e);

	void drawStock(CStockInfoItem* pStock,const QRect& rtStock,QPainter& p);

	void clearStockDatas();
	void updateStockDatas();


	CStockInfoItem* hitTestStock(const QPoint& ptPoint) const;

private:
	int m_iItemWidth;				//�����ڵ���
	int m_iItemHeight;				//�����ڵ�߶�
	int m_iStockWidth;				//������Ʊ�Ŀ��
	int m_iStockHeight;				//������Ʊ�ĸ߶�
	RStockCircle m_crCircle;		//��ǰ����ʾ����
	RSortType m_stSort;				//��������
	Qt::SortOrder m_stOrder;		//����ʽ�����򡢽���

	QVector<uint> m_vColors;		//��ɫ��21����


	int m_iScaleX;					//
	int m_iScaleY;					//
	QPixmap m_pixScale;				//
	QDateTime m_tmLastUpdate;		//last update time;
private:
	QList<CStockInfoItem*> m_listStocks;		//���еĹ�Ʊ
	QMap<CStockInfoItem*,QList<RStockData*>*> m_mapStockDatas;
};

#endif	//All_STOCK_WIDGET_H