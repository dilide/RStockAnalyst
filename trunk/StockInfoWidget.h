/************************************************************************/
/* �ļ����ƣ�StockInfoWidget.h
/* ����ʱ�䣺2012-12-13 17:55
/*
/* ��    ����������ʾ��ֻ��Ʊ��������Ϣ���̳���CBaseWidget
/************************************************************************/
#ifndef STOCK_INFO_WIDGET_H
#define STOCK_INFO_WIDGET_H
#include "BaseWidget.h"
#include "StockInfoItem.h"

class CStockInfoWidget : public CBaseWidget
{
	Q_OBJECT
public:
	CStockInfoWidget(CBaseWidget* parent = 0);
	~CStockInfoWidget(void);

public:
	//���ظ�K��ͼ��������Ϣ
	virtual bool loadPanelInfo(const QDomElement& eleWidget);
	//�����K��ͼ��������Ϣ
	virtual bool savePanelInfo(QDomDocument& doc,QDomElement& eleWidget);

public:
	//ͨ������keyword��ȡ��Ҫ�ڰ�����������ʾ������
	virtual void getKeyWizData(const QString& keyword,QList<KeyWizData*>& listRet);
	//���̾��鴰��ȷ�Ϻ󴥷�
	virtual void keyWizEntered(KeyWizData* pData);

public slots:
	virtual void setStockCode(const QString& only);
	void updateStockInfo(const QString& only);

protected:
	void setStockItem(CStockInfoItem* pItem);

protected:
	virtual void paintEvent(QPaintEvent* e);				//�����¼�
	virtual void keyPressEvent(QKeyEvent* e);				//���̲���

	//�麯���������ؼ����Զ���˵���
	virtual QMenu* getCustomMenu();

protected slots:
	void onSetStockCode();								//�����Ի������û��ֶ������Ʊ����

private:
	QMenu* m_pMenuCustom;					//�Զ���˵�

	CStockInfoItem* m_pStockItem;

private:
	/*���ƴ������õ��ĳ�Ա����*/
	int m_iItemHeight;						//���еĸ߶�
	QColor m_clrGrid;						//�ָ���������ɫ
	QColor m_clrTitle;						//������ɫ
	QColor m_clrDesc;						//������Ϣ������ɫ
	QColor m_clrDataRed;					//��������������ɫ
	QColor m_clrDataBlue;					//�����½�������ɫ
	QColor m_clrDataNone;					//��������״̬
};


#endif	//STOCK_INFO_WIDGET_H