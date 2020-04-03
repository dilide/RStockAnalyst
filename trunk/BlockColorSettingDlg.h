/************************************************************************/
/* �ļ����ƣ�BlockColorWidget.h
/* ����ʱ�䣺2012-12-13 10:03
/*
/* ��    ����ɫ����ɫ�༭����
/*           ���ڱ༭ɫ�鴰�ڵ���ʾ��ɫ
/************************************************************************/
#ifndef BLOCK_COLOR_SETTING_DLG_H
#define BLOCK_COLOR_SETTING_DLG_H
#include <QtGui>

class CBlockColorSettingDlg : public QDialog
{
	Q_OBJECT
public:
	CBlockColorSettingDlg(QWidget* parent = 0);
	~CBlockColorSettingDlg(void);

	static CBlockColorSettingDlg* getDialog();

public:
	void reloadColorMode();
	bool saveColorMode();

protected slots:
	void onAddColorMode();
	void onRemoveColorMode();
	void onColorItemChanged();

	void onAddItemColor();
	void onRemoveItemColor();
	void onClickedItemColor(QListWidgetItem* pItem);

	void onBtnOk();					//ȷ��
	void onBtnCancel();				//ȡ��

protected:
	void updateCurMode();			//���µ�ǰ����ɫ��

private:
	static CBlockColorSettingDlg* m_pDialog;
	QListWidget* m_pListColors;
	QListWidget* m_pListDetail;

	QMap<QString,QVector<QColor>> m_mapBlockColors;	//ɫ����ɫ���ࡣ��ͨ���ٷֱ�������ȡ��ɫ����������
};


#endif	//BLOCK_COLOR_WIDGET_H