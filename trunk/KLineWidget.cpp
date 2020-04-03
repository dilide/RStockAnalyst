#include "StdAfx.h"
#include "KLineWidget.h"
#include "DataEngine.h"
#include "KeyWizard.h"
#include "ColorManager.h"
#include <Windows.h>

#define	KLINE_BORDER	2
#define TIME_OF_DAY		(3600*24)

CKLineWidget::CKLineWidget( CBaseWidget* parent /*= 0*/ )
	: CCoordXBaseWidget(parent,WidgetKLine)
	, m_pStockItem(0)
	, m_iShowCount(100)
	, m_iCurExp(0)
	, m_bShowMax(false)
	, m_iTitleHeight(16)
	, m_iCoorYWidth(40)
	, m_iCoorXHeight(16)
	, m_iMainLinerHeight(200)
	, m_mapData(NULL)
	, m_bLock(false)
	, m_tmLastUpdate(QDateTime::currentDateTime())
	, m_iFenShiCount(1)
	, m_iFenShiAsis(FenShiVol)
	, m_iFenShiCoordY(PercentAuto)
	, m_bYCoordPercent(true)
	, m_bYCoordAuto(false)
{
	m_typeCircle = Min1;

	m_vSizes.push_back(60);
	m_vExps.push_back("DrawK(OPEN,CLOSE,HIGH,LOW)");

	m_pMenuCustom->addAction(tr("���ù�Ʊ����"),this,SLOT(onSetStockCode()));
	{
		m_pMenuCustom->addSeparator();
		QMenu* pMenuDeputy = m_pMenuCustom->addMenu(tr("��Ӹ�ͼ"));
		pMenuDeputy->addAction(tr("��ͨ��ͼ"),this,SLOT(onAddDeputy()));
		pMenuDeputy->addAction(tr("����ͼ"),this,SLOT(onAddVolume()));

	}
	m_pMenuCustom->addAction(tr("ɾ����ͼ"),this,SLOT(onRemoveDeputy()));
	m_pMenuCustom->addAction(tr("���ñ��ʽ"),this,SLOT(onSetExpression()));
	m_pMenuCustom->addAction(tr("��������ͼ����ʾ����"),this,SLOT(onSetSizes()));

	m_pMenuAdd2Block = m_pMenuCustom->addMenu(tr("��ӵ���ѡ���"));

	m_pMenuYCoordMode = m_pMenuCustom->addMenu(tr("Y����ʾ��ʽ"));

//	setMinimumSize(200,200);
}

CKLineWidget::~CKLineWidget(void)
{
	clearTmpData();

	delete m_pMenuCustom;
}

bool CKLineWidget::loadPanelInfo( const QDomElement& eleWidget )
{
	if(!CCoordXBaseWidget::loadPanelInfo(eleWidget))
		return false;

	m_vSizes.clear();
	m_vExps.clear();

	QDomElement eleLiners = eleWidget.firstChildElement("liners");
	if(eleLiners.isElement())
	{
		//�������е�Liner
		QDomElement eleLiner = eleLiners.firstChildElement("Liner");
		while(eleLiner.isElement())
		{
			m_vSizes.push_back(eleLiner.attribute("size").toInt());

			QDomElement eleExp = eleLiner.firstChildElement("exp");
			if(eleExp.isElement())
			{
				m_vExps.push_back(eleExp.text());
			}
			else
			{
				m_vExps.push_back("DrawK(OPEN,CLOSE,HIGH,LOW)");
			}

			eleLiner = eleLiner.nextSiblingElement("Liner");
		}
	}

	QDomElement eleCode = eleWidget.firstChildElement("code");
	if(eleCode.isElement())
		setStockCode(eleCode.text());

	QDomElement eleYCoordAuto = eleWidget.firstChildElement("YCoordAuto");
	if(eleYCoordAuto.isElement())
		m_bYCoordAuto = eleYCoordAuto.text().toInt();
	QDomElement eleYCoordPercent = eleWidget.firstChildElement("YCoordPercent");
	if(eleYCoordPercent.isElement())
		m_bYCoordPercent = eleYCoordPercent.text().toInt();

	return true;
}

bool CKLineWidget::savePanelInfo( QDomDocument& doc,QDomElement& eleWidget )
{
	if(!CCoordXBaseWidget::savePanelInfo(doc,eleWidget))
		return false;
	if(m_pStockItem)
	{
		//��ǰ�Ĺ�Ʊֵ
		QDomElement eleCode = doc.createElement("code");
		eleCode.appendChild(doc.createTextNode(m_pStockItem->getOnly()));
		eleWidget.appendChild(eleCode);


		QDomElement eleLiners = doc.createElement("liners");
		eleWidget.appendChild(eleLiners);
		{
			//�������еĸ�ͼ��Ϣ
			for(int i=0;i<m_vSizes.size();++i)
			{
				QDomElement eleLiner = doc.createElement("Liner");
				eleLiners.appendChild(eleLiner);
				int iSize = m_vSizes[i];
				eleLiner.setAttribute("size",iSize);

				QDomElement eleExp = doc.createElement("exp");
				eleExp.appendChild(doc.createTextNode(m_vExps[i]));
				eleLiner.appendChild(eleExp);
			}
		}

		QDomElement eleYCoordAuto = doc.createElement("YCoordAuto");
		eleYCoordAuto.appendChild(doc.createTextNode(QString("%1").arg(m_bYCoordAuto)));
		eleWidget.appendChild(eleYCoordAuto);

		QDomElement eleYCoordPercent = doc.createElement("YCoordPercent");
		eleYCoordPercent.appendChild(doc.createTextNode(QString("%1").arg(m_bYCoordPercent)));
		eleWidget.appendChild(eleYCoordPercent);
	}

	return true;
}

void CKLineWidget::updateData()
{
	resetTmpData();
	return /*CCoordXBaseWidget::updateData()*/;
}

void CKLineWidget::setStockCode( const QString& only )
{
	CStockInfoItem* pItem = CDataEngine::getDataEngine()->getStockInfoItem(only);
	if(pItem==NULL)
	{
		pItem = CDataEngine::getDataEngine()->getStockInfoItemByCode(only);
	}
	if(pItem)
	{
		setStockItem(pItem);
	}

	return CBaseWidget::setStockCode(only);
}

void CKLineWidget::setBlock( const QString& block )
{
	CBlockInfoItem* pBlock = CDataEngine::getDataEngine()->getStockBlock(block);
	if(pBlock)
	{
		setStockItem(pBlock);
	}

	return CBaseWidget::setBlock(block);
}

