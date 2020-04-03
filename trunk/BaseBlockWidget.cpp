/************************************************************************/
/* �ļ����ƣ�BaseBlockWidget.h
/* ����ʱ�䣺2013-01-14 13:38
/*
/* ��    ��������ɫ��ͼ�ĳ��󣬼̳���CCoordXBaseWidget
/*           �����Ե�ʹ�ò�ͬ��ɫ������ʾ��Ʊ���������ƣ��Ӷ�����ͬ�������У�
/*           չ�ָ������Ϣ��
/************************************************************************/

#include "StdAfx.h"
#include "BaseBlockWidget.h"
#include "ColorManager.h"
#include "DataEngine.h"
#include "KeyWizard.h"


CBaseBlockWidget::CBaseBlockWidget( CBaseWidget* parent /*= 0*/, RWidgetType type /*= CBaseWidget::Basic*/ )
	: CBaseWidget(parent,type)
	, m_pMenuColorMode(0)
	, m_iCBHeight(16)
	, m_iCBWidth(16)
	, m_qsColorMode("")
	, m_sort(SortByCode)
	, m_sortOrder(Qt::AscendingOrder)
	, m_pMenuCustom(0)
{
	{
		//��ʼ������ʽ
		m_listSortOp.push_back(RWidgetOpData(SortByCode,"vsc","��������"));
		m_listSortOp.push_back(RWidgetOpData(SortByIncrease,"vsi","�Ƿ�����"));
		m_listSortOp.push_back(RWidgetOpData(SortByTurnRatio,"vst","����������"));
		m_listSortOp.push_back(RWidgetOpData(SortByVolumeRatio,"vsv","��������"));
	}
	//��ʼ���˵�
	{
		m_pMenuCustom = new QMenu(tr("ɫ��ͼ�˵�"));

		//��ɫ��ʾģʽ�˵�
		m_pMenuColorMode = m_pMenuCustom->addMenu("������ɫģʽ");
		//����ɫ��Ĵ�С
		m_pMenuCustom->addAction(tr("����ɫ���С"),this,SLOT(onSetBlockSize()));
		//�����춯������
		m_pMenuCustom->addAction(tr("�����춯����"),this,SLOT(onSetAbnomal()));

		//��������ʽ
		m_pMenuSortMode = m_pMenuCustom->addMenu("��������ʽ");
		{
			foreach(const RWidgetOpData& _d,m_listSortOp)
			{
				m_pMenuSortMode->addAction(_d.desc,this,SLOT(onSetSortMode()))->setData(_d.value);
			}
		}
	}

	m_pMenuCustom->addSeparator();
}


CBaseBlockWidget::~CBaseBlockWidget(void)
{
	delete m_pMenuCustom;
}

bool CBaseBlockWidget::loadPanelInfo( const QDomElement& eleWidget )
{
	if(!CBaseWidget::loadPanelInfo(eleWidget))
		return false;

	//ɫ��Ŀ��
	if(eleWidget.hasAttribute("CBWidth"))
	{
		m_iCBWidth = eleWidget.attribute("CBWidth").toInt();
	}
	//ɫ��ĸ߶�
	if(eleWidget.hasAttribute("CBHeight"))
	{
		m_iCBHeight = eleWidget.attribute("CBHeight").toInt();
	}

	//��ǰ����ɫģʽ
	QDomElement eleColorMode = eleWidget.firstChildElement("color");
	if(eleColorMode.isElement())
	{
		m_qsColorMode = eleColorMode.text();
	}

	//��ǰ���춯����
	QDomElement eleAbnomals = eleWidget.firstChildElement("Abnomals");
	if(eleAbnomals.isElement())
	{
		QDomElement eleAbnomal = eleAbnomals.firstChildElement("Abnomal");
		while (eleAbnomal.isElement())
		{
			RAbnomalType _t = static_cast<RAbnomalType>(eleAbnomal.attribute("type").toInt());
			float _v = eleAbnomal.text().toFloat();
			m_mapAbnomal[_t] = _v;

			eleAbnomal = eleAbnomal.nextSiblingElement("Abnomal");
		}
	}
	
	return true;
}

bool CBaseBlockWidget::savePanelInfo( QDomDocument& doc,QDomElement& eleWidget )
{
	if(!CBaseWidget::savePanelInfo(doc,eleWidget))
		return false;

	eleWidget.setAttribute("CBWidth",m_iCBWidth);
	eleWidget.setAttribute("CBHeight",m_iCBHeight);

	//��ǰ����ɫģʽ
	QDomElement eleColorMode = doc.createElement("color");
	eleColorMode.appendChild(doc.createTextNode(m_qsColorMode));
	eleWidget.appendChild(eleColorMode);

	//��ǰ���춯����
	QDomElement eleAbnomals = doc.createElement("Abnomals");
	QMap<RAbnomalType,float>::iterator iter = m_mapAbnomal.begin();
	while (iter!=m_mapAbnomal.end())
	{
		QDomElement eleAbnomal = doc.createElement("Abnomal");
		eleAbnomal.setAttribute("type",iter.key());
		eleAbnomal.appendChild(doc.createTextNode(QString("%1").arg(iter.value())));
		eleAbnomals.appendChild(eleAbnomal);
		++iter;
	}
	eleWidget.appendChild(eleAbnomals);

	return true;
}

