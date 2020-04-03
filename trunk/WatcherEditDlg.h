/************************************************************************/
/* �ļ����ƣ�WatcherEditDlg.h
/* ����ʱ�䣺2013-06-08 14:03
/*
/* ��    �������ڱ༭�����״���Ӷ���
/************************************************************************/
#ifndef	WATCHER_EDIT_DLG_H
#define	WATCHER_EDIT_DLG_H
#include <QtGui>
#include "RadarWatcher.h"

class CWatcherEditDlg : public QDialog
{
	Q_OBJECT
public:
	CWatcherEditDlg(CRadarWatcher* pWatcher = NULL,QWidget* parent = NULL);
	~CWatcherEditDlg(void);


protected slots:
	void onBtnOk();
	void onBtnCancel();

private:
	//������ʾ�Ŀؼ�
	QLineEdit m_editSec;
	QLineEdit m_editHold;
	QComboBox m_combType;
	QComboBox m_combBlock;
	QComboBox m_combDestBlock;
	//
	QPushButton m_btnOk;
	QPushButton m_btnCancel;

private:
	int m_iSec;									//���Ӽ��
	float m_fHold;								//��ֵ
	RadarType m_type;							//�״�����
	CBlockInfoItem* m_pWatcherBlock;			//���ӵİ��
	CRadarWatcher* m_pWatcher;					//������
	CBlockInfoItem* m_pDestBlock;				//Ŀ����
};

#endif	//WATCHER_EDIT_DLG_H