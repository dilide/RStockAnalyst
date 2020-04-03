#include "StdAfx.h"
#include <QtNetwork>
#include "BaseInfoGetter.h"
#include "DataEngine.h"

float getFloatByRegExp(const QString& qsContent,const QString& qsExp, int iIndex = 0)
{
	QRegExp rx(qsExp);
	qsContent.indexOf(rx);
	QStringList listCaps = rx.capturedTexts();
	if(listCaps.size()>iIndex)
	{
		bool bOk = false;
		float fRet = listCaps[1].toFloat(&bOk);
		if(bOk)
		{
//			qDebug()<<fRet;
			return fRet;
		}
	}
	return FLOAT_NAN;
}


QString CBaseInfoGetter::getWebContentByUrl( const QString& qsUrl )
{
	QNetworkAccessManager manager;
	QNetworkRequest req(qsUrl);
	QEventLoop loop;
	QNetworkReply* pReply = manager.get(req);
	QObject::connect(pReply,SIGNAL(finished()),&loop,SLOT(quit()));
	loop.exec();
	QString qsRet = pReply->readAll();


	pReply->deleteLater();
	return qsRet;
}


void CBaseInfoGetter::updateBaseInfoFromEastMoney( qRcvBaseInfoData* pBaseInfo )
{
	QString qsUrl = QString("http://quote.eastmoney.com/%1%2.html").arg(CDataEngine::getMarketStr(pBaseInfo->wMarket)).arg(pBaseInfo->code);
	QString qsContent = getWebContentByUrl(qsUrl);
	if(qsContent.contains("�����ʵ�ҳ�治����"))
	{
		return;
	}

	{
		//ȥ��������ʱ�䡱ǰ������
//		qsContent.remove(QRegExp(".*����ʱ��"));
	}
	//
	{
		//����ʱ�� \d{4}��\d{2}��\d{2}��
		QRegExp rx("\\d{4}��\\d{2}��\\d{2}��");
		qsContent.indexOf(rx);
		QStringList listCaps = rx.capturedTexts();
		if(listCaps.size()>0)
		{
			QDate dtTime = QDate::fromString(listCaps[0],"yyyy��MM��dd��");
		}
	}
	{
		//�ܹɱ�  (?:prozgb.{2,15})(\d+\.\d+)
		float _v = getFloatByRegExp(qsContent,"(?:prozgb.{2,15})(\\d+\\.\\d+)",1);
		if(!_isnan(_v))
		{
			pBaseInfo->fZgb = _v*10000;
		}
	}
	{
		//��ͨ�� (?:proltgb.{2,15})(\d+\.\d+)
		float _v = getFloatByRegExp(qsContent,"(?:proltgb.{2,15})(\\d+\\.\\d+)",1);
		if(!_isnan(_v))
		{
			pBaseInfo->fLtAg = _v*10000;
		}
	}
	{
		//ÿ������ (?:ÿ������.{2,100})(\d+\.\d+)
		float _v = getFloatByRegExp(qsContent,"(?:ÿ������.{2,100})(\\d+\\.\\d+)",1);
		if(!_isnan(_v))
		{
			pBaseInfo->fMgsy = _v;
		}
	}
	{
		//ÿ�ɾ��ʲ� (?:ÿ�ɾ��ʲ�.{2,100})(\d+\.\d+)
		float _v = getFloatByRegExp(qsContent,"(?:ÿ�ɾ��ʲ�.{2,100})(\\d+\\.\\d+)",1);
		if(!_isnan(_v))
		{
			pBaseInfo->fMgjzc = _v;
		}
	}
	{
		//���ʲ������� (?:���ʲ�������.{2,100})(\d+\.\d+)
		float _v = getFloatByRegExp(qsContent,"(?:���ʲ�������.{2,100})(\\d+\\.\\d+)",1);
		if(!_isnan(_v))
		{
			pBaseInfo->fJzcsyl = _v;
		}
	}
	{
		//Ӫҵ���� (?:Ӫҵ����.{2,100})(\d+\.\d+)
		float _v = getFloatByRegExp(qsContent,"(?:Ӫҵ����.{2,100})(\\d+\\.\\d+)",1);
		if(!_isnan(_v))
		{
			pBaseInfo->fZyywsr = _v*10000;
		}
	}
	{
		//Ӫҵ���������� (?:Ӫҵ����������.{2,100})(\d+\.\d+)
		float _v = getFloatByRegExp(qsContent,"(?:Ӫҵ����������.{2,100})(\\d+\\.\\d+)",1);
		if(!_isnan(_v))
		{
//			pBaseInfo->fZyywsr = _v*10000;
		}
	}
	{
		//����ë���� (?:����ë����.{2,100})(\d+\.\d+)
		float _v = getFloatByRegExp(qsContent,"(?:����ë����.{2,100})(\\d+\\.\\d+)",1);
		if(!_isnan(_v))
		{
//			pBaseInfo->fZyywsr = _v*10000;
		}
	}
	{
		//������ (?:������.{2,100})(\d+\.\d+)
		float _v = getFloatByRegExp(qsContent,"(?:������.{2,100})(\\d+\\.\\d+)",1);
		if(!_isnan(_v))
		{
			pBaseInfo->fJlr = _v;
		}
	}
	{
		//������������ (?:������������.{2,100})(\d+\.\d+)
		float _v = getFloatByRegExp(qsContent,"(?:������������.{2,100})(\\d+\\.\\d+)",1);
		if(!_isnan(_v))
		{
//			pBaseInfo->fJlr = _v;
		}
	}
	{
		//ÿ��δ�������� (?:ÿ��δ��������.{2,100})(\d+\.\d+)
		float _v = getFloatByRegExp(qsContent,"(?:ÿ��δ��������.{2,100})(\\d+\\.\\d+)",1);
		if(!_isnan(_v))
		{
//			pBaseInfo->fJlr = _v;
		}
	}
}