void CBaseBlockWidget::updateColorBlockData()
{

}

void CBaseBlockWidget::updateSortMode( bool /*bSelFirst = true*/ )
{

}


void CBaseBlockWidget::setColorMode( const QString& mode )
{
	m_qsColorMode = mode;
	//QList<QAction*> listActs = m_pMenuColorMode->actions();
	//foreach(QAction* pAct,listActs)
	//{
	//	pAct->setChecked((pAct->data().toString() == mode) ? true : false);
	//}

	update();
}

void CBaseBlockWidget::onSetColorMode()
{
	QAction* pAct = reinterpret_cast<QAction*>(sender());
	setColorMode(pAct->data().toString());
}

void CBaseBlockWidget::onSetSortMode()
{
	//���õ�ǰ����ʾ����
	QAction* pAct = reinterpret_cast<QAction*>(sender());
	setSortMode(static_cast<RSortType>(pAct->data().toInt()));
	return;
}

void CBaseBlockWidget::onSetAbnomal()
{
	CAbnomalSettingDlg::ShowAbnomalDlg(m_mapAbnomal);
	updateSortMode();
}

void CBaseBlockWidget::onSetBlockSize()
{
	//��������ɫ���С�ĶԻ�����������ɫ��Ĵ�С
	QDialog dlg(this);
	QGridLayout layout(&dlg);
	QLabel label1(tr("���"),&dlg);
	QLabel label2(tr("�߶�"),&dlg);
	QLineEdit editW(&dlg);
	QLineEdit editH(&dlg);
	QPushButton btnOk(&dlg);
	dlg.setLayout(&layout);
	layout.addWidget(&label1,0,0,1,1);
	layout.addWidget(&label2,1,0,1,1);
	layout.addWidget(&editW,0,1,1,1);
	layout.addWidget(&editH,1,1,1,1);
	layout.addWidget(&btnOk,2,0,1,2);
	btnOk.setText(tr("ȷ��"));
	QIntValidator intValidator(1,100);
	editW.setValidator(&intValidator);
	editH.setValidator(&intValidator);
	editW.setText(QString("%1").arg(m_iCBWidth));
	editH.setText(QString("%1").arg(m_iCBHeight));
	dlg.setWindowTitle(tr("ɫ���С����"));
	connect(&btnOk,SIGNAL(clicked()),&dlg,SLOT(accept()));

	if(QDialog::Accepted != dlg.exec())
		return;

	int iW = editW.text().toInt();
	int iH = editH.text().toInt();
	if((iW>0&&iH>0))
	{
		if(iW!=m_iCBWidth||iH!=m_iCBHeight)
		{
			m_iCBHeight = iH;
			m_iCBWidth = iW;
			update();
		}
	}
	else
	{
		QMessageBox::information(this,tr("��ʾ"),tr("����ɫ���Сʧ�ܣ�����������룡"));
	}
}

