/************************************************************************/
/* �ļ����ƣ�BaseWidget.h
/* ����ʱ�䣺2012-11-16 10:59
/*
/* ��    �������л��ƴ��ڵĻ��֧࣬�ֲ�ֵȲ���
/************************************************************************/
#ifndef BASE_WIDGET_H
#define BASE_WIDGET_H
#include <QtGui>
#include <QtXml>
#include "RStockFunc.h"
#include "BlockInfoItem.h"

typedef struct tagKeyWizData
{
	int cmd;			//��ǰ������
	void* arg;			//����
	QString desc;		//������Ϣ��������ʾ�ڼ��̾����б��
} KeyWizData;


//���ڲ����Ĺ�������
typedef struct tagRWidgetOpData
{
	int value;					//ֵ
	QString key;				//���ٲ��Ҽ�
	QString desc;				//������Ϣ

	tagRWidgetOpData(const int& _v, const QString& _k, const QString& _d)
		: value(_v)
		, key(_k)
		, desc(_d)
	{
	}
} RWidgetOpData;

class CBaseWidget : public QWidget
{
	Q_OBJECT
public:
	static CBaseWidget* createBaseWidget(CBaseWidget* parent=0, RWidgetType type=WidgetBasic);

public:
	CBaseWidget(CBaseWidget* parent = 0, RWidgetType type = WidgetBasic);
	~CBaseWidget(void);

	void initMenu();				//��ʼ���˵���
	void realignSplitter();			//���¶�Splitter���в���
public:
	/*Widget�Ļ�������*/
	//����
	void setWidgetName(const QString& qsTitle){m_qsName = qsTitle;}
	QString getWidgetName(){return m_qsName;}


	//��ȡ�ô����е�Splitterָ��
	QSplitter* getSplitter(){return m_pSplitter;}
	//��ȡ������
	CBaseWidget* getParent(){return m_pParent;}
	//�������ø�����
	void resetParent(CBaseWidget* parent);
	//��ȡ���е��Ӵ���
	QList<CBaseWidget*> getChildren();
	//������е��Ӵ���
	void clearChildren();

	//��ȡ���ؼ��ڸ������еĴ�С���ٷֱȣ�
	int getSize();

	//��ȡwidget���ڵ�����
	int getWidgetIndex(CBaseWidget* widget) const;
	//���µ�widget �滻֮ǰ����index�Ĵ���
	void replaceWidget(int index, CBaseWidget* widget);

	bool isRelate(){ return m_pActRelate->isChecked(); }

	//�麯���������ؼ����Զ���˵���
	virtual QMenu* getCustomMenu(){ return 0; }
	//���ظ�Widget��������Ϣ
	virtual bool loadPanelInfo(const QDomElement& eleWidget);
	//�����Widget��������Ϣ
	virtual bool savePanelInfo(QDomDocument& doc,QDomElement& eleWidget);

public:
	//ͨ������keyword��ȡ��Ҫ�ڼ��̾�������ʾ������
	virtual void getKeyWizData(const QString& keyword,QList<KeyWizData*>& listRet);
	//���̾��鴰��ȷ�Ϻ󴥷�
	virtual void keyWizEntered(KeyWizData* pData);

public slots:
	/*
		�麯�����������������ش˺����Խ�����Ӧ����
	*/
	virtual void setStockCode(const QString& code);
	/*
		�麯�����������������ش˺����Խ�����Ӧ����
	*/
	virtual void setBlock(const QString& block);

	/*
		�麯�����������������ش˺����Խ�����Ӧ����
	*/
	virtual void onBlockClicked(CBlockInfoItem* pBlock,int iCmd);

protected slots:
	/*�Ҽ��˵�����*/
	void onLeftInsert();			//�����
	void onRightInsert();			//�Ҳ���
	void onTopInsert();				//�ϲ���
	void onBottomInsert();			//�²���
	void onInsertWidget();			//���봰����Ӧ

	/*�Ҽ��˵������ò�ͬ�İ���*/
	void onResetWidget();			//�������ô�������
protected:
	void insertWidget(RWidgetInsert _i);
	void setWidgetType(RWidgetType _t);

protected:
	virtual void paintEvent(QPaintEvent* e);
	virtual void contextMenuEvent(QContextMenuEvent* e);
	virtual void keyPressEvent(QKeyEvent* e);
	virtual void mousePressEvent(QMouseEvent* e);

protected:
	CBaseWidget* m_pParent;			//������ָ��
	QMenu* m_pMenu;					//�Ҽ��˵�
	QSplitter* m_pSplitter;			//�ָ���
	QString m_qsName;				//��������
	RWidgetType m_type;				//��������
	QAction* m_pActRelate;			//�Ƿ���������ڹ���

	QVBoxLayout* m_pLayout;			//��ǰ��Layout

	//�Ƿ�����ˬģʽ
	bool m_bClearMode;
protected:
	QList<RWidgetOpData> m_listWidget;	//֧�ֵĴ�������
	QList<RWidgetOpData> m_listOperate;	//֧�ֵĴ�������
};


#endif	//BASE_WIDGET_H