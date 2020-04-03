/************************************************************************/
/* �ļ����ƣ�BlockCodeManager.h
/* ����ʱ�䣺2013-05-21 10:43
/*
/* ��    �������ڹ�����Ĺ�Ʊ����
/************************************************************************/
#ifndef BLOCK_CODE_MANAGER_H
#define BLOCK_CODE_MANAGER_H
#include <QtCore>


class CBlockCodeManager
{

public:
	static QString getBlockCode(const QString& qsCodePath);

protected:
	CBlockCodeManager(void);
	~CBlockCodeManager(void);

private:
	static QSettings* m_pRegApp;
};


#endif	//BLOCK_CODE_MANAGER_H