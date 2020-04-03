#ifndef ABNORMAL_SETTING_DLG_H
#define ABNORMAL_SETTING_DLG_H
#include <QtGui>

enum RAbnomalType
{
	HighIncrease = 1,		//���Ƿ�
	LowIncrease,			//���Ƿ�������
	HighTurnRatio,			//�߻�����
	HighVolumeRatio,		//������
	HighAvgVolume,			//ÿ�ʾ��� ��
	HighPrice,				//�۸����ĳֵ
	LowPrice,				//�۸�С��ĳֵ
	HighLTSZ,				//����ͨ��
	LowLTSZ,				//����ͨ��
	HighMGSY,				//��ÿ������
	HighPERatio,			//����ӯ��
	HighZYYWRatio,			//����Ӫҵ�������� ��
	HighYJYZ,				//��ҵ��Ԥ��
	HighYJZZ,				//��ҵ������
	HighYJHBZZ,				//��ҵ��������������4������
	HighJZCSYL,				//���ʲ�������   >=
	HighBuyVolume,			//ί��1+2+3>=
	HighSellVolume,			//ί��1+2+3>=
	HighCommRatio,			//ί�� >=
};

enum RAbnomalEqual
{
	AbnomalLesser = -1,		//С�ڵ���
	AbnomalEqual = 0,		//����
	AbnomalGreater = 1,		//���ڵ���
};

struct RAbnomalValue
{
	RAbnomalType _T;		//����
	RAbnomalEqual _E;		//�Ա�
	float _Default;			//Ĭ��ֵ
	QString _Desc;			//����
	RAbnomalValue(RAbnomalType _t,RAbnomalEqual _e,QString _d,float _default)
	{
		_T = _t;
		_E = _e;
		_Desc = _d;
		_Default = _default;
	}
};

class CAbnomalSettingDlg : public QDialog
{
	Q_OBJECT
public:
	static void initAbnomals();
	static void ShowAbnomalDlg(QMap<RAbnomalType,float>& map,QWidget* parent = 0);

protected:
	CAbnomalSettingDlg(const QMap<RAbnomalType,float>& map,QWidget* parent = 0);
	~CAbnomalSettingDlg(void);

protected:
	QMap<RAbnomalType,float> getAbnomalMap();
	void initDlg();

protected slots:
	void onBtnOk();
	void onBtnCancel();

private:
	QMap<RAbnomalType,QPair<QLineEdit*,QCheckBox*>> m_mapCtrls;		//���е�LineEdit
	QMap<RAbnomalType,float> m_mapAbnomal;	//�쳣����������

private:
	static QMap<RAbnomalType,RAbnomalValue*> m_mapAbnomals;		//
	static bool m_bInit;
};

#endif	//ABNORMAL_SETTING_DLG_H