void CBaseBlockWidget::setSortMode(RSortType sort)
{
	if(m_sort == sort)
	{
		m_sortOrder = (m_sortOrder==Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
		updateSortMode(true);
	}
	else
	{
		m_sort = sort;
		updateSortMode();
	}
	return;
}

QMenu* CBaseBlockWidget::getCustomMenu()
{
	QAction* pAction = m_pMenu->menuAction();
	if(!m_pMenuCustom->actionGeometry(pAction).isValid())
		m_pMenuCustom->addMenu(m_pMenu);

	{
		//��ӵ�ǰ���е�֧�ֵ���ɫģʽ�˵�
		m_pMenuColorMode->clear();

		QStringList listColors = CColorManager::getBlockColorList();
		foreach(const QString& clr,listColors)
		{
			QAction* pAct = m_pMenuColorMode->addAction(clr,this,SLOT(onSetColorMode()));
			pAct->setData(clr);
			pAct->setCheckable(true);
			if(clr == m_qsColorMode)
				pAct->setChecked(true);
		}
	}

	{
		//���õ�ǰѡ�е�����ʽ
		QList<QAction*> listAct = m_pMenuSortMode->actions();
		foreach(QAction* pAct,listAct)
		{
			pAct->setCheckable(true);
			pAct->setChecked(pAct->data().toInt() == m_sort);
		}
	}


	return m_pMenuCustom;
}

void CBaseBlockWidget::drawColocBlock(QPainter& p,int iY,QVector<float>& vValue)
{
	int nTimes = 1;
	CColorItem* pClrItem = CColorManager::getColorItem(m_qsColorMode);

	QMap<time_t,float>::iterator iter = m_mapShowTimes.begin();
	
	int iMapSize = m_mapTimes.size()-1;
	while(iter!=m_mapShowTimes.end())
	{
		QRectF rtCB = QRectF(iter.value(),iY,m_iCBWidth,m_iCBHeight);
		if(m_mapTimes.contains(iter.key()))
		{
			float f = vValue[iMapSize - m_mapTimes[iter.key()]];
			rtCB.adjust(1,1,-1,-1);
			p.fillRect(rtCB,pClrItem->getColor((int)(f*nTimes)));
		}
		++iter;
	}
}
//ͨ������keyword��ȡ��Ҫ�ڼ��̾�������ʾ������
void CBaseBlockWidget::getKeyWizData(const QString& keyword,QList<KeyWizData*>& listRet)
{
	foreach(const RWidgetOpData& _d,m_listSortOp)
	{
		if(_d.key.indexOf(keyword)>-1)
		{
			KeyWizData* pData = new KeyWizData;
			pData->cmd = CKeyWizard::CmdSort;
			pData->arg = (void*)_d.value;
			pData->desc = _d.desc;
			listRet.push_back(pData);
			if(listRet.size()>20)
				return;
		}
	}
	return CBaseWidget::getKeyWizData(keyword,listRet);
}
//���̾��鴰��ȷ�Ϻ󴥷�
void CBaseBlockWidget::keyWizEntered(KeyWizData* pData)
{
	if(pData->cmd == CKeyWizard::CmdSort)
	{
		setSortMode(static_cast<RSortType>((int)(pData->arg)));
		return;
	}

	return CBaseWidget::keyWizEntered(pData);
}

bool CBaseBlockWidget::isMatchAbnomal( CAbstractStockItem* pItem )
{
	QMap<RAbnomalType,float>::iterator iter = m_mapAbnomal.begin();
	while (iter!=m_mapAbnomal.end())
	{
		switch (iter.key())
		{
		case HighIncrease:
			{
				if(pItem->getIncrease()<iter.value()||_isnan(pItem->getIncrease()))
					return false;
			}
			break;
		case LowIncrease:
			{
				if(pItem->getIncrease()>iter.value()||_isnan(pItem->getIncrease()))
					return false;
			}
			break;
		case HighTurnRatio:
			{
				if(pItem->getTurnRatio()<iter.value()||_isnan(pItem->getTurnRatio()))
					return false;
			}
			break;
		case HighVolumeRatio:
			{
				if(pItem->getVolumeRatio()<iter.value()||_isnan(pItem->getVolumeRatio()))
					return false;
			}
			break;
		case HighAvgVolume:
			{
				//ÿ�ʾ��� ��
				;
			}
			break;
		case HighPrice:
			{
				//�۸����ĳֵ
				if(pItem->getNewPrice()<iter.value()||_isnan(pItem->getNewPrice()))
					return false;
			}
			break;
		case LowPrice:
			{
				//�۸�С��ĳֵ
				if(pItem->getNewPrice()>iter.value()||_isnan(pItem->getNewPrice()))
					return false;
			}
			break;
		case HighLTSZ:
			{
				//����ͨ��
				if(pItem->getLTSZ()<iter.value()||_isnan(pItem->getLTSZ()))
					return false;
			}
			break;
		case LowLTSZ:
			{
				//����ͨ��
				if(pItem->getLTSZ()>iter.value()||_isnan(pItem->getLTSZ()))
					return false;
			}
			break;
		case HighMGSY:
			{
				//��ÿ������
				if(pItem->getMgsy()<iter.value()||_isnan(pItem->getMgsy()))
					return false;
			}
			break;
		case HighPERatio:
			{
				//����ӯ��
				if(pItem->getPERatio()<iter.value()||_isnan(pItem->getPERatio()))
					return false;
			}
			break;
		case HighZYYWRatio:
			{
				//����Ӫҵ�������� ��
			}
			break;
		case HighYJYZ:
			{
				//��ҵ��Ԥ��
			}
			break;
		case HighYJZZ:
			{
				//��ҵ������
			}
			break;
		case HighYJHBZZ:
			{
				//��ҵ��������������4������
			}
			break;
		case HighJZCSYL:
			{
				//���ʲ�������   >=
			}
			break;
		case HighBuyVolume:
			{
				//ί��1+2+3>=
				if(pItem->getBIDVOL3()<iter.value()||_isnan(pItem->getBIDVOL3()))
					return false;
			}
			break;
		case HighSellVolume:
			{
				//ί��1+2+3>=
				if(pItem->getASKVOL3()<iter.value()||_isnan(pItem->getASKVOL3()))
					return false;
			}
			break;
		case HighCommRatio:
			{
				//ί�� >=
				if(pItem->getCommRatio()<iter.value()||_isnan(pItem->getCommRatio()))
					return false;
			}
		default:
			break;
		}
		++iter;
	}

	return true;
}

void CBaseBlockWidget::updateTimesH()
{
	//���µ�ǰ�ĺ��������ݣ��Ӻ���ǰ����ʱ��
	m_mapTimes = CDataEngine::getTodayTimeMap(Min5);
}

void CBaseBlockWidget::updateShowTimes( const QRectF& rtCoordX,float fItemWidth )
{
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

void CBaseBlockWidget::drawCoordX( QPainter& p,const QRectF& rtCoordX,float fItemWidth )
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

		--iCount;
		fCurX = fCurX- fItemWidth;
	}
	return;
}
