#include "StdAfx.h"
#include "CoordXBaseWidget.h"
#include "DataEngine.h"
#include "KeyWizard.h"
#include <math.h>
extern lua_State* g_Lua;
extern QString g_native;
extern luaL_Reg* g_pFuncs;



CCoordXBaseWidget::CCoordXBaseWidget(CBaseWidget* parent /*= 0*/, RWidgetType type /*= CBaseWidget::Basic*/)
	: CBaseWidget(parent,type)
	, m_typeCircle(Min1)
	, m_pMenuCustom(0)
{
	{
		m_pL = luaL_newstate();
		luaL_openlibs(m_pL);
		luaL_dostring(m_pL,g_native.toLocal8Bit());
		if(lua_type(m_pL,-1)==LUA_TSTRING)
		{
			const char* aa = lua_tostring(m_pL,-1);
			QString ss = QString::fromLocal8Bit(aa);
			qDebug()<<"Load native.lua error:"<<ss;
		}
		lua_pushglobaltable(m_pL);
		luaL_setfuncs(m_pL, g_pFuncs,0);
		lua_pop(m_pL,1);
	}
	{
		//��ʼ����ʾ���ڵĿ��ٲ��ұ�
		m_listCircle.push_back(RWidgetOpData(AutoCircle,"0","�Զ���ʱͼ"));
		m_listCircle.push_back(RWidgetOpData(FenShi,"1","��ʱͼ"));
		m_listCircle.push_back(RWidgetOpData(Sec3,"s3","3���ʱͼ"));
		m_listCircle.push_back(RWidgetOpData(Sec6,"s6","6���ʱͼ"));
		m_listCircle.push_back(RWidgetOpData(Sec12,"s12","12���ʱͼ"));
		m_listCircle.push_back(RWidgetOpData(Min1,"m1","һ��ʱͼ"));
		m_listCircle.push_back(RWidgetOpData(Min5,"m52","���ʱͼ"));
		m_listCircle.push_back(RWidgetOpData(Min15,"m153","�̷�ʱͼ"));
		m_listCircle.push_back(RWidgetOpData(Min30,"m304","��Сʱ��ʱͼ"));
		m_listCircle.push_back(RWidgetOpData(Min60,"h15","ʱ��ʱͼ"));

		m_listCircle.push_back(RWidgetOpData(Day,"d6","����ͼ"));
		m_listCircle.push_back(RWidgetOpData(Week,"w7","����ͼ"));
		m_listCircle.push_back(RWidgetOpData(Month,"y8","����ͼ"));
		m_listCircle.push_back(RWidgetOpData(Month3,"q3","����ͼ"));
		m_listCircle.push_back(RWidgetOpData(Year,"y9","����ͼ"));
	}

	m_pMenuCustom = new QMenu(tr("ɫ��ͼ�˵�"));
	{
		//���õ�ǰK��ͼ����ʾ����
		m_pMenuCircle = m_pMenuCustom->addMenu(tr("��������"));
		foreach(const RWidgetOpData& _d,m_listCircle)
		{
			m_pMenuCircle->addAction(_d.desc,this,SLOT(onSetCircle()))->setData(_d.value);
		}
	}
	//m_pL = lua_newthread(g_Lua);
}


CCoordXBaseWidget::~CCoordXBaseWidget(void)
{
}

bool CCoordXBaseWidget::loadPanelInfo( const QDomElement& eleWidget )
{
	if(!CBaseWidget::loadPanelInfo(eleWidget))
		return false;

	//��ǰ��ʾ������
	if(eleWidget.hasAttribute("circle"))
	{
		m_typeCircle = static_cast<RStockCircle>(eleWidget.attribute("circle").toInt());
	}

	return true;
}

bool CCoordXBaseWidget::savePanelInfo( QDomDocument& doc,QDomElement& eleWidget )
{
	if(!CBaseWidget::savePanelInfo(doc,eleWidget))
		return false;

	//��ʾ������
	eleWidget.setAttribute("circle",m_typeCircle);

	return true;
}

void CCoordXBaseWidget::updateData()
{
	updateTimesH();
}

void CCoordXBaseWidget::clearTmpData()
{

}

void CCoordXBaseWidget::updateTimesH()
{
	//���µ�ǰ�ĺ��������ݣ��Ӻ���ǰ����ʱ��
	if(m_typeCircle<Day)
		m_mapTimes = CDataEngine::getTodayTimeMap(m_typeCircle);
	else
		m_mapTimes = CDataEngine::getHistoryTimeMap(m_typeCircle,100);
}

