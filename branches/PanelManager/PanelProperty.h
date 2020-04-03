/*
============================================================================
�ļ�����	:	PanelProperty.h
����		:	���ǿ�
����ʱ��	:	2011-12-29 16:56
�޸�ʱ��	:	2011-12-29 16:57
˵��		:	������ʾPanel�����Դ���
============================================================================
*/

#ifndef _PANEL_PROPERTY_H
#define _PANEL_PROPERTY_H
#include <QtGui>

namespace eGingkoPanelManager
{
	class CPanelViewer;
	class CPanelProperty;
	class CROIProperty;

	class CPropertyDelegateItem : public QItemDelegate
	{
		Q_OBJECT
	public:
		CPropertyDelegateItem(CPanelProperty* parent);
		~CPropertyDelegateItem(){}

		static CPropertyDelegateItem* getPropertyDelegateItem(int iType,CPanelProperty* parent);

	protected:
		virtual QWidget* createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
		virtual void setEditorData ( QWidget * editor, const QModelIndex & index ) const;
		virtual void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;

	private:
		CPanelProperty* m_pPanelProperty;
		static QMap<int, CPropertyDelegateItem*> m_mapPropertyDelegate;
	};

	class CPanelProperty : public QTreeWidget
	{
		Q_OBJECT
		friend class CPropertyDelegateItem;
	public:
		CPanelProperty(QWidget* parent = 0);
		~CPanelProperty(void);

		static CPanelProperty* getPanelProperty(int iType);

	public:
		void modifyPanelName(const QString& name);	//�޸�����

	public slots:
		virtual void setPanelViewer(CPanelViewer* pPanel);		//���õ�ǰ��Panel
		void updateProperty();							//���µ�ǰPanel����ʾ����


	private:
		QTreeWidgetItem* m_pItemName;	//Panel������
		QTreeWidgetItem* m_pItemX;		//Panel��X����
		QTreeWidgetItem* m_pItemY;		//Panel��Y����
		QTreeWidgetItem* m_pItemWidth;	//Panel�Ŀ��
		QTreeWidgetItem* m_pItemHeight;	//Panel�ĸ߶�
		QTreeWidgetItem* m_pItemDock;	//Panel��Dock����
		QTreeWidgetItem* m_pItemViewerType;	//��ʾ�ؼ���� ����

		CPanelViewer* m_pCurrentViewer;

		QMap<int,QString> m_mapViewerType;

	private:
		static QMap<int, CPanelProperty*> m_mapPanelProperty;
	};
}
#endif	//_PANEL_PROPERTY_H