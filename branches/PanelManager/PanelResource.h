/*
============================================================================
�ļ�����	:	PanelResource.h
����		:	���ǿ�
����ʱ��	:	2011-12-29 16:56
�޸�ʱ��	:	2011-12-29 16:57
˵��		:	������ʾ��ǰ���е�Panel��Դ�б��Լ�Panel֮��ĸ��ӹ�ϵ��
============================================================================
*/

#ifndef _PANEL_RESOURCE_H
#define _PANEL_RESOURCE_H

#include <QtGui>

namespace eGingkoPanelManager
{
	class CPanelResource : public QTreeWidget
	{
		Q_OBJECT
	public:
		CPanelResource(QWidget* parent = 0);
		~CPanelResource(void);

	protected slots:
		void onCurrentItemChanged(QTreeWidgetItem* current,QTreeWidgetItem* previous);
	};
}

#endif	//_PANEL_RESOURCE_H