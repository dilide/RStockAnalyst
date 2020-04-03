#include "StdAfx.h"
#include "BColorBlockWidget.h"
#include "DataEngine.h"
#include "ColorManager.h"
#include "KeyWizard.h"
#include "MainWindow.h"

CBColorBlockWidget::CBColorBlockWidget(CBaseWidget* parent /*= 0*/ )
	: CBaseBlockWidget(parent,WidgetBColorBlock)
	, m_iTitleHeight(16)
	, m_iBottomHeight(16)
	, showStockIndex(0)
	, m_pSelectedBlock(0)
	, m_pCurBlock(0)
	, m_iRightLen(2)
	, m_iLeftLen(0)
{
	connect(&m_timerUpdateUI,SIGNAL(timeout()),this,SLOT(updateUI()));
	m_timerUpdateUI.start(3000);

	m_pActRemoveBlock = m_pMenuCustom->addAction(tr("ɾ��ѡ�а��"),this,SLOT(onRemoveBlock()));
	m_pMenuCustom->addAction(tr("����ѡ�а������"),this,SLOT(onRecalcBlock()));
}


CBColorBlockWidget::~CBColorBlockWidget(void)
{
	clearTmpData();
}

bool CBColorBlockWidget::loadPanelInfo( const QDomElement& eleWidget )
{
	if(!CBaseBlockWidget::loadPanelInfo(eleWidget))
		return false;

	//��ǰ�İ������
	QDomElement eleBlock = eleWidget.firstChildElement("block");
	if(eleBlock.isElement())
	{
		setBlock(eleBlock.text());
	}

	updateUI();
	return true;
}

bool CBColorBlockWidget::savePanelInfo( QDomDocument& doc,QDomElement& eleWidget )
{
	if(!CBaseBlockWidget::savePanelInfo(doc,eleWidget))
		return false;

	//��ǰ�İ������
	if(m_pCurBlock)
	{
		QDomElement eleBlock = doc.createElement("block");
		eleBlock.appendChild(doc.createTextNode(m_pCurBlock->getOnly()));
		eleWidget.appendChild(eleBlock);
	}

	return true;
}

void CBColorBlockWidget::clearTmpData()
{
	m_listBlocks.clear();
	m_mapBlockIndex.clear();
	m_pSelectedBlock = 0;
	m_pCurBlock = 0;
}

void CBColorBlockWidget::updateSortMode( bool bSelFirst )
{
	if(bSelFirst)
	{
		m_pSelectedBlock = 0;
		showStockIndex = 0;
	}

	if(m_pCurBlock)
	{
		m_listBlocks.clear();
		QList<CBlockInfoItem*> list = m_pCurBlock->getBlockList();
		foreach (CBlockInfoItem* pItem,list)
		{
			if(isMatchAbnomal(pItem))
				m_listBlocks.append(pItem);
		}
	}

	//������������
	if(m_sort<=SortByCode)
	{
		QMultiMap<QString,CBlockInfoItem*> mapSort;
		foreach(CBlockInfoItem* pItem,m_listBlocks)
		{
			if(m_sort == SortByCode)
				mapSort.insert(pItem->getOnly(),pItem);
		}
		if(m_sortOrder==Qt::AscendingOrder)
			m_listBlocks = mapSort.values();
		else
		{
			QList<CBlockInfoItem*> list;
			QMultiMap<QString,CBlockInfoItem*>::iterator iter = mapSort.begin();
			while(iter!=mapSort.end())
			{
				list.push_front(iter.value());
				++iter;
			}
			m_listBlocks = list;
		}
	}
	else
	{
		QMultiMap<float,CBlockInfoItem*> mapSort;
		foreach(CBlockInfoItem* pItem,m_listBlocks)
		{
			float v = 0.0;
			if(m_sort == SortByIncrease)
				v = _isnan(pItem->getIncrease()) ? 0.0 : pItem->getIncrease();
			else if(m_sort == SortByTurnRatio)
				v = _isnan(pItem->getTurnRatio()) ? 0.0 : pItem->getTurnRatio();
			else if(m_sort == SortByVolumeRatio)
				v = _isnan(pItem->getVolumeRatio()) ? 0.0 : pItem->getVolumeRatio();
			else
				v = _isnan(pItem->getIncrease()) ? 0.0 : pItem->getIncrease();

			mapSort.insert(v,pItem);
		}
		if(m_sortOrder==Qt::AscendingOrder)
			m_listBlocks = mapSort.values();
		else
		{
			QList<CBlockInfoItem*> list;
			QMultiMap<float,CBlockInfoItem*>::iterator iter = mapSort.begin();
			while(iter!=mapSort.end())
			{
				list.push_front(iter.value());
				++iter;
			}
			m_listBlocks = list;
		}
	}

	m_mapBlockIndex.clear();
	for(int i=0;i<m_listBlocks.size();++i)
	{
		CBlockInfoItem* pItem = m_listBlocks[i];
		m_mapBlockIndex[pItem] = i;
		//�������»���(Ŀǰ���ö�ʱˢ�£�δʹ�øø��½ӿ�)
	}

	if(m_pSelectedBlock == 0 && m_listBlocks.size()>0)
	{
		clickedBlock(m_listBlocks.first());
	}

	update();
	return;
}

