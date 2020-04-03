/************************************************************************/
/* �ļ����ƣ�WatcherSettingDlg.h
/* ����ʱ�䣺2013-06-08 14:03
/*
/* ��    �������������״�������Ĵ���
/************************************************************************/
#ifndef	WATCHER_SETTING_DLG_H
#define	WATCHER_SETTING_DLG_H
#include <QtGui>

class CWatcherSettingDlg : public QDialog
{
	Q_OBJECT
public:
	CWatcherSettingDlg(QWidget* parent = NULL);
	CWatcherSettingDlg(const QList<int>& listUsed,QWidget* parent=NULL);
	~CWatcherSettingDlg(void);

public:
	QList<int> getSelIDs();

protected slots:
	void onRefresh();
	void onAddWatcher();
	void onDelWatcher();
	void onEditWatcher();

private:
	void initDlg();

private:
	QTreeWidget m_listWatchers;
	QPushButton m_btnAdd;
	QPushButton m_btnDel;
	QPushButton m_btnEdit;
	QPushButton m_btnRefresh;

	bool m_bForSel;
	QList<int> m_listSel;
	bool m_bInit;
};

#endif	//WATCHER_SETTING_DLG_H