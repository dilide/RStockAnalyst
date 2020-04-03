/*
============================================================================
�ļ�����	:	MainWindow.h
����		:	���ǿ�
����ʱ��	:	2011-12-29 16:56
�޸�ʱ��	:	2011-12-29 16:57
˵��		:	����������ڡ�
============================================================================
*/

#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H
#include <QtGui>
#include <QtXml>
#include "ListPanel.h"
#include "PanelViewer.h"
#include "PanelResource.h"
#include "PanelProperty.h"

namespace eGingkoPanelManager
{
	class CMainWindow : public QMainWindow
	{
		Q_OBJECT
	public:
		CMainWindow(QWidget* parent = NULL);
		~CMainWindow(void);
		static CMainWindow* getMainWindow();

	public:
		void setPropertyWidget(QWidget* widget);
		CPanelViewer* getCentralPanel(){ return m_pMainWidget; }

	protected:
		virtual void showEvent(QShowEvent* event);

	protected slots:
		void OnLoadPanelFile();							//��Ӧ����Panel����
		void OnSavePanelFile();							//��Ӧ����Panel����
		void OnClearPanels()							//��Ӧ���Panel����
		{
			m_pMainWidget->clearChildren();
		}

	private:
		QWidget* m_pViewWidget;				//

		CPanelViewer* m_pMainWidget;		//���벼�ִ���
		CListPanel* m_pListPanels;		//Panel������
		CPanelResource* m_pPanelResource;		//������Դ��ϵͼ
		QMenuBar* m_pMenuBar;

		QDockWidget* m_pDockSettings;
		QDockWidget* m_pDockResource;
		QDockWidget* m_pDockListViewers;	//
		QDockWidget* m_pDockProperty;		//���Թ���Dock

		QLineEdit* m_pEditName;
		QComboBox* m_pComboClinic;

		static CMainWindow* m_pMainWindow;
	};
}
#endif	//_MAIN_WINDOW_H