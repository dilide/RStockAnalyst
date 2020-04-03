#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtGui>
#include "BaseWidget.h"
#include "BlockInfoItem.h"

class CMainWindow : public QMainWindow
{
	Q_OBJECT
public:
	CMainWindow();
	~CMainWindow();
	static CMainWindow* getMainWindow();


public:
	bool setupStockDrv();
	void initTemplates();			//��ʼ������ģ��
	void saveTemplates();			//��������ģ��
	void initBlockMenus();			//��ʼ�����Ĳ˵���ȫ�֣�

	QMenu* getBlockMenu(CBaseWidget* pWidget,const int iCmd)
	{
		m_pBlockMenuWidget = pWidget;
		m_iBlockMenuCmd = iCmd;
		return &m_menuBlocks;
	}

public:
	void clickedStock(const QString& code);
	void clickedBlock(const QString& block);

protected slots:
	void onAddTemplate();			//��Ӱ���
	void onSaveTemplate();			//�������а���
	void onRemoveTemplate();		//ɾ������
	void onShowFuncHelper();		//��ʾ������������


	void onMarketCloseSetting();	//�������̺���������ʱ��
	void onMarketClose();			//���̺�������������
	void onBlockActClicked();		//���˵������󴥷�
	void onMarketCloseTimer();		//15�������ʱ�����˺��������ں����н��м�飩

	void onShowAllStocks();			//��ʾ���еĹ�Ʊ��Ϣ

	//
	void onWatcherSetting();		//����״�����

	//���¼��������ݵĶ�ʱ����Ӧ����
	void onCalcBlock();

protected:
	virtual bool winEvent( MSG* message, long* result );


protected:
	virtual long OnStockDrvMsg(WPARAM wParam,LPARAM lParam);

private:
	CBaseWidget* getSubWindows(const QString& title);
	void CreateSubBlockMenu(QMenu* pMenuParent,CBlockInfoItem* pBlockParent);

private:
	static CMainWindow* m_pMainWindow;
	QTabWidget* m_pTabWidget;
	QMenuBar* m_pMenuBar;
	QString m_qsTemplateDir;		//��������ļ����ڵ�Ŀ¼

	CBaseWidget* m_pBlockMenuWidget;					//��ǰBlockMenu��Ӧ�Ĵ���ָ��
	int m_iBlockMenuCmd;								//��ǰBlockMenu��Ӧ�Ĳ���
	QMenu m_menuBlocks;									//���˵�
	QMap<CBlockInfoItem*,QAction*> mapBlockMenus;		//���˵�map

	QTimer m_timerClose;								//�Ƿ�����
	QTime m_tmExportClose;								//�����������ݵ�ʱ��

	QTimer m_timerCalcBlock;							//���¼���������

	bool m_bExportClose;								//�Ƿ��Ѿ�������������
};


#endif	//MAIN_WINDOW_H