void CBColorBlockWidget::getKeyWizData( const QString& keyword,QList<KeyWizData*>& listRet )
{
	foreach(CBlockInfoItem* pItem,m_listBlocks)
	{
		if(pItem->isMatch(keyword))
		{
			KeyWizData* pData = new KeyWizData;
			pData->cmd = CKeyWizard::CmdBlock;
			pData->arg = pItem;
			pData->desc = QString("%1 %2").arg(pItem->getCode()).arg(pItem->getName());
			listRet.push_back(pData);
			if(listRet.size()>20)
				return;
		}
	}

	return CBaseBlockWidget::getKeyWizData(keyword,listRet);
}

void CBColorBlockWidget::keyWizEntered( KeyWizData* pData )
{
	if(pData->cmd == CKeyWizard::CmdBlock)
	{
		clickedBlock(reinterpret_cast<CBlockInfoItem*>(pData->arg));
		return;
	}

	return CBaseBlockWidget::keyWizEntered(pData);
}

void CBColorBlockWidget::setBlock( const QString& block )
{
	CBlockInfoItem* pBlock = CDataEngine::getDataEngine()->getStockBlock(block);
	if(pBlock)
	{
		QList<CBlockInfoItem*> list = pBlock->getBlockList();
		if(list.size()>0)
		{
			clearTmpData();
			for (int i=0;i<list.size();++i)
			{
				CBlockInfoItem* pItem = list[i];
				m_mapBlockIndex[pItem] = i;
			}

			m_pCurBlock = pBlock;
			updateUI();
		}
		else
		{
			m_listBlocks.contains(pBlock);
			clickedBlock(pBlock);
		}
	}
}

void CBColorBlockWidget::updateUI()
{
	updateTimesH();
	updateSortMode(false);
}


void CBColorBlockWidget::clickedBlock( CBlockInfoItem* pItem )
{
	if(pItem == m_pSelectedBlock)
		return;
	int iShowCount = m_rtClient.height()/m_iCBHeight;
	int iRow = m_mapBlockIndex[pItem];
	if((iRow<showStockIndex)||(iRow>showStockIndex+iShowCount))
	{
		showStockIndex = iRow;
		update(m_rtClient);
	}

	CBlockInfoItem* pPreSelectedBlock = m_pSelectedBlock;
	m_pSelectedBlock = pItem;
	update(rectOfBlock(pPreSelectedBlock));
	update(rectOfBlock(m_pSelectedBlock));
	if(m_pSelectedBlock)
	{
		CMainWindow::getMainWindow()->clickedBlock(m_pSelectedBlock->getOnly());
	}
}

void CBColorBlockWidget::paintEvent( QPaintEvent* /*e*/ )
{
	QPainter p(this);
	QRect rtClient = this->rect();
	if(m_bClearMode)
	{
		m_rtClient = rtClient;
		m_rtBottom = QRect(rtClient.left(),rtClient.bottom()-m_iBottomHeight,rtClient.width(),m_iBottomHeight);

		updateShowTimes(QRect(m_rtBottom.left()+m_iLeftLen,m_rtBottom.top(),
			m_rtBottom.width()-m_iRightLen-m_iLeftLen,m_rtBottom.height()),m_iCBWidth);

		drawClient(p,m_rtClient);
	}
	else
	{
		m_rtHeader = QRect(rtClient.left(),rtClient.top(),rtClient.width(),m_iTitleHeight);
		m_rtClient = QRect(rtClient.left(),rtClient.top()+m_iTitleHeight,rtClient.width(),rtClient.height()-m_iTitleHeight-m_iBottomHeight);
		m_rtBottom = QRect(rtClient.left(),rtClient.bottom()-m_iBottomHeight,rtClient.width(),m_iBottomHeight);

		updateShowTimes(QRect(m_rtBottom.left()+m_iLeftLen,m_rtBottom.top(),
			m_rtBottom.width()-m_iRightLen-m_iLeftLen,m_rtBottom.height()),m_iCBWidth);

		drawHeader(p,m_rtHeader);
		drawClient(p,m_rtClient);
		drawBottom(p,m_rtBottom);
	}
}

