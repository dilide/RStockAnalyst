/*
============================================================================
�ļ�����	:	ListPanel.h
����		:	���ǿ�
����ʱ��	:	2011-12-29 16:56
�޸�ʱ��	:	2011-12-29 16:57
˵��		:	���ڴ����֧�ֵ����д��ڣ�Ŀǰֻ��Panelһ��
============================================================================
*/

#ifndef _LIST_PANEL_H
#define _LIST_PANEL_H
#include <QListWidget>
#include <QtEvents>

namespace eGingkoPanelManager
{
	class CListPanel : public QListWidget
	{
		Q_OBJECT
	public:
		CListPanel(QWidget* parent = 0);
		~CListPanel(void);

	public:
		void initListPanel();

	protected:
		void startDrag(Qt::DropActions supportedActions);
	};
}

#endif	//_LIST_PANEL_H