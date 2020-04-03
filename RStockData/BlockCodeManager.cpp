/************************************************************************/
/* �ļ����ƣ�BlockCodeManager.cpp
/* ����ʱ�䣺2013-05-21 10:43
/*
/* ��    �������ڹ�����Ĺ�Ʊ����
/************************************************************************/
#include "StdAfx.h"
#include "BlockCodeManager.h"


QSettings* CBlockCodeManager::m_pRegApp = 0;

QString CBlockCodeManager::getBlockCode( const QString& qsCodePath )
{
	if(m_pRegApp == 0)
	{
		m_pRegApp = new QSettings("HKEY_CURRENT_USER\\Software\\RStockAnalyst\\", QSettings::NativeFormat);
	}

	if(m_pRegApp->contains(qsCodePath))
	{
		return m_pRegApp->value(qsCodePath).toString();
	}
	else
	{
		QString qsCurrentCode = m_pRegApp->value("\\","800000").toString();
		qsCurrentCode = QString("%1").arg(qsCurrentCode.toInt()+1);
		m_pRegApp->setValue(qsCodePath,qsCurrentCode);
		m_pRegApp->setValue("\\",qsCurrentCode);
		
		return qsCurrentCode;
	}
}

CBlockCodeManager::CBlockCodeManager(void)
{
}


CBlockCodeManager::~CBlockCodeManager(void)
{
}
