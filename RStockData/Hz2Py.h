#ifndef _Hz_2_Py_H_
#define _Hz_2_Py_H_
#include <QtCore>

class CHz2Py
{
public:
	/*��ʼ������->ƴ�����ձ�*/
	static int initHz2PyTable(const QString& qsFile);
	/*��ȡ���ֵ�����ĸ��Сд��*/
	static QList<QList<QChar>> getHzFirstLetter(const QString& qsWord);

private:
	static QMap<QChar,QList<QString>> m_mapHanZi2PY;				//����ƴ�����ձ�
};

#endif	//_Hz_2_Py_H_