void CKLineWidget::setStockItem( CAbstractStockItem* pItem )
{
	QDateTime tmNow = QDateTime::currentDateTime();
/*	if(m_tmLastUpdate.msecsTo(tmNow)<100)
		return;
	if(m_bLock)
	{
		return;
	}
*/
	m_bLock = true;
	m_tmLastUpdate = tmNow;

	if(pItem)
	{
		if(m_pStockItem)
		{
			//�Ƴ����к� updateKLine������ �ź�/��
			disconnect(m_pStockItem,SIGNAL(stockItemHistoryChanged(const QString&)),this,SLOT(updateDayLine(const QString&)));
			disconnect(m_pStockItem,SIGNAL(stockItemFenBiChanged(const QString&)),this,SLOT(updateMinLine(const QString&)));
		}

		//����Ĭ����ʾ100��K��
		if((pItem!=m_pStockItem)&&(m_iShowCount<50))
			m_iShowCount = 100;

		m_pStockItem = pItem;
		//�������»���
		connect(pItem,SIGNAL(stockItemHistoryChanged(const QString&)),this,SLOT(updateDayLine(const QString&)));
		connect(pItem,SIGNAL(stockItemFenBiChanged(const QString&)),this,SLOT(updateMinLine(const QString&)));

		//����K��ͼ
		resetTmpData();
	}
	m_bLock = false;
}

void CKLineWidget::updateTimesH()
{
	//���µ�ǰ�ĺ��������ݣ��Ӻ���ǰ����ʱ��
	if(m_typeCircle == FenShi)
	{
		m_mapTimes.clear();
		//����7���ڵķ�ʱ������
		time_t tmDay = (CDataEngine::getCurrentTime()/TIME_OF_DAY);
		for (int i=0; i<m_iFenShiCount; ++i)
		{
			time_t tmCur = ((tmDay-i)*TIME_OF_DAY) + 3600*7;	//15:00����
			time_t tmLast = tmCur-1800*11;						//9:30����
			time_t tmNoon1 = tmCur-1800*7;						//11:30����
			time_t tmNoon2 = tmCur-1800*4;						//13:00���¿���
			while(tmCur>=tmNoon2)
			{
				m_mapTimes[tmCur] = m_mapTimes.size();
				tmCur-=60;
			}
			tmCur = tmNoon1;
			while(tmCur>=tmLast)
			{
				m_mapTimes[tmCur] = m_mapTimes.size();
				tmCur-=60;
			}
		}
	}
	else if(m_typeCircle<Day)
		m_mapTimes = CDataEngine::getTodayTimeMap(m_typeCircle);
	else
		m_mapTimes = CDataEngine::getHistoryTimeMap(m_typeCircle,m_iShowCount*2);
}

QMap<time_t,RStockData*>* CKLineWidget::getColorBlockMap( CAbstractStockItem* pItem )
{
	if(!pItem)
		return new QMap<time_t,RStockData*>();
	QMap<time_t,RStockData*>* pMap = NULL;
	if(m_typeCircle < Day)
	{
		//��ȡ�������ݣ����м���
		QList<qRcvFenBiData*> FenBis = pItem->getFenBiList();
		pMap = CDataEngine::getColorBlockItems(m_mapTimes,FenBis);
	}
	else
	{
		//��ȡ��������
		int iItemCount = m_iShowCount*2;
		QList<qRcvHistoryData*> historys = pItem->getLastHistory(iItemCount);
		qRcvReportData* pLastReport = pItem->getCurrentReport();
		qRcvHistoryData* pLastData = 0;
		if(historys.size()>0 && pLastReport)
		{
			qRcvHistoryData* pLastHistory = historys.last();
			if(QDateTime::fromTime_t(pLastHistory->time).date() < QDateTime::fromTime_t(pLastReport->tmTime).date())
			{
				pLastData = new qRcvHistoryData(pLastReport);
			}
		}
		if(pLastData)
		{
			historys.push_back(pLastData);
		}
		pMap = CDataEngine::getColorBlockItems(m_mapTimes,historys);
		if(pLastData)
		{
			//�����ȡ����������
			delete pLastData;
		}
	}

	return pMap;
}

void CKLineWidget::updateMinLine( const QString& only )
{
	if(m_typeCircle>=Day)
		return;
	if(m_pStockItem&&m_pStockItem->getOnly()!=only)
		return;

	resetTmpData();
}

void CKLineWidget::updateDayLine( const QString& only )
{
	if(m_typeCircle<Day)
		return;
	if(m_pStockItem&&m_pStockItem->getOnly()!=only)
		return;

	resetTmpData();
}

void CKLineWidget::paintEvent( QPaintEvent* )
{
	QPainter p(this);
//	p.setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing,true);
	QRect rtClient = this->rect();
	p.fillRect(rtClient,QColor(0,0,0));
	if(!m_pStockItem)
		return;

	if(m_typeCircle == FenShi)
	{
		//���Ʒ�ʱͼ
		drawFenShi(p,rtClient);
		return;
	}

	if(m_iShowCount>m_mapData->size())
	{
		m_iShowCount = m_mapData->size();
	}

	if(!m_bClearMode)
	{
		/*��ͷ��*/
		QRect rtTitle = QRect(rtClient.left(),rtClient.top(),rtClient.width(),m_iTitleHeight);
		drawTitle(p,rtTitle);

		/*��X������*/
		QRectF rtCoordX = QRectF(rtClient.left()+3,rtClient.bottom()-m_iCoorXHeight+1,rtClient.width()-m_iCoorYWidth-5,m_iCoorXHeight);
		m_fItemWidth = float(rtCoordX.width()-1)/float(m_iShowCount);
		drawCoordX(p,rtCoordX,m_fItemWidth);

		rtClient.adjust(3,m_iTitleHeight,-m_iCoorYWidth-2,-m_iCoorXHeight);			//�ı�Ϊ�ɻ�ͼ����Ĵ�С
	}
	else
	{
		rtClient.adjust(3,0,-2,0);			//�ı�Ϊ�ɻ�ͼ����Ĵ�С
	}
	/*
	//�����½ǵ�������ť
	QRect rtShowBtns = QRect(rtClient.right()-m_iCoorYWidth,rtClient.bottom()-m_iCoorXHeight,m_iCoorYWidth,m_iCoorXHeight);
	drawShowBtns(p,rtShowBtns);
	*/

	m_rtClient = rtClient;

	if(m_bShowMax&&m_iCurExp<m_vExps.size())
	{
		QString qsExp = m_vExps[m_iCurExp];		//��ȡ���ʽ

		//����ȫ������
		RDrawInfo draw;
		draw.dwVersion = RSTOCK_VER;
		draw.pPainter = &p;
		draw.rtClient = rtClient;
		draw.fItemWidth = m_fItemWidth;
		draw.iEndIndex = m_mapData->size()-1;
		draw.iCurColor = 0;
		drawExpArgs(p,rtClient,qsExp,draw.lsColors);
		lua_pushlightuserdata(m_pL,&draw);
		lua_setglobal(m_pL,"_draw");

		luaL_dostring(m_pL,qsExp.toLocal8Bit());

		drawCoordY(p,QRectF(rtClient.right(),rtClient.top(),m_iCoorYWidth,rtClient.height()),draw.fMax,draw.fMin);
	}
	else
	{
		int iCurY = rtClient.top();		//��ǰ���Ƶ���λ��
		for(int i=0;i<m_vSizes.size();++i)
		{
			int iTotal = 0;					//�����ܺ�
			for (int j=i;j<m_vSizes.size();++j)
				iTotal += m_vSizes[j];

			int iTotalHeight = rtClient.bottom()-iCurY;
			int iHeight = (float)((float)m_vSizes[i]/float(iTotal))*iTotalHeight + 0.5;

			QRectF rtArea = QRectF(rtClient.left(),iCurY,rtClient.width(),iHeight);
			{
				QString qsExp = m_vExps[i];		//��ȡ���ʽ
				RDrawInfo draw;
				draw.dwVersion = RSTOCK_VER;
				draw.pPainter = &p;
				draw.rtClient = rtArea;
				draw.fItemWidth = m_fItemWidth;
				draw.iEndIndex = m_mapData->size()-1;
				draw.iCurColor = 0;
				drawExpArgs(p,rtArea.toRect(),qsExp,draw.lsColors);
				lua_pushlightuserdata(m_pL,&draw);
				lua_setglobal(m_pL,"_draw");

				luaL_dostring(m_pL,qsExp.toLocal8Bit());

				drawCoordY(p,QRectF(rtArea.right(),rtArea.top(),50,rtArea.height()),draw.fMax,draw.fMin);
			}

			iCurY += iHeight;
		}
	}
	return;
}

