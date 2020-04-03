/*
============================================================================
�ļ�����	:	PanelViewer.h
����		:	���ǿ�
����ʱ��	:	2011-12-29 16:56
�޸�ʱ��	:	2011-12-29 16:57
˵��		:	����ʵ��Panel�Ļ��ƣ��Լ�Panel����Ӧ��һЩ������֧��Ƕ��
============================================================================
*/

#ifndef _PANEL_VIEWER_H
#define _PANEL_VIEWER_H
#include <QtGui>
#include <QtEvents>
#include <QDomElement>
#include <QDomDocument>


namespace eGingkoPanelManager
{
	class CPanelProperty;
	class CPanelResource;
	class CPanelViewer : public QWidget
	{
		Q_OBJECT
	public:
		enum PanelDirect
		{
			PanelTop = 1,
			PanelBottom,
			PanelLeft,
			PanelRight,
			PanelFill
		};
		static CPanelViewer* createPanelViewer(int iType,QTreeWidgetItem* pItemParent,QWidget* parent=0);
	public:
		CPanelViewer(QTreeWidgetItem* pItemParent, int iType, QWidget* parent = 0);		//���캯��
		~CPanelViewer(void);					//��������

	public:
		/*Panel�Ļ�������*/
		//����
		void setPanelName(const QString& qsTitle){m_qsName = qsTitle;m_pItem->setData(0,Qt::DisplayRole,m_qsName);}
		QString getPanelName(){return m_qsName;}

		//X����
		void setPanelX(int iX){ m_iX = iX; }
		int getPanelX(){ return m_iX; }
		//Y����
		void setPanelY(int iY){ m_iY = iY; }
		int getPanelY(){ return m_iY; }

		//��ȡX��Y�����нϴ��һ������Ϊֻ��һ���ǿ���ʹ�õģ���ʱ������
		int getPositon(){ return qMax(m_iY,m_iX); }

		//���
		void setPanelWidth(int iWidth){ m_iWidth = iWidth; }
		int getPanelWidth(){ return m_iWidth; }
		//���
		void setPanelHeight(int iHeight){ m_iHeight = iHeight; }
		int getPanelHeight(){ return m_iHeight; }

		//��ȡWidth��Height�н�С��һ������Ϊ������һ����100%�ģ�������Ҫȡ�ľ��ǽ�С���Ǹ���
		int getSize(){ return qMin(m_iHeight,m_iWidth); }

		//Dock
		void setPanelDock(PanelDirect iDock){m_iDock = iDock;}
		PanelDirect getPanelDock(){return m_iDock;}

		//ViewerType		��ʾ�ؼ�����
		GKG_VIEWERTYPE getViewerType() const { return m_iViewerType; }

		//��ȡ�ô����е�Splitterָ��
		QSplitter* getSplitter(){return m_pSplitter;}


		//��ȡ������
		QWidget* getParent(){return m_pParent;}
		//���ø�����
		void resetParent(QWidget* parent);


		//��ȡ��ǰ���ڶ�Ӧ��TreeWidgetItemָ��
		QTreeWidgetItem* getTreeItem(){return m_pItem;}


		//���ý���
		void setPanelFocus();
		//ȥ������
		void killPanelFocus(){m_bFocus = false;update();}
		//��ȡ��Panel�Ƿ��ý���
		bool isPanelFocus(){return m_bFocus;}
		//�жϸ�Panel�ĸ�Panel�Ƿ��ý���
		bool isPanelParentFocus();

		//�������ò���
		void resizeSplitter();

		//�жϸô����Ƿ�ΪpWidget���Ӵ��ڣ�������
		bool hasParent(QWidget* pWidget);

		//���ظ�Panel��������Ϣ
		virtual bool loadPanelInfo(const QDomElement& elePanel);
		//�����Panel��������Ϣ
		virtual bool savePanelInfo(QDomDocument& doc,QDomElement& elePanel);


		//��ȡ���е���Panel
		QList<CPanelViewer*> getChildren();
		//��ȡ��Panel�ĸ���
		int getChildrenCount(){ return m_pSplitter->count(); };
		//�����Panel
		bool clearChildren();

	public slots:
		/* ����Panel��
		/* @param1 true:�ӱ������µ�����; false:�ӽ�����µ�����
		*/
		void updatePanel(bool bSave = false);

	protected:
		virtual void paintEvent(QPaintEvent *event);			//����

		virtual void dragEnterEvent(QDragEnterEvent *event);		//
		virtual void dragLeaveEvent(QDragLeaveEvent *event);
		virtual void dragMoveEvent(QDragMoveEvent *event);
		virtual void dropEvent(QDropEvent *event);
		virtual void mousePressEvent(QMouseEvent * event);
		virtual void mouseMoveEvent(QMouseEvent * event);
		virtual void mouseReleaseEvent(QMouseEvent * event);

		virtual void resizeEvent(QResizeEvent* event);

		virtual void showEvent(QShowEvent* event);


		//��ȡ�������ʾ������
		QRect getHighlightedRect(const QPoint& ptCurrent);

		//��ȡĳ�����ڵ�λ��
		PanelDirect getDirect(const QPoint& ptCurrent);

	public:
		static CPanelResource* g_pPanelResource;		//Panel����Դ����

		static bool g_bDraging;							//�Ƿ��д������ڱ���ק
		static bool g_bLoading;							//���ڳ�ʼ���������أ�

		static CPanelViewer* g_pPanelMain;				//������
		static QMap<int, QImage*> g_mapPixmapPanel;		//panel��������ʾͼƬ

	protected:
		/*��������*/
		QString m_qsName;		//����
		int m_iX;				//left
		int m_iY;				//top
		int m_iWidth;			//width
		int m_iHeight;			//height
		PanelDirect m_iDock;	//Dock
		QImage* m_pImgBack;		//background image

		GKG_VIEWERTYPE m_iViewerType;	//��ʾ�ؼ����� Ĭ��Ϊ:GKG_VIEWER_ROI

		QRect m_rtHighlighted;
		QRect m_rtClose;		//�رհ�ť�Ļ�������
		int m_iTitleHeight;

		QSplitter* m_pSplitter;
		QWidget* m_pParent;
		QTreeWidgetItem* m_pItemParent;
		QTreeWidgetItem* m_pItem;

		bool m_bFocus;			//��panel�Ƿ��ý���

		QPoint m_ptMousePress;	//��갴��ʱ������

	private:
		static CPanelViewer* g_pPanelFocusing;			//���ڻ�ȡ�����Panel
	};
}
#endif	//_PANEL_VIEWER_H