void CCoordXBaseWidget::updateShowTimes( const QRectF& rtCoordX,float fItemWidth )
{
	if(m_typeCircle == FenShi)
		return;

	m_mapShowTimes.clear();
	//����������ƺ�����
	float fBeginX = rtCoordX.right();
	float fEndX = rtCoordX.left();
	float fCBWidth = fBeginX-fEndX;
	if(fCBWidth<0)
		return;

	QList<time_t> listTimes = m_mapTimes.keys();
	float fCurX = fBeginX-fItemWidth;
	int iCount = listTimes.size()-1;

	while(fCurX>fEndX && iCount>=0)
	{
		m_mapShowTimes[listTimes[iCount]] = fCurX;

		--iCount;
		fCurX = fCurX- fItemWidth;
	}
	return;
}

void CCoordXBaseWidget::drawCoordX(QPainter& p,const QRectF& rtCoordX,float fItemWidth)	//����X������
{
	//����������ƺ�����
	float fBeginX = rtCoordX.right();
	float fEndX = rtCoordX.left();
	float fCBWidth = fBeginX-fEndX;
	if(fCBWidth<0)
		return;

	QList<time_t> listTimes = m_mapTimes.keys();
	float fCurX = fBeginX-fItemWidth;
	float fLastX = fBeginX;
	int iCount = listTimes.size()-1;
	if(iCount<0)
		return;

	int iTimeCount = 0;				//ֻ����������ʱ�����ɫ��������ɫ�����ڲ鿴��
	if(m_typeCircle<Day)
	{
		time_t tmCurDate = QDateTime(QDateTime::fromTime_t(listTimes[iCount]).date()).toTime_t();
		while(fCurX>fEndX && iCount>=0)
		{
			time_t tmTime = listTimes[iCount];
			if(tmTime<tmCurDate)
			{
				p.setPen(QColor(255,255,255));
				p.fillRect(fLastX-14,rtCoordX.top(),30,rtCoordX.height(),QColor(0,0,0));
				p.drawLine(fCurX+fItemWidth,rtCoordX.top(),fCurX+fItemWidth,rtCoordX.top()+2);
				p.drawText(fCurX+fItemWidth-14,rtCoordX.top()+2,30,rtCoordX.height()-2,
					Qt::AlignCenter,QDateTime::fromTime_t(tmCurDate).toString("MM/dd"));

				tmCurDate = QDateTime(QDateTime::fromTime_t(tmTime).date()).toTime_t();
				fLastX = fCurX;
				++iTimeCount;
			}
			else
			{
				if(m_typeCircle<Min1)
				{
					if((fLastX-fCurX)>30)
					{
						if(iTimeCount%2)
						{
							p.setPen(QColor(255,0,0));
							p.drawLine(fCurX,rtCoordX.top(),fCurX,rtCoordX.top()+2);
							p.drawText(fCurX-14,rtCoordX.top()+2,30,rtCoordX.height()-2,
								Qt::AlignCenter,QDateTime::fromTime_t(tmTime).toString("hh:mm"));
						}
						else
						{
							p.setPen(QColor(0,255,255));
							p.drawLine(fCurX,rtCoordX.top(),fCurX,rtCoordX.top()+2);
							p.drawText(fCurX-24,rtCoordX.top()+2,50,rtCoordX.height()-2,
								Qt::AlignCenter,QDateTime::fromTime_t(tmTime).toString("mm:ss"));
						}

						fLastX = fCurX;
						++iTimeCount;
					}
				}
				else
				{
					if((fLastX-fCurX)>30)
					{
						p.setPen( iTimeCount%2 ? QColor(255,0,0) : QColor(0,255,255));
						p.drawLine(fCurX,rtCoordX.top(),fCurX,rtCoordX.top()+2);
						p.drawText(fCurX-14,rtCoordX.top()+2,30,rtCoordX.height()-2,
							Qt::AlignCenter,QDateTime::fromTime_t(tmTime).toString("hh:mm"));

						fLastX = fCurX;
						++iTimeCount;
					}
				}
			}

			--iCount;
			fCurX = fCurX- fItemWidth;
		}
	}
	else
	{
		while(fCurX>fEndX && iCount>=0)
		{
			if((fLastX-fCurX)>80)
			{
				p.setPen( iTimeCount%2 ? QColor(255,0,0) : QColor(0,255,255));
				p.drawLine(fCurX,rtCoordX.top(),fCurX,rtCoordX.top()+2);
				switch(m_typeCircle)
				{
				case Week:
					{
						QDateTime dtmTmp = QDateTime::fromTime_t(listTimes[iCount]);
						p.drawText(fCurX-40,rtCoordX.top()+2,80,rtCoordX.height()-2,
							Qt::AlignCenter,QString("%1 %2").arg(dtmTmp.date().year()).arg(dtmTmp.date().weekNumber()));
					}
					break;
				case Month:
					{
						QDateTime dtmTmp = QDateTime::fromTime_t(listTimes[iCount]);
						p.drawText(fCurX-40,rtCoordX.top()+2,80,rtCoordX.height()-2,
							Qt::AlignCenter,QDateTime::fromTime_t(listTimes[iCount]).toString("yyyy/MM"));
					}
					break;
				case Month3:
					{
						QDateTime dtmTmp = QDateTime::fromTime_t(listTimes[iCount]);
						p.drawText(fCurX-40,rtCoordX.top()+2,80,rtCoordX.height()-2,
							Qt::AlignCenter,QDateTime::fromTime_t(listTimes[iCount]).toString("yyyy/MM"));
					}
					break;
				case Year:
					{
						QDateTime dtmTmp = QDateTime::fromTime_t(listTimes[iCount]);
						p.drawText(fCurX-40,rtCoordX.top()+2,80,rtCoordX.height()-2,
							Qt::AlignCenter,QDateTime::fromTime_t(listTimes[iCount]).toString("yyyy"));
					}
					break;
				default:
					p.drawText(fCurX-40,rtCoordX.top()+2,80,rtCoordX.height()-2,
						Qt::AlignCenter,QDateTime::fromTime_t(listTimes[iCount]).toString("yyyy/MM/dd"));
					break;
				}
				fLastX = fCurX;
				++iTimeCount;
			}

			--iCount;
			fCurX = fCurX- fItemWidth;
		}
	}
	return;
}