void CBColorBlockWidget::mouseMoveEvent( QMouseEvent* e )
{
	if(!((qApp->mouseButtons())&Qt::LeftButton))
	{
		update();
		QToolTip::hideText();
		return CBaseBlockWidget::mouseMoveEvent(e);
	}

	CBlockInfoItem* pBlock = hitTestBlock(e->pos());
	qRcvFenBiData* item = hitTestCBItem(e->pos());
	if(item == NULL || pBlock==0)
	{
		QToolTip::hideText();
		return CBaseBlockWidget::mouseMoveEvent(e);
	}

	QString qsTooltip;		//Tips
	QString qsTime = QDateTime::fromTime_t(item->tmTime).toString("hh:mm:ss");

	qsTooltip = QString("�������:%1\r\nʱ��:%2\r\n��ǰ��:%3\r\n�ɽ���:%4\r\n�ɽ���:%5\r\n�ǵ���:")
		.arg(pBlock->getName()).arg(qsTime).arg(item->fPrice).arg(item->fVolume).arg(item->fAmount);

	float fTotal = 0.0;
	for(int i=0;i<10;++i)
	{
		fTotal+=item->fBuyPrice[i];
		qsTooltip += QString("(%1 %2)-").arg(10-i).arg((int)(item->fBuyPrice[i]+0.5));
	}

	for (int i=10;i<20;++i)
	{
		fTotal+=item->fBuyPrice[i];
		qsTooltip += QString("(%1 %2)-").arg(9-i).arg((int)(item->fBuyPrice[i]+0.5));
	}

	{
		qsTooltip += QString("(0 %1)").arg(pBlock->getStockCount()-(int)(fTotal+0.5));
	}

	QToolTip::showText(e->globalPos(),qsTooltip,this);
	return CBaseBlockWidget::mouseMoveEvent(e);
}

