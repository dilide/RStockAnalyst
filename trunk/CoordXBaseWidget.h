#ifndef COORDX_BASE_WIDGET_H
#define	COORDX_BASE_WIDGET_H
#include "RStockFunc.h"
#include "BaseWidget.h"

class CCoordXBaseWidget : public CBaseWidget
{
	Q_OBJECT
public:
	CCoordXBaseWidget(CBaseWidget* parent = 0, RWidgetType type = WidgetBasic);
	~CCoordXBaseWidget(void);

protected:
	//���ظ�K��ͼ��������Ϣ
	virtual bool loadPanelInfo(const QDomElement& eleWidget);
	//�����K��ͼ��������Ϣ
	virtual bool savePanelInfo(QDomDocument& doc,QDomElement& eleWidget);
	//��������
	virtual void updateData();
	//����ǰ���ڴ�
	virtual void clearTmpData();

public:
	//ͨ������keyword��ȡ��Ҫ�ڰ�����������ʾ������
	virtual void getKeyWizData(const QString& keyword,QList<KeyWizData*>& listRet);
	//���̾��鴰��ȷ�Ϻ󴥷�
	virtual void keyWizEntered(KeyWizData* pData);

protected:
	virtual void updateTimesH();										//���µ�ǰ�ĺ���������
	void updateShowTimes(const QRectF& rtCoordX,float fItemWidth);		//������Ҫ���л��Ƶ�ʱ����

	virtual void drawCoordX(QPainter& p,const QRectF& rtCoordX,float fItemWidth);	//����X������
	//����Y�ᣬ��Ҫ����K��ͼ
	virtual void drawCoordY(QPainter& p,const QRectF rtCoordY, float fMax, float fMin);

protected slots:
	void onSetCircle();								//���õ�ǰ����ʾ����

protected:
	virtual void setCircle(RStockCircle _c);		//���õ�ǰ����ʾ����

protected:
	QMenu* m_pMenuCustom;					//�Զ���˵�
	QMenu* m_pMenuCircle;					//�������ò˵�
	RStockCircle m_typeCircle;				//��ͼ����ʾ����
	lua_State* m_pL;

protected:
	QMap<time_t,int> m_mapTimes;			//��ǰ��Ҫ��ʾ������ʱ�䣨�������꣩
	QMap<time_t,float> m_mapShowTimes;		//��ǰ�Ѿ����Ƶ�ʱ��

	QList<RWidgetOpData> m_listCircle;	//��ʾ���ڱ�
};

#endif	//COORDX_BASE_WIDGET_H