//����Y������
void CCoordXBaseWidget::drawCoordY( QPainter& p,const QRectF rtCoordY, float fMax, float fMin )
{
	//��߾�ȷ��С�������λ������������1000�����м���

	if(!rtCoordY.isValid())
		return;
	if(fMax<=fMin)
		return;

	//���û�����ɫ
	p.setPen(QColor(255,0,0));

	//Y���꣨�۸�
	p.drawLine(rtCoordY.topLeft(),rtCoordY.bottomLeft());			//����

	qint64 iValueMax = fMax*1000;
	qint64 iValueMin = fMin*1000;
	qint64 iAllValue = iValueMax-iValueMin;
	float fAllUi = rtCoordY.height();
	float fRealPerUi = fAllUi/(fMax-fMin);

	qint64 iValueIncre = 1;
	while(iAllValue/iValueIncre>100)
	{
		iValueIncre *= 10;
	}
	if(iValueIncre>10000000000)
		return;

	qint64 iValue = qint64((iValueMin + iValueIncre)/iValueIncre)*iValueIncre;
	float fBottom = rtCoordY.bottom();
	float fX = rtCoordY.left();

	float fLastY = -100;
	int iDrawIndex = 0;
	while(iValue<iValueMax)
	{
		float fValue = iValue/(1000.0);
		float fY = (fBottom - (fValue - fMin)*(fRealPerUi));
		if(abs(fY-fLastY) > 15)
		{
			if(iDrawIndex%2)
				p.setPen(QColor(255,0,0));
			else
				p.setPen(QColor(0,255,255));

			p.drawLine(fX,fY,fX+4,fY);
			int iDot = 3 - int(log10(double(iValueIncre))+0.1);
			if(iDot<0)
				iDot = 0;
			if(fValue>10000.0)
			{
				p.drawText(fX+7,fY+4,QString("%1").arg(fValue,0,'g',iDot));
			}
			else
			{
				p.drawText(fX+7,fY+4,QString("%1").arg(fValue,0,'f',iDot));
			}
			p.setPen(QColor(255,0,0));

			fLastY = fY;
			++iDrawIndex;
		}
		iValue = iValue+iValueIncre;
	}
}

void CCoordXBaseWidget::onSetCircle()
{
	//���õ�ǰ����ʾ����
	QAction* pAct = reinterpret_cast<QAction*>(sender());
	setCircle(static_cast<RStockCircle>(pAct->data().toInt()));
}

void CCoordXBaseWidget::setCircle( RStockCircle _c )
{
	clearTmpData();
	m_typeCircle = _c;
	updateData();
}

void CCoordXBaseWidget::getKeyWizData( const QString& keyword,QList<KeyWizData*>& listRet )
{
	foreach(const RWidgetOpData& _d,m_listCircle)
	{
		if(_d.key.indexOf(keyword)>-1)
		{
			KeyWizData* pData = new KeyWizData;
			pData->cmd = CKeyWizard::CmdCircle;
			pData->arg = (void*)_d.value;
			pData->desc = _d.desc;
			listRet.push_back(pData);
			if(listRet.size()>20)
				return;
		}
	}

	return CBaseWidget::getKeyWizData(keyword,listRet);
}

void CCoordXBaseWidget::keyWizEntered( KeyWizData* pData )
{
	if(pData->cmd == CKeyWizard::CmdCircle)
	{
		setCircle(static_cast<RStockCircle>((int)(pData->arg)));
		return;
	}
	return CBaseWidget::keyWizEntered(pData);
}
