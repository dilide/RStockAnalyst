/************************************************************************/
/* ��ȡ��ֻ��Ʊ�Ļ�����Ϣ
/************************************************************************/
#ifndef	BASE_INFO_GETTER_H
#define	BASE_INFO_GETTER_H
#include "RStockFunc.h"

class CBaseInfoGetter
{
public:
	static QString getWebContentByUrl(const QString& qsUrl);
	static void updateBaseInfoFromEastMoney(qRcvBaseInfoData* pBaseInfo);		//�Ӷ����Ƹ�����������
};

#endif	//BASE_INFO_GETTER_H