void CKLineWidget::mouseMoveEvent( QMouseEvent* e )
{
	if(!((qApp->mouseButtons())&Qt::LeftButton))
	{
		update();
		QToolTip::hideText();
		return CCoordXBaseWidget::mouseMoveEvent(e);
	}
	if(!m_pStockItem)
	{
		return QToolTip::hideText();
	}
	if(!m_rtClient.contains(e->pos()))
		return QToolTip::hideText();

	float fEnd = m_rtClient.right();
	int iLastN = (fEnd - e->posF().x())/m_fItemWidth;
	if(iLastN<0||iLastN>=m_iShowCount)
		return QToolTip::hideText();

	RStockData* pData = NULL;
	int iIndex = 0;
	QMap<time_t,RStockData*>::iterator iter = m_mapData->end();
	while(iter!=m_mapData->begin())
	{
		--iter;
		if(iIndex == iLastN)
		{
			pData = iter.value();
			break;
		}
		if(iIndex>iLastN)
			break;
		++iIndex;
	}
	
	if(pData)
	{
		QString qsTooltip;		//Tips
		QString qsTime;
		if(m_typeCircle<Day)
		{
			time_t tmToday = QDateTime(QDateTime::fromTime_t(m_pStockItem->getCurrentReport()->tmTime).date()).toTime_t();
			if(pData->tmTime>tmToday)
				qsTime = QDateTime::fromTime_t(pData->tmTime).toString("hh:mm:ss");
			else
				qsTime = QDateTime::fromTime_t(pData->tmTime).toString("MM/dd hh:mm");
		}
		else
		{
			QDate dtTmp = QDateTime::fromTime_t(pData->tmTime).date();
			if(m_typeCircle == Week)
				qsTime = QString("%1 %2").arg(dtTmp.year()).arg(dtTmp.weekNumber());
			else if(m_typeCircle == Month)
				qsTime = dtTmp.toString("yyyy/MM");
			else if(m_typeCircle == Month3)
				qsTime = dtTmp.toString("yyyy/MM");
			else if(m_typeCircle == Year)
				qsTime = dtTmp.toString("yyyy");
			else
				qsTime = dtTmp.toString("yyyy/MM/dd");
		}
	
		qsTooltip = QString("��Ʊ����:%1\r\nʱ��:%2\r\n���¼�:%7\r\n��߼�:%3\r\n��ͼ�:%4\r\n���̼�:%8\r\n�ɽ���:%5\r\n�ɽ���:%6")
			.arg(m_pStockItem->getCode()).arg(qsTime).arg(pData->fHigh).arg(pData->fLow)
			.arg(pData->fVolume).arg(pData->fAmount).arg(pData->fClose).arg(pData->fOpen);

		QToolTip::showText(e->globalPos(),qsTooltip,this);
	}
	else
	{
		QToolTip::hideText();
	}

	return CCoordXBaseWidget::mouseMoveEvent(e);
}

void CKLineWidget::leaveEvent( QEvent* e )
{
	update();
	return CBaseWidget::leaveEvent(e);
}

void CKLineWidget::mousePressEvent( QMouseEvent* e )
{
	QPoint ptCur = e->pos();
	QRect rtClient = rect();
	rtClient.adjust(3,m_iTitleHeight,-m_iCoorYWidth-2,-m_iCoorXHeight);			//�ı�Ϊ�ɻ�ͼ����Ĵ�С

	int iCurY = rtClient.top();		//��ǰ���Ƶ���λ��
	for(int i=0;i<m_vSizes.size();++i)
	{
		int iTotal = 0;					//�����ܺ�
		for (int j=i;j<m_vSizes.size();++j)
			iTotal += m_vSizes[j];

		int iTotalHeight = rtClient.bottom()-iCurY;
		int iHeight = (float)((float)m_vSizes[i]/float(iTotal))*iTotalHeight + 0.5;

		QRectF rtArea = QRectF(rtClient.left(),iCurY,rtClient.width(),iHeight);
		if(rtArea.contains(ptCur))
		{
			m_iCurExp = i;
			break;
		}

		iCurY += iHeight;
	}

	if(e->button()==Qt::LeftButton)
	{
		if(m_rtTitle.contains(ptCur))
		{
			QMap<int,QRect>::iterator iter = m_mapCircles.begin();
			while(iter!=m_mapCircles.end())
			{
				if((*iter).contains(ptCur))
				{
					setCircle(static_cast<RStockCircle>(iter.key()));
					break;
				}
				++iter;
			}
		}
		else if(m_rtAddShow.contains(ptCur))
		{
			onClickedAddShow();
		}
		else if(m_rtSubShow.contains(ptCur))
		{
			onClickedSubShow();
		}
	}

	return CCoordXBaseWidget::mousePressEvent(e);
}

void CKLineWidget::mouseDoubleClickEvent( QMouseEvent* e )
{
	QPoint ptCur = e->pos();
	QRect rtClient = rect();
	rtClient.adjust(3,m_iTitleHeight,-m_iCoorYWidth-2,-m_iCoorXHeight);			//�ı�Ϊ�ɻ�ͼ����Ĵ�С
	if(rtClient.contains(ptCur))
	{
		m_bShowMax = !m_bShowMax;
		update();
	}
}

