/************************************************************************/
/* �ļ����ƣ�ColorManager.h
/* ����ʱ�䣺2012-11-30 14:38
/*
/* ��    ������ɫ������
/*           ���ڸ��ֳ��õ���ɫ����
/************************************************************************/

#ifndef COLOR_MANAGER_H
#define COLOR_MANAGER_H
#include <QtCore>
#include "rsd_global.h"

inline uint rRGB(int r, int g, int b)// set RGB value
{ return (0xffu << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff); }

class RSDSHARED_EXPORT CColorItem : public QObject
{
public:
	CColorItem(const QVector<uint>& vClrs);
	~CColorItem();

public:
	uint getColor(const float& _f,const float& _abs=0.1);
	uint getColor(const int& _v);

private:
	QVector<uint> m_vColors;
};


class RSDSHARED_EXPORT CColorManager
{
public:
	static void initAll();
	static void reloadBlockColors();		//���¼���ɫ����ɫ��
private:
	static void initCommonColor();
	static void initBlockColors();

public:
	/*��ȡ��ǰ��֧�ֵ�ɫ����ɫ�б�*/
	static QStringList getBlockColorList();
	/*��ȡָ������ɫ�б�*/
	static CColorItem* getColorItem(const QString& mode);

public:
	/*��ȡ���õ���ɫ��ͨ������ֵ*/
	static uint getCommonColor(int index);

private:
	static CColorItem* m_pDefaultColor;	//Ĭ��ɫ���
	static QMap<QString,CColorItem*> m_mapColors;	//ɫ����ɫ���ࡣ��ͨ���ٷֱ�������ȡ��ɫ����������
	static QVector<uint> CommonColor;		//���õ���ɫ��
};


#endif	//COLOR_MANAGER_H