void CBColorBlockWidget::mousePressEvent( QMouseEvent* e )
{
	e->accept();
	QPoint ptCur = e->pos();
	if(m_rtHeader.contains(ptCur))
	{
		m_sortOrder = (m_sortOrder==Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
		updateSortMode(true);
	}
	else if(m_rtClient.contains(ptCur))
	{
		int iCurIndex = showStockIndex+(ptCur.y()-m_rtClient.top())/m_iCBHeight;
		if(iCurIndex>=0&&iCurIndex<m_listBlocks.size())
		{
			clickedBlock(m_listBlocks[iCurIndex]);
		}
	}
	else if(m_rtBottom.contains(ptCur))
	{
		QList<QPair<CBlockInfoItem*,QRect>>::iterator iter = m_listBlockBtns.begin();
		while(iter!=m_listBlockBtns.end())
		{
			if(iter->second.contains(ptCur))
			{
				setBlock(iter->first->getOnly());
				break;
			}
			++iter;
		}
	}

	return CBaseBlockWidget::mousePressEvent(e);
}

void CBColorBlockWidget::wheelEvent( QWheelEvent* e )
{
	int numDegrees = e->delta() / 8;
	int numSteps = numDegrees / 15;
	int iIndex = showStockIndex-numSteps*5;
	if(iIndex<0) {iIndex = 0;}
	if(iIndex>=0&&iIndex<m_listBlocks.size())
	{
		e->accept();
		showStockIndex = iIndex;
		updateUI();
	}
	return CBaseWidget::wheelEvent(e);
}

void CBColorBlockWidget::keyPressEvent( QKeyEvent* e )
{
	int iKey = e->key();
	if(Qt::Key_Left == iKey)
	{
		e->accept();
		return;
	}
	else if(Qt::Key_Right == iKey)
	{
		e->accept();
		return;
	}
	else if(Qt::Key_Down == iKey)
	{
		int iCurIndex = m_mapBlockIndex[m_pSelectedBlock];
		if(m_listBlocks.size()>(iCurIndex+1))
		{
			CBlockInfoItem* pItem = m_listBlocks[iCurIndex+1];
			int iRow = m_mapBlockIndex[pItem];
			int iShowCount = m_rtClient.height()/m_iCBHeight;
			if(iShowCount<1)
				return;
			if((iRow-showStockIndex)>=iShowCount)
			{
				showStockIndex = iRow-iShowCount+1;
				updateUI();
			}
			clickedBlock(m_listBlocks[iCurIndex+1]);
		}
		e->accept();
		return;
	}
	else if(Qt::Key_Up == iKey)
	{
		int iCurIndex = m_mapBlockIndex[m_pSelectedBlock];
		if(iCurIndex>0)
		{
			CBlockInfoItem* pItem = m_listBlocks[iCurIndex-1];
			int iRow = m_mapBlockIndex[pItem];
			if(iRow<showStockIndex)
			{
				showStockIndex = iRow;
				updateUI();
			}
			clickedBlock(pItem);
		}
		e->accept();
		return;
	}
	else if(Qt::Key_PageDown == iKey)
	{
		int iShowCount = m_rtClient.height()/m_iCBHeight;
		if(iShowCount<1)
			return;
		if((showStockIndex+iShowCount)<m_listBlocks.size())
		{
			showStockIndex = showStockIndex+iShowCount;
			updateUI();
		}
		e->accept();
		return;
	}
	else if(Qt::Key_PageUp == iKey)
	{
		int iShowCount = m_rtClient.height()/m_iCBHeight;
		if(iShowCount<1)
			return;
		showStockIndex = (showStockIndex-iShowCount)>0 ? (showStockIndex-iShowCount) : 0;
		updateUI();
		e->accept();
		return;
	}
	else if(Qt::Key_F10 == iKey)
	{
		//F10����
		if(m_pSelectedBlock)
		{
			if(!CDataEngine::getDataEngine()->showF10Data(m_pSelectedBlock->getCode()))
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

	return CBaseBlockWidget::keyPressEvent(e);
}

QMenu* CBColorBlockWidget::getCustomMenu()
{
	CBaseBlockWidget::getCustomMenu();
	m_pActRemoveBlock->setEnabled(m_pCurBlock == CDataEngine::getDataEngine()->getCustomBlock());
	return m_pMenuCustom;
}

void CBColorBlockWidget::drawHeader( QPainter& p,const QRect& rtHeader )
{
	p.fillRect(rtHeader,QColor(0,0,0));
	p.setPen(QColor(255,0,0));
	QRect rtCoord = rtHeader.adjusted(0,0,-1,-1);
	p.drawRect(rtCoord);

	p.setPen(QColor(255,255,255));
	if(!m_pCurBlock)
		return;

	QString qsText = m_pCurBlock->getBlockName();
	switch(m_sort)
	{
	case SortByCode:		//����Ʊ��������
		qsText += " (����)";
		break;
	case SortByIncrease:		//���Ƿ�����
		qsText += " (�Ƿ�)";
		break;
	case SortByTurnRatio:	//������������
		qsText += " (������)";
		break;
	case SortByVolumeRatio:	//����������
		qsText += " (����)";
		break;
	default:
		qsText += " (δ����)";
		break;
	}

	if(m_sortOrder == Qt::AscendingOrder)
	{
		p.drawText(rtHeader,Qt::AlignLeft|Qt::AlignVCenter,QString("��%1").arg(qsText));
	}
	else
	{
		p.drawText(rtHeader,Qt::AlignLeft|Qt::AlignVCenter,QString("��%1").arg(qsText));
	}
}

void CBColorBlockWidget::drawClient( QPainter& p,const QRect& rtClient )
{
	CColorItem* pClrItem = CColorManager::getColorItem(m_qsColorMode);

	p.fillRect(rtClient,QColor(0,0,0));

	int iCurY = rtClient.top();
	int iIndex = showStockIndex;
	for(;iCurY<rtClient.bottom();iCurY=(iCurY+m_iCBHeight))
	{
		if(iIndex<0||iIndex>=m_listBlocks.size())
			break;

		drawBlock(p,QRect(rtClient.left(),iCurY,rtClient.width(),m_iCBHeight),m_listBlocks[iIndex],pClrItem);
		++iIndex;
	}
}

void CBColorBlockWidget::drawBottom( QPainter& p,const QRect& rtBottom )
{
	p.fillRect(rtBottom,QColor(0,0,0));

	/*
	QRectF rtColors = QRectF(rtBottom.left(),rtBottom.top(),50,rtBottom.height());
	float fColorsWidth = rtColors.width()-5;
	float fColorWidth = fColorsWidth/COLOR_BLOCK_SIZE;
	for(int i=0;i<COLOR_BLOCK_SIZE;++i)
	{
		p.fillRect(QRectF(rtBottom.left()+i*fColorWidth,rtBottom.top(),fColorWidth,rtBottom.height()),
			QColor::fromRgb(m_clrTable[i]));
	}
	*/
	//����������ƺ�����
	drawCoordX(p,QRect(rtBottom.left()+m_iLeftLen,rtBottom.top(),
		rtBottom.width()-m_iRightLen-m_iLeftLen,rtBottom.height()),m_iCBWidth);

	//���ư�鰴ť
	m_listBlockBtns.clear();
	int iBeginX = rtBottom.right()-m_iLeftLen-m_mapTimes.size()*m_iCBWidth;
	QList<CBlockInfoItem*> listBlocks = CDataEngine::getDataEngine()->getTopLevelBlocks();
	foreach(CBlockInfoItem* pBlock,listBlocks)
	{
		if(iBeginX<rtBottom.left())
			break;
		if(pBlock->getBlockCount()>0)
		{
			QRect rtText = QRect(iBeginX-50,rtBottom.top(),48,rtBottom.height());
			if(m_pCurBlock == pBlock)
			{
				p.setPen(QColor(255,255,255));
				p.fillRect(rtText,QColor(127,0,0));
			}
			else
			{
				p.setPen(QColor(127,0,0));
				p.drawRect(rtText);
			}
			p.drawText(rtText,pBlock->getBlockName(),QTextOption(Qt::AlignCenter));

			m_listBlockBtns.append(QPair<CBlockInfoItem*,QRect>(pBlock,rtText));
			iBeginX-=50;
		}
	}
}

void CBColorBlockWidget::drawBlock( QPainter& p,const QRect& rtCB,CBlockInfoItem* pItem,CColorItem* pClrItem)
{
	if(pItem == m_pSelectedBlock)
	{
		p.fillRect(rtCB,QColor(50,50,50));
	}
/*
	���ٻ����������˵��
	{
		//�������ı�ʶ��Ϣ������������ƣ�
		p.setPen(QColor(255,255,255));
		QString qsText = pItem->getName();
		if(qsText.isEmpty())
			qsText = pItem->getCode();
		if(rtCB.height()>35)
		{
			//�������ɫ��ĸ߶��㹻�Ļ�������ʾ����
			if(m_sort == SortByIncrease)
				qsText += QString("\n%1%").arg(pItem->getIncrease(),0,'f',2);
			else if(m_sort == SortByTurnRatio)
				qsText += QString("\n%1%").arg(pItem->getTurnRatio(),0,'f',2);
			else if(m_sort == SortByVolumeRatio)
				qsText += QString("\n%1%").arg(pItem->getVolumeRatio(),0,'f',2);
		}
		p.drawText(QRect(rtCB.left(),rtCB.top(),RCB_OFFSET_LEFT,m_iCBHeight),Qt::AlignCenter,qsText);
	}
*/
	//����������ƺ�����
	float fBeginX = rtCB.right()-m_iRightLen;
	float fEndX = rtCB.left()+m_iLeftLen;
	float fCBWidth = fBeginX-fEndX;
	if(fCBWidth<0)
		return;

	QList<RStockData*> listDatas = pItem->getToday5MinList();
	if(listDatas.size()>0)
	{
		QList<RStockData*>::iterator iterFenBi = listDatas.end();
		do
		{
			--iterFenBi;
			RBlockData* pFenBi = reinterpret_cast<RBlockData*>(*iterFenBi);

			QMap<time_t,float>::iterator iter = m_mapShowTimes.lowerBound(pFenBi->tmTime);
			if(iter==m_mapShowTimes.end())
			{
				continue;
			}
			else if(iter!=m_mapShowTimes.begin())
			{
				if(iter.value()<rtCB.left())
					break;

				//ɫ��Ĵ�С
				float fTop = rtCB.top();
				float fLeft = iter.value()-m_iCBWidth;
				float fHeight = rtCB.height()-1;

				float fCount[21];
				memcpy(&fCount[0],&(pFenBi->fIncrease[0]),sizeof(float)*21);

				float fTotal = 0;

				for (int i=0;i<21;++i)
				{
					fTotal = fTotal+fCount[i];
				}

				float fPer = fHeight/fTotal;
				float fCurY = fTop;
				if(fPer>=fHeight)
					continue;

				for (int j=0;j<10;++j)
				{
					//10-1
					float fPerH = fCount[j]*fPer;
					p.fillRect(QRectF(fLeft,fCurY,m_iCBWidth,fPerH),pClrItem->getColor(20-j));
					fCurY+=fPerH;
				}
				{
					//0
					float fPerH = (fCount[20])*fPer;
					p.fillRect(QRectF(fLeft,fCurY,m_iCBWidth,fPerH),pClrItem->getColor(10));

					fCurY+=fPerH;
				}

				for (int j=10;j<20;++j)
				{
					float fPerH = fCount[j]*fPer;
					p.fillRect(QRectF(fLeft,fCurY,m_iCBWidth,fPerH),pClrItem->getColor(19-j));

					fCurY+=fPerH;
				}
				/*
				QRectF rtBlock = QRectF(iter.value()-m_iCBWidth,fTop,m_iCBWidth,rtCB.height());

				QString qsText = QString("%1 %2").arg(fTotal).arg(pItem->getStockList().size());
				p.drawText(rtBlock,QDateTime::fromTime_t(pFenBi->tmTime).toString("mm:ss"));*/

			}
			else
			{
				break;
			}
		}
		while(iterFenBi!=listDatas.begin());
	}
	//����
	//drawColocBlock(p,rtCB.top(),_vColor,_vHeight,_vWidth);
}

QRect CBColorBlockWidget::rectOfBlock( CBlockInfoItem* pItem )
{
	if(m_mapBlockIndex.contains(pItem))
	{
		int iRow = m_mapBlockIndex[pItem];
		return QRect(m_rtClient.left(),(m_rtClient.top()+(iRow-showStockIndex)*m_iCBHeight),m_rtClient.width(),m_iCBHeight);
	}

	return QRect();
}

CBlockInfoItem* CBColorBlockWidget::hitTestBlock( const QPoint& ptPoint ) const
{
	int iRow = (ptPoint.y()-m_rtClient.top())/m_iCBHeight + showStockIndex;
	if(iRow<0||iRow>=m_listBlocks.size())
		return 0;

	return m_listBlocks[iRow];
}

qRcvFenBiData* CBColorBlockWidget::hitTestCBItem( const QPoint& ptPoint ) const
{
	CBlockInfoItem* pItem = hitTestBlock(ptPoint);

	qRcvFenBiData* pData = NULL;
/*	if(pItem && mapBlockColorBlocks.contains(pItem))
	{
		QMap<time_t,qRcvFenBiData*>* pMap = mapBlockColorBlocks[pItem];
		int iIndex = (m_rtClient.right() - ptPoint.x())/m_iCBWidth;
		QMap<time_t,int>::iterator iter = m_mapTimes.end();
		while(iter!=m_mapTimes.begin())
		{
			--iter;

			if(iIndex==iter.value())
			{
				QMap<time_t,qRcvFenBiData*>::iterator iterFenBi = pMap->lowerBound(iter.key());
				if(iterFenBi!=pMap->end())
				{
					pData = iterFenBi.value();
					break;
				}
			}
			else if(iIndex<iter.value())
				break;
		}
	}*/
	return pData;
}

void CBColorBlockWidget::onBlockReportUpdate( const QString& qsOnly )
{
	//δʹ��
	CBlockInfoItem* pBlock = CDataEngine::getDataEngine()->getStockBlock(qsOnly);
	update(rectOfBlock(pBlock));
}

void CBColorBlockWidget::onRemoveBlock()
{
	if(m_pSelectedBlock)
	{
		m_pCurBlock->removeBlockInfo(m_pSelectedBlock);
		setBlock(m_pCurBlock->getOnly());
	}
}

void CBColorBlockWidget::onRecalcBlock()
{
	if(m_pSelectedBlock)
	{
		m_pSelectedBlock->recalcMinData();
		updateSortMode(false);
	}
}