void CKLineWidget::keyPressEvent(QKeyEvent* e)
{
	int iKey = e->key();
	if(iKey == Qt::Key_Up)
	{
		if(m_typeCircle == FenShi)
		{
			if(m_iFenShiCount>1)
				--m_iFenShiCount;
			return updateData();
		}
		else
		{
			int iShowCount = m_iShowCount*0.9;
			if(iShowCount>=m_iShowCount)
				iShowCount = m_iShowCount-2;
			return setShowCount(iShowCount);
		}
	}
	else if(iKey == Qt::Key_Down)
	{
		if(m_typeCircle == FenShi)
		{
			if(m_iFenShiCount<7)
				++m_iFenShiCount;
			return updateData();
		}
		else
		{
			int iShowCount = m_iShowCount*1.1;
			if(iShowCount<=m_iShowCount)
				iShowCount = m_iShowCount+5;
			return setShowCount(iShowCount);
		}
	}
	else if(Qt::Key_F10 == iKey)
	{
		//F10����
		if(m_pStockItem)
		{
			if(!CDataEngine::getDataEngine()->showF10Data(m_pStockItem->getCode()))
			{
				//δ��F10���� do something
			}
		}
		return;
	}
	else if(Qt::Key_F11 == iKey)
	{
		m_bClearMode = !m_bClearMode;
		update();
		return;
	}

	return CBaseWidget::keyPressEvent(e);
}

QMenu* CKLineWidget::getCustomMenu()
{
	QAction* pAction = m_pMenu->menuAction();
	if(!m_pMenuCustom->actionGeometry(pAction).isValid())
		m_pMenuCustom->addMenu(m_pMenu);

	{
		QList<QAction*> listAct = m_pMenuCircle->actions();
		foreach(QAction* pAct,listAct)
		{
			pAct->setCheckable(true);
			pAct->setChecked(pAct->data().toInt() == m_typeCircle);
		}
	}

	{
		//��ӵ���ѡ���
		m_pMenuAdd2Block->clear();
		CBlockInfoItem* pCustomBlock = CDataEngine::getDataEngine()->getCustomBlock();
		if(pCustomBlock)
		{
			foreach(CBlockInfoItem* pBlock,pCustomBlock->getBlockList())
			{
				m_pMenuAdd2Block->addAction(pBlock->getBlockName(),this,SLOT(onAdd2Block()))->setData(uint(pBlock));
			}
		}
		m_pMenuAdd2Block->addSeparator();
		m_pMenuAdd2Block->addAction(tr("��ӵ��½����"),this,SLOT(onAdd2NewBlock()));
	}
	{
		//����Y����ʾ��ʽ
		m_pMenuYCoordMode->clear();
		QAction* pActAuto = m_pMenuYCoordMode->addAction(tr("�Զ���ʾ��Χ"),this,SLOT(onSetYCoordAuto()));
		QAction* pActPercent = m_pMenuYCoordMode->addAction(tr("��ʾ�ٷֱ�"),this,SLOT(onSetYCoordPercent()));
		pActAuto->setCheckable(true);
		pActPercent->setCheckable(true);

		pActAuto->setChecked(m_bYCoordAuto);
		pActPercent->setChecked(m_bYCoordPercent);
	}
	return m_pMenuCustom;
}

void CKLineWidget::onSetStockCode()
{
	QDialog dlg(this);
	QVBoxLayout layout(&dlg);
	QLineEdit edit(&dlg);
	QPushButton btnOk(&dlg);
	dlg.setLayout(&layout);
	layout.addWidget(&edit);
	layout.addWidget(&btnOk);
	btnOk.setText(tr("ȷ��"));
	connect(&btnOk,SIGNAL(clicked()),&dlg,SLOT(accept()));

	if(QDialog::Accepted != dlg.exec())
		return;

	QString code = edit.text();
	setStockCode(code);
}

void CKLineWidget::onSetExpression()
{
	if(m_iCurExp>=m_vExps.size())
		return;

	QDialog dlg(this);
	QVBoxLayout layout(&dlg);
	QTextEdit edit(&dlg);
	QPushButton btnOk(&dlg);
	layout.addWidget(&edit);
	layout.addWidget(&btnOk);
	dlg.setLayout(&layout);
	btnOk.setText(tr("ȷ��"));
	edit.setText(m_vExps[m_iCurExp]);
	connect(&btnOk,SIGNAL(clicked()),&dlg,SLOT(accept()));

	if(QDialog::Accepted != dlg.exec())
		return;

	m_vExps[m_iCurExp] = edit.toPlainText();
}

void CKLineWidget::onClickedAddShow()
{
	setShowCount(m_iShowCount+10);
}

void CKLineWidget::onClickedSubShow()
{
	setShowCount(m_iShowCount-10);
}

void CKLineWidget::onAddDeputy()
{
	m_vSizes.push_back(20);
	m_vExps.push_back("DrawLine(CLOSE)");
	update();
}

void CKLineWidget::onAddVolume()
{
	m_vSizes.push_back(20);
	m_vExps.push_back("DrawHist(VOLUME)");
	update();
}


void CKLineWidget::onRemoveDeputy()
{
	//ɾ����ǰѡ�еĸ�ͼ
	if(m_iCurExp<m_vSizes.size())
	{
		m_vSizes.remove(m_iCurExp);
		m_vExps.remove(m_iCurExp);
		m_iCurExp = 0;
		update();
	}
}

void CKLineWidget::onSetSizes()
{
	//�������û��༭���Դ��ڱ����ĶԻ���
	QDialog dlg(this);
	QGridLayout layout(&dlg);
	QPushButton btnOk(this);
	dlg.setLayout(&layout);

	int iCount = m_vSizes.size();
	QVector<QLabel*> vLabels;
	QVector<QSpinBox*> vSpins;
	for (int i=0;i<iCount;++i)
	{
		//���ó�ʼ�ı���
		QLabel* pLabel = new QLabel(&dlg);
		pLabel->setText(QString("%1").arg(i+1));
		vLabels.push_back(pLabel);
		QSpinBox* pSpin = new QSpinBox(&dlg);
		pSpin->setMinimum(1);
		pSpin->setMaximum(100);
		pSpin->setValue(m_vSizes[i]);
		vSpins.push_back(pSpin);

		layout.addWidget(pLabel,i,0);
		layout.addWidget(pSpin,i,1);
	}
	layout.addWidget(&btnOk,iCount,0,1,2);
	btnOk.setText(tr("ȷ��"));
	connect(&btnOk,SIGNAL(clicked()),&dlg,SLOT(accept()));

	if(QDialog::Accepted != dlg.exec())
		return;

	m_vSizes.clear();			//���֮ǰ�ı�������

	for (int i=0;i<iCount;++i)
	{
		m_vSizes.push_back(vSpins[i]->value());
		delete vLabels[i];
		delete vSpins[i];
	}
}

