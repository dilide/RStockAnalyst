/*
============================================================================
�ļ�����	:	PanelResource.cpp
����		:	���ǿ�
����ʱ��	:	2011-12-29 16:56
�޸�ʱ��	:	2011-12-29 16:57
˵��		:	������ʾ��ǰ���е�Panel��Դ�б��Լ�Panel֮��ĸ��ӹ�ϵ��
============================================================================
*/

#include "common.h"
#include "PanelResource.h"
#include "PanelViewer.h"

using namespace eGingkoPanelManager;

CPanelResource::CPanelResource( QWidget* parent /*= 0*/ )
	: QTreeWidget(parent)
{
	setHeaderLabels(QStringList()<<tr("Name"));
	//����itemѡ�е��ź�
	connect(this,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
		this,SLOT(onCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
}

CPanelResource::~CPanelResource(void)
{
}

void CPanelResource::onCurrentItemChanged( QTreeWidgetItem* current,QTreeWidgetItem* previous )
{
	if(!current)
		return;
	QWidget* pWidget = (QWidget*)current->data(0,Qt::UserRole).toUInt();
	if(pWidget->inherits("eGingkoPanelManager::CPanelViewer"))
	{
		//����Ӧ�Ĵ�������Ϊ��ý����״̬��
		(static_cast<CPanelViewer*>(pWidget))->setPanelFocus();
	}
}
