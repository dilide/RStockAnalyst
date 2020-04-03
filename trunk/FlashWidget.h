/************************************************************************/
/* �ļ����ƣ�FlashWidget.h
/* ����ʱ�䣺2013-07-14 11:05
/*
/* ��    ����������ʾ��ֻ��Ʊ�����ƣ��̳���CBaseWidget
/*           �����Ե�ʹ�ò�ͬ��ɫ������ʾ��Ʊ���������ƣ��Ӷ�����ͬ�������У�
/*           չ�ָ������Ϣ��
/************************************************************************/
#ifndef FLASH_WIDGET_H
#define FLASH_WIDGET_H
#include "BaseWidget.h"

class CFlashWidget : public CBaseWidget
{
	Q_OBJECT
public:
	CFlashWidget(CBaseWidget* parent = 0);
	~CFlashWidget(void);

public:
	//���ظ�K��ͼ��������Ϣ
	virtual bool loadPanelInfo(const QDomElement& eleWidget);
	//�����K��ͼ��������Ϣ
	virtual bool savePanelInfo(QDomDocument& doc,QDomElement& eleWidget);

protected slots:
	void onAddWidget();
	void onSwitchNextWidget();

protected:
	//�麯���������ؼ����Զ���˵���
	virtual QMenu* getCustomMenu();
	virtual void paintEvent(QPaintEvent* e);

	void restNewWidget(QWidget* pWidget);

private:
	QMenu* m_pMenuCustom;					//�Զ���˵�
	QMenu* m_pMenuAddWidget;				//���Ӵ���

private:
	QList<CBaseWidget*> m_listWidgetsStack;	//������ʾ�Ĵ���
	int m_iSwitchSeconds;					//�л�ʱ��
	QTimer m_timerSwitch;					//�л�ʱ����
	CBaseWidget* m_pCurWidget;				//��ǰ��ʾ��Widget

	int m_iTitleHeight;						//��ǰͷ���ĸ߶�
};


#endif	//FLASH_WIDGET_H