void CKLineWidget::onAdd2Block()
{
	if(!m_pStockItem)
		return;
	if(!m_pStockItem->isInstanceOfStock())
		return;
	{
		QAction* pAct = reinterpret_cast<QAction*>(sender());
		CBlockInfoItem* pBlock = reinterpret_cast<CBlockInfoItem*>(pAct->data().toUInt());
		if(pBlock)
		{
			pBlock->addStockInfo(reinterpret_cast<CStockInfoItem*>(m_pStockItem));
		}
	}
}

void CKLineWidget::onAdd2NewBlock()
{
	if(!m_pStockItem->isInstanceOfStock())
		return;

	if(!m_pStockItem)
		return;

	CBlockInfoItem* pCustomBlock = CDataEngine::getDataEngine()->getCustomBlock();
	if(!pCustomBlock)
		return;

	QDialog dlg(this);
	QVBoxLayout layout(this);
	QLineEdit edit(this);
	QPushButton btnOk(this);
	dlg.setLayout(&layout);
	layout.addWidget(&edit);
	layout.addWidget(&btnOk);
	btnOk.setText(tr("ȷ��"));
	connect(&btnOk,SIGNAL(clicked()),&dlg,SLOT(accept()));

	if(QDialog::Accepted != dlg.exec())
		return;

	QString block = edit.text();
	if(pCustomBlock->hasBlock(block))
	{
		QMessageBox::information(this,tr("�½���ѡ���ʧ��"),tr("�Ѵ���ͬ�����"));
		return;
	}

	QString qsFilePath = QString("%1/%2").arg(pCustomBlock->getFilePath()).arg(block);
	QFile file(qsFilePath);
	file.open(QFile::WriteOnly);
	file.close();
	CBlockInfoItem* pBlock = new CBlockInfoItem(qsFilePath,pCustomBlock);
	if(pBlock)
	{
		pCustomBlock->addBlockInfo(pBlock);
		pBlock->addStockInfo(reinterpret_cast<CStockInfoItem*>(m_pStockItem));
	}
}


void CKLineWidget::setShowCount(int _iShow)
{
	m_iShowCount = _iShow;
	updateData();
	if(_iShow>m_mapData->size())
	{
		m_iShowCount = m_mapData->size();
	}
	else if(_iShow<1)
		m_iShowCount = 1;
	else
		m_iShowCount = _iShow;
	update();
}

void CKLineWidget::drawTitle( QPainter& p,const QRect& rtTitle )
{
	if(!rtTitle.isValid())
		return;

	m_rtTitle = rtTitle;
	QRect rtClient = rtTitle.adjusted(2,0,-2,0);
	QString qsName = m_pStockItem->getName();
	if(qsName.isEmpty())
		qsName = m_pStockItem->getCode();

	p.setPen(QColor(127,0,0));
	p.drawRect(rtClient);

	p.setPen(QColor(255,255,255));
	p.drawText(rtClient,Qt::AlignLeft|Qt::AlignVCenter,qsName);

	int iSize = m_listCircle.size();
	int iRight = rtTitle.right();
	int iTop = rtTitle.top();
	for (int i=iSize-1;i>=0;--i)
	{
		QRect rtCircle = QRect(iRight-(iSize-i)*16-20,iTop+2,12,12);
		m_mapCircles[m_listCircle[i].value] = rtCircle;
		if(m_typeCircle == m_listCircle[i].value)
		{
			p.fillRect(rtCircle,QColor(127,127,127));
		}
		else
		{
			p.setPen(QColor(240,240,240));
			p.drawRect(rtCircle);
		}
		p.setPen(QColor(255,255,255));
		p.drawText(rtCircle,m_listCircle[i].desc.left(1),QTextOption(Qt::AlignCenter));
	}
}

void CKLineWidget::drawShowBtns( QPainter& p,const QRect& rtBtns )
{
	p.setPen(QColor(255,0,0));
	int iBtnWidth = 16;
	int iBtnHeight = rtBtns.height()-1;
	m_rtAddShow = QRect(rtBtns.right()-2*iBtnWidth,rtBtns.top()+1,iBtnWidth,iBtnHeight);
	m_rtSubShow = QRect(rtBtns.right()-iBtnWidth,rtBtns.top()+1,iBtnWidth,iBtnHeight);
	
	p.drawRect(m_rtAddShow);
	p.drawRect(m_rtSubShow);

	p.drawText(m_rtAddShow,Qt::AlignCenter,QString("+"));
	p.drawText(m_rtSubShow,Qt::AlignCenter,QString("-"));
}

void CKLineWidget::drawExpArgs( QPainter& p,const QRect& rtClient, 
							   const QString& e,QList<uint>& lsColor )
{
	QFontMetrics fm(p.font());
	int x = rtClient.left()+5;
	int y = rtClient.top() + fm.height();

	QStringList listExps = e.split("\n");
	foreach(const QString& _e,listExps)
	{
		int _i = _e.indexOf("DrawLine");
		if(_i>-1)
		{
			_i += 9;
			QString _arg = _e.mid(_i,_e.lastIndexOf(")")-_i);
			uint clPen = CColorManager::getCommonColor(lsColor.size());
			lsColor.push_back(clPen);
			p.setPen(QColor::fromRgb(clPen));
			p.drawText(x,y,_arg+";");
			x += (fm.width(_arg)+20);
		}
	}
}

void CKLineWidget::clearTmpData()
{
	//foreach(stLinerItem* p,listItems)
	//	delete p;
	if(!m_pStockItem)
		return;
	if(m_mapData)
	{
		if(m_typeCircle<Day)
		{
			if(CDataEngine::getCurrentTime()>0)
			{
				if(m_typeCircle<Min1 && m_typeCircle!=FenShi)
				{
					QMap<time_t,RStockData*>::iterator iter = m_mapData->begin();
					while(iter!=m_mapData->end())
					{
						delete (*iter);
						++iter;
					}
				}
			}
		}
		else
		{
			QMap<time_t,RStockData*>::iterator iter = m_mapData->begin();
			while(iter!=m_mapData->end())
			{
				delete iter.value();
				++iter;
			}
		}
		m_mapData->clear();
		delete m_mapData;
		m_mapData = NULL;
	}
	disconnect(this,SLOT(updateMinLine(const QString&)));
	disconnect(this,SLOT(updateDayLine(const QString&)));
}

