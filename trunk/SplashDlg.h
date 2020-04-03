/************************************************************************/
/* �ļ����ƣ�SplashDlg.h
/* ����ʱ�䣺2012-11-08 10:59
/*
/* ��    �������ڳ����ʼ���ͳ����˳�ʱ���������ڣ��̳���QDialog
/************************************************************************/

#ifndef SPLASH_DLG_H
#define SPLASH_DLG_H
#include <QtGui>

class CSplashDlg : public QWidget
{
	Q_OBJECT
public:
	static CSplashDlg* getSplashDlg();

private:
	CSplashDlg(QWidget* parent = 0);
	~CSplashDlg();

public slots:
	void showMessage(const QString& msg, int iPro = 90);

protected:
	virtual void showEvent(QShowEvent* event);

private:
	static CSplashDlg* m_pSelf;
	QProgressBar* m_pProgressBar;
	QLabel* m_pLabel;
};

#endif	//SPLASH_DLG_H