void CKLineWidget::resetTmpData()
{
	clearTmpData();
	if(!m_pStockItem)
		return;
	updateTimesH();			//�������µ�ʱ��������

	if(m_typeCircle == FenShi)
	{
		m_mapData = new QMap<time_t,RStockData*>();
		QList<RStockData*> listData = m_pStockItem->getMinList();
		foreach(RStockData* pData,listData)
		{
			m_mapData->insert(pData->tmTime,pData);
		}

		update();
		return;
	}
	else if(m_typeCircle<Min1 || m_typeCircle>Min60)
	{
		m_mapData = getColorBlockMap(m_pStockItem);
		{
			//����K��ͼ��ȥ����ֵ
			QMap<time_t,RStockData*>::iterator iter = m_mapData->begin();
			while(iter != m_mapData->end())
			{
				time_t tmRemove = 0;
				if(iter.value()==NULL)
				{
					tmRemove = iter.key();
				}
				++iter;

				if(tmRemove!=0)
				{
					m_mapData->remove(tmRemove);
					m_mapTimes.remove(tmRemove);
				}
			}
		}
	}
	else
	{
		QList<RStockData*> listData = m_pStockItem->getMinList(m_typeCircle);
		m_mapData = new QMap<time_t,RStockData*>();
		m_mapTimes.clear();

		int iCount = listData.count();
		for(int i=(iCount-1);i>=0;--i)
		{
			RStockData* _p = listData[i];
			m_mapData->insert(_p->tmTime,_p);
			m_mapTimes.insert(_p->tmTime,m_mapTimes.size());
		}
	}

	if(m_mapData->size()!=m_mapTimes.size())
	{
		qDebug()<<"Something wrong in 'CKLineWidget::resetTmpData()'!";
	}
	/*�����º���������õ��ű�������*/
	{
		RCalcInfo calc;
		calc.dwVersion = RSTOCK_VER;
		calc.emCircle = m_typeCircle;
		calc.mapData = m_mapData;
		calc.mapDataEx = NULL;
		calc.pItem = m_pStockItem;
		
		lua_pushlightuserdata(m_pL,&calc);
		lua_setglobal(m_pL,"_calc");

		lua_getglobal(m_pL,"InitValues");
		lua_call(m_pL,0,0);
	}

	//���½���
	update();
}

void CKLineWidget::getKeyWizData( const QString& keyword,QList<KeyWizData*>& listRet )
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

	foreach(CStockInfoItem* pItem,CDataEngine::getDataEngine()->getStockInfoList())
	{
		if(pItem->isMatch(keyword))
		{
			KeyWizData* pData = new KeyWizData;
			pData->cmd = CKeyWizard::CmdStock;
			pData->arg = pItem;
			pData->desc = QString("%1 %2").arg(pItem->getCode()).arg(pItem->getName());
			listRet.push_back(pData);
			if(listRet.size()>20)
				return;
		}
	}

	return CBaseWidget::getKeyWizData(keyword,listRet);
}

void CKLineWidget::keyWizEntered( KeyWizData* pData )
{
	if(pData->cmd == CKeyWizard::CmdStock)
	{
		setStockItem(reinterpret_cast<CStockInfoItem*>(pData->arg));
		return;
	}

	return CCoordXBaseWidget::keyWizEntered(pData);
}

void CKLineWidget::drawCoordX( QPainter& p,const QRectF& rtCoordX,float fItemWidth )
{
	if(m_typeCircle == FenShi)
		return;

	updateShowTimes(rtCoordX,fItemWidth);

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
	if(m_typeCircle==FenShi)
	{

	}
	else if(m_typeCircle<Day)
	{
		time_t tmCurHalfHour = listTimes[iCount]/1800;

		p.setPen(QColor(155,0,0));
		QPen oldPen = p.pen();
		QPen newPen = QPen(Qt::DotLine);
		newPen.setColor(QColor(155,0,0));

		while(fCurX>fEndX && iCount>=0)
		{
			time_t tmTime = listTimes[iCount];
			if((tmCurHalfHour - tmTime/1800)>24)
			{
				//���һ��ʱ��
				p.setPen(oldPen);
				p.setPen(QColor(0,155,155));

				if((fLastX - fCurX) < 40)
				{
					//����֮ǰ���Ƶ�ʱ��
					p.fillRect(fLastX-14,rtCoordX.top(),30,rtCoordX.height(),QColor(0,0,0));
				}

				p.drawLine(fCurX+fItemWidth,m_rtClient.top(),fCurX+fItemWidth,rtCoordX.top()+2);
				p.drawText(fCurX+fItemWidth-14,rtCoordX.top()+2,30,rtCoordX.height()-2,
					Qt::AlignCenter,QDateTime::fromTime_t(tmCurHalfHour*1800).toString("MM/dd"));

				fLastX = fCurX+fItemWidth;
				tmCurHalfHour = tmTime/1800;
			}
			else if(tmTime/1800 < tmCurHalfHour)
			{
				if((fLastX - fCurX)>40)
				{
					if(tmCurHalfHour%2==0)
						p.setPen(oldPen);
					else
						p.setPen(newPen);

					p.drawLine(fCurX+fItemWidth,m_rtClient.top(),fCurX+fItemWidth,rtCoordX.top()+2);

					p.setPen( tmCurHalfHour%2 ? QColor(0,255,255) : QColor(255,0,0));
					p.drawText(fCurX+fItemWidth-14,rtCoordX.top()+2,30,rtCoordX.height()-2,
						Qt::AlignCenter,QDateTime::fromTime_t(tmCurHalfHour*1800).toString("hh:mm"));

					fLastX = fCurX+fItemWidth;
				}
				tmCurHalfHour = tmTime/1800;
			}

			//if(tmTime<tmCurDate)
			//{
			//	p.setPen(QColor(255,255,255));
			//	p.fillRect(fLastX-14,rtCoordX.top(),30,rtCoordX.height(),QColor(0,0,0));
			//	p.drawLine(fCurX+fItemWidth,rtCoordX.top(),fCurX+fItemWidth,rtCoordX.top()+2);
			//	p.drawText(fCurX+fItemWidth-14,rtCoordX.top()+2,30,rtCoordX.height()-2,
			//		Qt::AlignCenter,QDateTime::fromTime_t(tmCurDate).toString("MM/dd"));

			//	tmCurDate = QDateTime(QDateTime::fromTime_t(tmTime).date()).toTime_t();
			//	fLastX = fCurX;
			//	++iTimeCount;
			//}
			//else
			//{
			//	if(m_typeCircle<Min1)
			//	{
			//		if((fLastX-fCurX)>30)
			//		{
			//			if(iTimeCount%2)
			//			{
			//				p.setPen(QColor(255,0,0));
			//				p.drawLine(fCurX,rtCoordX.top(),fCurX,rtCoordX.top()+2);
			//				p.drawText(fCurX-14,rtCoordX.top()+2,30,rtCoordX.height()-2,
			//					Qt::AlignCenter,QDateTime::fromTime_t(tmTime).toString("hh:mm"));
			//			}
			//			else
			//			{
			//				p.setPen(QColor(0,255,255));
			//				p.drawLine(fCurX,rtCoordX.top(),fCurX,rtCoordX.top()+2);
			//				p.drawText(fCurX-24,rtCoordX.top()+2,50,rtCoordX.height()-2,
			//					Qt::AlignCenter,QDateTime::fromTime_t(tmTime).toString("mm:ss"));
			//			}

			//			fLastX = fCurX;
			//			++iTimeCount;
			//		}
			//	}
			//	else
			//	{
			//		if((fLastX-fCurX)>30)
			//		{
			//			p.setPen( iTimeCount%2 ? QColor(255,0,0) : QColor(0,255,255));
			//			p.drawLine(fCurX,rtCoordX.top(),fCurX,rtCoordX.top()+2);
			//			p.drawText(fCurX-14,rtCoordX.top()+2,30,rtCoordX.height()-2,
			//				Qt::AlignCenter,QDateTime::fromTime_t(tmTime).toString("hh:mm"));

			//			fLastX = fCurX;
			//			++iTimeCount;
			//		}
			//	}
			//}

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

void CKLineWidget::drawCoordY( QPainter& p,const QRectF rtCoordY, float fMax, float fMin, bool bPercent /*= false*/ )
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


	float fLastClose = m_pStockItem->getLastClose();
	float fLastCloseY = -99999;
	if(bPercent)
	{
		fLastCloseY = (fBottom-(fLastClose-fMin)*(fRealPerUi));
		p.drawText(fX+7,fLastCloseY+4,QString("0.00%"));
		QPen wPen = p.pen();
		wPen.setWidth(2);
		p.setPen(wPen);
		p.drawLine(fX,fLastCloseY,m_rtClient.left(),fLastCloseY);
	}


	QPen oldPen = p.pen();
	QPen newPen = QPen(Qt::DotLine);
	newPen.setColor(QColor(50,50,50));

	while(iValue<iValueMax)
	{
		float fValue = iValue/(1000.0);
		float fY = (fBottom - (fValue - fMin)*(fRealPerUi));
		if(abs(fY-fLastCloseY)<30)
		{
			fLastY = fLastCloseY;
		}

		if(abs(fY-fLastY) > 30)
		{
			p.setPen(oldPen);
			if(iDrawIndex%2)
				p.setPen(QColor(255,0,0));
			else
				p.setPen(QColor(0,255,255));

			p.drawLine(fX,fY,fX+4,fY);

			int iDot = 3 - int(log10(double(iValueIncre))+0.1);
			if(iDot<0)
				iDot = 0;
			if(bPercent)
			{
				float fPercent = (fValue-fLastClose)/fLastClose*100;
				p.drawText(fX+7,fY+4,QString("%1%").arg(fPercent,0,'f',2));
			}
			else
			{
				if(fValue>10000.0)
				{
					p.drawText(fX+7,fY+4,QString("%1").arg(fValue,0,'g',iDot));
				}
				else
				{
					p.drawText(fX+7,fY+4,QString("%1").arg(fValue,0,'f',iDot));
				}
			}

			p.setPen(newPen);
			p.drawLine(fX,fY,m_rtClient.left(),fY);

			fLastY = fY;
			++iDrawIndex;
		}
		iValue = iValue+iValueIncre;
	}
}

void CKLineWidget::drawFenShi( QPainter& p, QRect rtClient )
{
	/*��ͷ��*/
	QRect rtTitle = QRect(rtClient.left(),rtClient.top(),rtClient.width(),m_iTitleHeight);
	drawTitle(p,rtTitle);

	QRectF rtCoordX = QRectF(rtClient.left()+3,rtClient.bottom()-m_iCoorXHeight+1,rtClient.width()-m_iCoorYWidth-5,m_iCoorXHeight);
	float fItemWidth = float(rtCoordX.width()-1)/(m_mapTimes.size()-1);

	//�ı�Ϊ�ɻ�ͼ����Ĵ�С
	rtClient.adjust(3,m_iTitleHeight,-m_iCoorYWidth-2,-m_iCoorXHeight);	

	float fBeginX = rtCoordX.right();
	float fEndX = rtCoordX.left();
	float fCBWidth = fBeginX-fEndX;
	if(fCBWidth<0 || m_mapTimes.size()<1)
		return;
	{
		//����������ƺ�����
		float fCurX = fBeginX;
		float fLastX = fBeginX;

		p.setPen(QColor(155,0,0));
		QPen oldPen = p.pen();
		QPen newPen = QPen(Qt::DotLine);
		newPen.setColor(QColor(155,0,0));

		QMap<time_t,int>::iterator iter = m_mapTimes.end();
		--iter;
		time_t tmCurHalfHour = iter.key()/1800;

		while(iter!=m_mapTimes.begin())
		{
			time_t tmTime = iter.key();
			if((tmCurHalfHour - tmTime/1800)>24)
			{
				//���һ��ʱ��
				p.setPen(oldPen);
				p.setPen(QColor(0,155,155));

				if((fLastX - fCurX) < 40)
				{
					//����֮ǰ���Ƶ�ʱ��
					p.fillRect(fLastX-14,rtCoordX.top(),30,rtCoordX.height(),QColor(0,0,0));
				}

				p.drawLine(fCurX,rtClient.top(),fCurX,rtCoordX.top()+2);
				p.drawText(fCurX-14,rtCoordX.top()+2,30,rtCoordX.height()-2,
					Qt::AlignCenter,QDateTime::fromTime_t(tmCurHalfHour*1800).toString("MM/dd"));

				fLastX = fCurX;
				tmCurHalfHour = tmTime/1800;
			}
			else if(tmTime/1800 < tmCurHalfHour)
			{
				if((fLastX - fCurX)>40)
				{
					QColor clrText(0,255,255);
					if(((((tmTime%(3600*24))/1800)-6)>0 ? ((tmCurHalfHour%2)==0) : (tmCurHalfHour%2==1)))
					{
						p.setPen(oldPen);
						clrText = QColor(255,0,0);
					}
					else
						p.setPen(newPen);

					p.drawLine(fCurX,rtClient.top(),fCurX,rtCoordX.top()+2);

					p.setPen(clrText);
					p.drawText(fCurX-14,rtCoordX.top()+2,30,rtCoordX.height()-2,
						Qt::AlignCenter,QDateTime::fromTime_t(tmCurHalfHour*1800).toString("hh:mm"));

					fLastX = fCurX;
				}
				tmCurHalfHour = tmTime/1800;
			}
			fCurX = fCurX- fItemWidth;
			--iter;
		}
	}
	{
		//����Y������
		//����ģʽ�����ֵ/��Сֵ  ���ٷֱ�  (lastclose*110+1.0)/100
	}
	{
		//������ͼ
		QRect rtMain,rtVolume,rtAsis;
		if(m_iFenShiAsis != FenShiVol)
		{
			int iHMain = rtClient.height()*0.6;
			int iHVol = rtClient.height()*0.2;
			rtMain = QRect(rtClient.left(),rtClient.top(),rtClient.width(),iHMain);
			rtVolume = QRect(rtClient.left(),rtClient.top()+iHMain,rtClient.width(),iHVol);
			rtAsis = QRect(rtClient.left(),rtClient.top()+iHMain+iHVol,rtClient.width(),rtClient.height()-iHMain-iHVol);
		}
		else
		{
			int iHMain = rtClient.height()*0.7;
			rtMain = QRect(rtClient.left(),rtClient.top(),rtClient.width(),iHMain);
			rtVolume = QRect(rtClient.left(),rtClient.top()+iHMain,rtClient.width(),rtClient.height()-iHMain);
			rtAsis = QRect();
		}

		float fMinPrice = 9999;
		float fMaxPrice = -9999;
		float fMinVolume = 9999;
		float fMaxVolume = -9999;

		{
			//�����ֵ����Сֵ�����ڻ���Y������
			QMap<time_t,RStockData*>::iterator iterBegin = m_mapData->lowerBound(m_mapTimes.begin().key());
			QMap<time_t,RStockData*>::iterator iterEnd = m_mapData->end();
			QMap<time_t,RStockData*>::iterator iterTmp = iterBegin;
			while(iterTmp!=m_mapData->end())
			{
				RStockData* pData = (*iterTmp);
				if(pData->fClose>fMaxPrice)
					fMaxPrice = pData->fClose;
				if(pData->fClose<fMinPrice)
					fMinPrice = pData->fClose;

				if(pData->fVolume>fMaxVolume)
					fMaxVolume = pData->fVolume;
				if(pData->fVolume<fMinVolume)
					fMinVolume = pData->fVolume;
				++iterTmp;
			}

			if(fMaxPrice<fMinPrice || fMaxVolume<fMinVolume || fMinVolume<0)
				return;
			float fLastClose = m_pStockItem->getLastClose();
			if(!m_bYCoordAuto)
			{
				float fMaxPrice1 = int(fLastClose*110+0.999)/100.0;
				float fMinPrice1 = int(fLastClose*90)/100.0;
				if(fMaxPrice1>fMaxPrice)
					fMaxPrice = fMaxPrice1;
				if(fMinPrice1<fMinPrice)
					fMinPrice = fMinPrice1;
			}
			else
			{
				fMaxPrice+=(float)0.01;
				fMinPrice-=(float)0.01;
			}
			fMaxVolume = fMaxVolume*1.1;
			fMinVolume = fMinVolume*0.9;
		}

		float fRight = rtMain.right();
		float fTop = rtMain.top();
		float fPer = rtMain.height()/(fMaxPrice-fMinPrice);
		float fBottomVol = rtVolume.bottom();
		float fPerVol = rtVolume.height()/(fMaxVolume-fMinVolume);

		{
			//����Y������
			drawCoordY(p,QRectF(rtMain.right(),rtMain.top(),m_iCoorYWidth,rtMain.height()),fMaxPrice,fMinPrice,m_bYCoordPercent);
			drawCoordY(p,QRectF(rtVolume.right(),rtVolume.top(),m_iCoorYWidth,rtVolume.height()),fMaxVolume,fMinVolume);
		}

		p.setPen(QColor(160,0,0));
		p.drawRect(rtMain);
		p.drawRect(rtVolume);

		time_t tmDay = (CDataEngine::getCurrentTime()/TIME_OF_DAY);
		for (int i=0;i<m_iFenShiCount;++i)
		{
			time_t tmBegin = ((tmDay-i)*TIME_OF_DAY);	//��ʼʱ��
			time_t tmEnd = ((tmDay-i+1)*TIME_OF_DAY);	//����ʱ��

			float fLastV = -9999;
			float fLastVA = -9999;	//����λ��
			float fLastH = -9999;
			p.setPen(QColor(250,250,250));

			QMap<time_t,RStockData*>::iterator iterBegin = m_mapData->upperBound(tmBegin);
			QMap<time_t,RStockData*>::iterator iterEnd = m_mapData->upperBound(tmEnd);
			QMap<time_t,RStockData*>::iterator iterTmp = iterBegin;
			if(!m_pStockItem->isIndex())
			{
				//��ָ���Ļ��Ʒ�ʽ
				double dVolume = 0.0;
				double dAmount = 0.0;
				while(iterTmp!=iterEnd)
				{
					RStockData* pData = (*iterTmp);
					QMap<time_t,int>::iterator iterT = m_mapTimes.upperBound(pData->tmTime);
					if(iterT!=m_mapTimes.end())
					{
						int iIndex = (*iterT)-1;
						float fV = (fMaxPrice - pData->fClose)*fPer+fTop;
						float fH = fRight - fItemWidth*iIndex;
						dVolume += pData->fVolume;
						dAmount += pData->fAmount;
						float fVA = (dVolume>0.0001) ? ((fMaxPrice - (dAmount/(dVolume*100)))*fPer+fTop) : -999;
						if(fLastV>0.0)
						{
							p.setPen(QColor(250,250,250));
							p.drawLine(fH,fV,fLastH,fLastV);
							p.setPen(QColor(192,192,0));
							p.drawLine(fH,fVA,fLastH,fLastVA);
						}
						fLastV = fV;
						fLastH = fH;
						fLastVA = fVA;

						float fVol = fBottomVol - (pData->fVolume-fMinVolume)*fPerVol;
						p.setPen(QColor(192,192,0));
						p.drawLine(fH,fBottomVol,fH,fVol);
					}
					++iterTmp;
				}
			}
			else
			{
				//ָ���Ļ��Ʒ�ʽ
				int iAVCount = 0;
				float fAVPrice = 0.0;
				while(iterTmp!=iterEnd)
				{
					RStockData* pData = (*iterTmp);
					QMap<time_t,int>::iterator iterT = m_mapTimes.upperBound(pData->tmTime);
					if(iterT!=m_mapTimes.end())
					{
						++iAVCount;
						fAVPrice += pData->fClose;

						int iIndex = (*iterT)-1;
						float fV = (fMaxPrice - pData->fClose)*fPer+fTop;
						float fH = fRight - fItemWidth*iIndex;
						float fVA = ((fMaxPrice - (fAVPrice/iAVCount))*fPer+fTop);
						if(fLastV>0.0)
						{
							p.setPen(QColor(250,250,250));
							p.drawLine(fH,fV,fLastH,fLastV);
							p.setPen(QColor(192,192,0));
							p.drawLine(fH,fVA,fLastH,fLastVA);
						}
						fLastV = fV;
						fLastH = fH;
						fLastVA = fVA;

						float fVol = fBottomVol - (pData->fVolume-fMinVolume)*fPerVol;
						p.setPen(QColor(192,192,0));
						p.drawLine(fH,fBottomVol,fH,fVol);
					}
					++iterTmp;
				}
			}
		}

	}
}

void CKLineWidget::onSetYCoordAuto()
{
	m_bYCoordAuto = !m_bYCoordAuto;
	updateData();
}

void CKLineWidget::onSetYCoordPercent()
{
	m_bYCoordPercent = !m_bYCoordPercent;
	updateData();
}
