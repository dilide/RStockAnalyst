#include "StdAfx.h"
#include "MarketTrendWidget.h"
#include "DataEngine.h"
#include "MainWindow.h"
#include "KeyWizard.h"

#define	GetColorByFloat(x)	(((x)==0.0) ? QColor(191,191,191) : (((x)>0.0) ? QColor(255,80,80) : QColor(0,255,255)))

#define BLOCK_CMD_SHOW	1
#define BLOCK_CMD_ADD	2

CMarketTrendWidget::CMarketTrendWidget( CBaseWidget* parent /*= 0*/ )
	: CBaseWidget(parent,WidgetMarketTrend)
	, m_iHeaderHeight(20)
	, m_iStockHeight(16)
	, m_iBottomHeight(18)
	, m_rtHeader(0,0,0,0)
	, m_rtClient(0,0,0,0)
	, m_rtBottom(0,0,0,0)
	, showHeaderIndex(0)
	, showStockIndex(0)
	, showBlockIndex(0)
	, m_pSelectedStock(0)
	, m_iSortColumn(0)
	, m_sortOrder(Qt::AscendingOrder)
	, m_pSelectedBlock(0)
	, m_bLoaded(false)
{
	m_listHeader << tr("����") << tr("����") << tr("����")
		<< tr("�Ƿ�") << tr("����") << tr("�����ʣ��ֲ") << tr("ǰ��") << tr("��")
		<< tr("���") << tr("���") << tr("����") << tr("����") << tr("�ܶ�ֲ֣�")
		<< tr("����") << tr("����") << tr("�ǵ�") << tr("���") << tr("����") << tr("��ӯ��")
		<< tr("��ͨ��ֵ") << tr("����") << tr("����") << tr("ί����") << tr("ί����")
		<< tr("ί��") << tr("ί��");

	/*��ʼ��������Ԫ�Ŀ��*/
	m_listItemWidth.push_back(30);
	m_listItemWidth.push_back(50);
	while(m_listItemWidth.size()<m_listHeader.size())
	{
		m_listItemWidth.push_back(80);
	}
	m_listItemWidth[5] = 100;

	m_pMenuCustom = new QMenu(tr("�г�����ͼ�˵�"));


	{
		//��ʼ������block
		QList<CBlockInfoItem*> listBlocks = CDataEngine::getDataEngine()->getTopLevelBlocks();
		if(listBlocks.size()>0)
		{
			foreach(CBlockInfoItem* b,listBlocks)
			{
				m_listBlocks.push_back(QPair<CBlockInfoItem*,QRect>(b,QRect()));
			}
			updateBlockRect();
			if(m_pSelectedBlock == 0)
				clickedBlock(listBlocks.first());
			else
				clickedBlock(m_pSelectedBlock);
		}
	}

//	setMinimumHeight(m_iHeaderHeight+m_iStockHeight+m_iBottomHeight);
//	setMinimumWidth(200);
}

CMarketTrendWidget::~CMarketTrendWidget(void)
{
	m_pSelectedStock = 0;
	clearTmpData();
	delete m_pMenuCustom;
}

bool CMarketTrendWidget::loadPanelInfo( const QDomElement& eleWidget )
{
	if(!CBaseWidget::loadPanelInfo(eleWidget))
		return false;
	
	//��ǰ�İ������
	QDomElement eleBlock = eleWidget.firstChildElement("block");
	if(eleBlock.isElement())
	{
		clickedBlock(CDataEngine::getDataEngine()->getStockBlock(eleBlock.text()));
	}

	//���õ�ǰѡ�еĹ�Ʊ
	QDomElement eleStock = eleWidget.firstChildElement("stock");
	if(eleStock.isElement())
	{
		CStockInfoItem* pItem = CDataEngine::getDataEngine()->getStockInfoItem(eleStock.text());
		if(pItem)
			clickedStock(pItem);
	}

	m_bLoaded = true;
	return true;
}

bool CMarketTrendWidget::savePanelInfo( QDomDocument& doc,QDomElement& eleWidget )
{
	if(!CBaseWidget::savePanelInfo(doc,eleWidget))
		return false;

	//��ǰ�İ������
	if(m_pSelectedBlock)
	{
		QDomElement eleBlock = doc.createElement("block");
		eleBlock.appendChild(doc.createTextNode(m_pSelectedBlock->getOnly()));
		eleWidget.appendChild(eleBlock);
	}

	//��ǰ��Ʊ����
	if(m_pSelectedStock)
	{
		QDomElement eleStock = doc.createElement("stock");
		eleStock.appendChild(doc.createTextNode(m_pSelectedStock->getOnly()));
		eleWidget.appendChild(eleStock);
	}
	
	return true;
}

void CMarketTrendWidget::setStocks( const QList<CAbstractStockItem*>& list )
{
	clearTmpData();
	foreach(CAbstractStockItem* pItem,list)
	{
		if(pItem->getCurrentReport()->tmTime>0)
			m_listStocks.append(pItem);

	}
	if(!list.contains(m_pSelectedStock))
	{
		m_pSelectedStock = 0;
	}
	//showHeaderIndex = 0;
	for(int i=0;i<m_listStocks.size();++i)
	{
		m_mapStockIndex[m_listStocks[i]] = i;
	}
	foreach(CAbstractStockItem* pItem,m_listStocks)
	{
		connect(pItem,SIGNAL(stockItemReportChanged(const QString&)),this,SLOT(stockInfoChanged(const QString&)));
		connect(pItem,SIGNAL(stockItemHistoryChanged(const QString&)),this,SLOT(stockInfoChanged(const QString&)));
	}

	if(m_pSelectedStock==0 && m_listStocks.size()>0)
	{
		showStockIndex = 0;
		clickedStock(m_listStocks.first());
	}
}

void CMarketTrendWidget::stockInfoChanged( const QString& code )
{
	if(m_iSortColumn>2)
	{
		//30��ˢ��һ��
		static QTime tmLast = QTime::currentTime();
		if(tmLast.secsTo(QTime::currentTime())>26)
		{
			resortStocks();
			tmLast = QTime::currentTime();
		}
	}
	CAbstractStockItem* pItem = CDataEngine::getDataEngine()->getStockItem(code);
	update(rectOfStock(pItem));
}

void CMarketTrendWidget::onAddToBlock()
{
	if(!m_pSelectedStock)
		return;
	QAction* pAct = reinterpret_cast<QAction*>(sender());
	if(!pAct)
		return;
	QString block = pAct->data().toString();
	if(block.isEmpty())
		return;
	CBlockInfoItem* pBlock = CDataEngine::getDataEngine()->getStockBlock(block);
	if(pBlock)
	{
		pBlock->appendStocks(QList<CStockInfoItem*>()<<reinterpret_cast<CStockInfoItem*>(m_pSelectedStock));
	}
}

void CMarketTrendWidget::onAddToNewBlock()
{
	if(!m_pSelectedStock)
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
	if(CDataEngine::getDataEngine()->isHadBlock(block))
		return;
	CBlockInfoItem* pBlock = CDataEngine::getDataEngine()->getStockBlock(block);
	if(pBlock)
	{
		pBlock->appendStocks(QList<CStockInfoItem*>()<<reinterpret_cast<CStockInfoItem*>(m_pSelectedStock));
	}
}

void CMarketTrendWidget::clearTmpData()
{
	//disconnect(this,SLOT(stockInfoChanged(const QString&)));
	foreach(CAbstractStockItem* pItem,m_listStocks)
	{
		disconnect(pItem,SIGNAL(stockItemReportChanged(const QString&)),this,SLOT(stockInfoChanged(const QString&)));
		disconnect(pItem,SIGNAL(stockItemHistoryChanged(const QString&)),this,SLOT(stockInfoChanged(const QString&)));
	}
	m_listStocks.clear();
	m_mapStockIndex.clear();
}

void CMarketTrendWidget::clickedHeader( int column )
{
	if(column==0)
	{
		m_iSortColumn = 0;
		return;
	}
	if(column == m_iSortColumn)
	{
		m_sortOrder = (m_sortOrder==Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
	}
	else
	{
		m_iSortColumn = column;
		m_sortOrder = Qt::AscendingOrder;
	}
	resortStocks();
	{
		//���õ�ǰѡ����Ϊ��һ��
		if(m_listStocks.size()>0)
		{
			showStockIndex = 0;
			clickedStock(m_listStocks.first());
		}
	}
}

void CMarketTrendWidget::clickedStock( CAbstractStockItem* pItem )
{
	if(pItem == m_pSelectedStock)
		return;
	int iShowCount = m_rtClient.height()/m_iStockHeight;
	int iRow = m_mapStockIndex[pItem];
	if((iRow<showStockIndex)||(iRow>showStockIndex+iShowCount))
	{
		showStockIndex = iRow;
		update(m_rtClient);
	}

	CAbstractStockItem* pPreSelectedStock = m_pSelectedStock;
	m_pSelectedStock = pItem;
	update(rectOfStock(pPreSelectedStock));
	update(rectOfStock(m_pSelectedStock));
	if(pItem->isInstanceOfBlock())
	{
		CMainWindow::getMainWindow()->clickedBlock(pItem->getOnly());
	}
	else if(pItem->isInstanceOfStock())
	{

		CMainWindow::getMainWindow()->clickedStock(pItem->getOnly());
	}

}

void CMarketTrendWidget::offsetShowHeaderIndex( int offset )
{
	int iIndex = showHeaderIndex+offset;
	if(iIndex>=0&&iIndex<m_listHeader.size()-3)
	{
		showHeaderIndex = iIndex;
		update();
	}
}

void CMarketTrendWidget::clickedBlock( CBlockInfoItem* block )
{
	CBlockInfoItem* pBlock = block;
	if(!pBlock)
		return;
	if(m_pSelectedBlock == pBlock)
	{
		CMainWindow::getMainWindow()->clickedBlock(pBlock->getOnly());
		setStocks(pBlock->getAbsStockList());
		resortStocks();
		update();
		return;
	}

	{
		m_pSelectedStock = 0;
		CMainWindow::getMainWindow()->clickedBlock(pBlock->getOnly());
		setStocks(pBlock->getAbsStockList());
		m_pSelectedBlock = block;
		{
			//��������ʽ
//			m_sortOrder = (m_sortOrder==Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
			clickedHeader(0);
		}
	}
	update();
}

void CMarketTrendWidget::paintEvent( QPaintEvent* )
{
	QPainter p(this);
	drawHeaders(p);
	drawStocks(p);
	drawBottom(p);
}

void CMarketTrendWidget::resizeEvent( QResizeEvent* e)
{
	updateDrawRect();
	return CBaseWidget::resizeEvent(e);
}

void CMarketTrendWidget::mousePressEvent( QMouseEvent* e )
{
	e->accept();
	QPoint ptCur = e->pos();
	if(m_rtHeader.contains(ptCur))
	{
		int iCurX = m_rtHeader.left();
		int iCurIndex = 0;
		for (;iCurIndex<3;++iCurIndex)
		{
			iCurX = iCurX+m_listItemWidth[iCurIndex];
			if(iCurX>ptCur.x())
				break;
		}
		if(iCurX>ptCur.x())
		{
			clickedHeader(iCurIndex);
			return;
		}
		iCurIndex = iCurIndex+showHeaderIndex;
		for (;iCurIndex<m_listItemWidth.size();++iCurIndex)
		{
			iCurX = iCurX+m_listItemWidth[iCurIndex];
			if(iCurX>ptCur.x())
				break;
		}
		if(iCurX>ptCur.x())
		{
			clickedHeader(iCurIndex);
			return;
		}
	}
	else if(m_rtClient.contains(ptCur))
	{
		int iCurIndex = showStockIndex+(ptCur.y()-m_rtClient.top())/m_iStockHeight;
		if(iCurIndex>=0&&iCurIndex<m_listStocks.size())
		{
			clickedStock(m_listStocks[iCurIndex]);
		}
	}
	else if(m_rtBottom.contains(ptCur))
	{
		if(m_rtPreIndex.contains(ptCur))
		{
			offsetShowHeaderIndex(-1);
		}
		else if(m_rtNextIndex.contains(ptCur))
		{
			offsetShowHeaderIndex(1);
		}
		else
		{
			QList<QPair<CBlockInfoItem*,QRect>>::iterator iter = m_listBlocks.begin();
			while(iter!=m_listBlocks.end())
			{
				if(iter->second.contains(ptCur))
				{
					clickedBlock(iter->first);
					break;
				}
				++iter;
			}
		}
	}

	return CBaseWidget::mousePressEvent(e);
}

void CMarketTrendWidget::mouseDoubleClickEvent( QMouseEvent * e )
{
	e->accept();
	QPoint ptCur = e->pos();
	if(m_rtClient.contains(ptCur))
	{
		int iCurIndex = showStockIndex+(ptCur.y()-m_rtClient.top())/m_iStockHeight;
		if(iCurIndex>=0&&iCurIndex<m_listStocks.size())
		{
			CAbstractStockItem* pItem = m_listStocks[iCurIndex];
			if(pItem->isInstanceOfBlock())
				clickedBlock(reinterpret_cast<CBlockInfoItem*>(m_listStocks[iCurIndex]));
		}
	}

	return CBaseWidget::mouseDoubleClickEvent(e);
}

void CMarketTrendWidget::wheelEvent( QWheelEvent* e )
{
	int numDegrees = e->delta() / 8;
	int numSteps = numDegrees / 15;
	int iIndex = showStockIndex-numSteps*5;
	if(iIndex<0) {iIndex = 0;}
	if(iIndex>=0&&iIndex<m_listStocks.size())
	{
		e->accept();
		showStockIndex = iIndex;
		update();
	}
	return CBaseWidget::wheelEvent(e);
}

void CMarketTrendWidget::keyPressEvent( QKeyEvent* e )
{
	int iKey = e->key();
	if(Qt::Key_Left == iKey)
	{
		e->accept();
		return offsetShowHeaderIndex(-1);
	}
	else if(Qt::Key_Right == iKey)
	{
		e->accept();
		return offsetShowHeaderIndex(1);
	}
	else if(Qt::Key_Down == iKey)
	{
		int iCurIndex = m_mapStockIndex[m_pSelectedStock];
		if(m_listStocks.size()>(iCurIndex+1))
		{
			CAbstractStockItem* pItem = m_listStocks[iCurIndex+1];
			int iRow = m_mapStockIndex[pItem];
			int iShowCount = m_rtClient.height()/m_iStockHeight;
			if(iShowCount<1)
				return;
			if((iRow-showStockIndex)>=iShowCount)
			{
				showStockIndex = iRow-iShowCount+1;
				update(m_rtClient);
			}
			clickedStock(m_listStocks[iCurIndex+1]);
		}
		e->accept();
	}
	else if(Qt::Key_Up == iKey)
	{
		int iCurIndex = m_mapStockIndex[m_pSelectedStock];
		if(iCurIndex>0)
		{
			CAbstractStockItem* pItem = m_listStocks[iCurIndex-1];
			int iRow = m_mapStockIndex[pItem];
			if(iRow<showStockIndex)
			{
				showStockIndex = iRow;
				update(m_rtClient);
			}
			clickedStock(pItem);
		}
		e->accept();
	}
	else if(Qt::Key_PageDown == iKey)
	{
		int iShowCount = m_rtClient.height()/m_iStockHeight;
		if(iShowCount<1)
			return;
		if((showStockIndex+iShowCount)<m_listStocks.size())
		{
			showStockIndex = showStockIndex+iShowCount;
			update(m_rtClient);
		}
		e->accept();
	}
	else if(Qt::Key_PageUp == iKey)
	{
		int iShowCount = m_rtClient.height()/m_iStockHeight;
		if(iShowCount<1)
			return;
		showStockIndex = (showStockIndex-iShowCount)>0 ? (showStockIndex-iShowCount) : 0;
		update(m_rtClient);

		e->accept();
	}
	else if(Qt::Key_F10 == iKey)
	{
		//F10����
		if(m_pSelectedStock)
		{
			if(!CDataEngine::getDataEngine()->showF10Data(m_pSelectedStock->getCode()))
			{
				//δ��F10���� do something
			}
		}
	}
	else
	{
		return CBaseWidget::keyPressEvent(e);
	}
}

QMenu* CMarketTrendWidget::getCustomMenu()
{
	update();
	QAction* pAction = m_pMenu->menuAction();
	if(!m_pMenuCustom->actionGeometry(pAction).isValid())
		m_pMenuCustom->addMenu(m_pMenu);

	QMenu* pBlockMenu = CMainWindow::getMainWindow()->getBlockMenu(this,BLOCK_CMD_SHOW);
	pBlockMenu->setTitle(tr("�л����"));
	m_pMenuCustom->addMenu(pBlockMenu);

	return m_pMenuCustom;
}

void CMarketTrendWidget::updateDrawRect()
{
	QRect rtClient = this->rect();
	m_rtHeader = QRect(rtClient.topLeft(),QSize(rtClient.width(),m_iHeaderHeight));
	m_rtBottom = QRect(rtClient.left(),rtClient.bottom()-m_iBottomHeight,rtClient.width(),m_iBottomHeight);
	m_rtClient = QRect(m_rtHeader.left(),m_rtHeader.bottom()+2,rtClient.width(),rtClient.height()-m_iBottomHeight-m_iHeaderHeight-2);

	//����ǰ���λ��
	m_rtNextIndex = QRect(m_rtBottom.right()-m_iBottomHeight,m_rtBottom.top(),m_iBottomHeight,m_iBottomHeight);
	m_rtPreIndex = QRect(m_rtBottom.right()-m_iBottomHeight*2,m_rtBottom.top(),m_iBottomHeight,m_iBottomHeight);
	updateBlockRect();
}

void CMarketTrendWidget::updateBlockRect()
{
	//���������ÿ�
	for(int i=0;i<showBlockIndex;++i)
	{
		if(i<m_listBlocks.size())
			m_listBlocks[i].second = QRect();
	}
	//�����������Ĵ�С
	QFont f;
	QFontMetrics m(f);
	int iCurX = m_rtBottom.left();
	for (int i=showBlockIndex;i<m_listBlocks.size();++i)
	{
		int iWidth = m.width(m_listBlocks[i].first->getBlockName()) + 16;
		QRect rtEntity = QRect(iCurX,m_rtBottom.top(),iWidth,m_rtBottom.height());
		m_listBlocks[i].second = rtEntity;

		iCurX = iCurX+iWidth;
	}
}

void CMarketTrendWidget::resortStocks()
{
	if(m_iSortColumn == 1||m_iSortColumn ==2)
	{
		QMultiMap<QString,CAbstractStockItem*> mapSort;
		foreach(CAbstractStockItem* pItem,m_listStocks)
		{
			mapSort.insert(dataOfDisplay(pItem,m_iSortColumn),pItem);
		}

		if(m_sortOrder==Qt::AscendingOrder)
			setStocks(mapSort.values());
		else
		{
			QList<CAbstractStockItem*> list;
			QMultiMap<QString,CAbstractStockItem*>::iterator iter = mapSort.begin();
			while(iter!=mapSort.end())
			{
				list.push_front(iter.value());
				++iter;
			}
			setStocks(list);
		}
	}
	else
	{
		QMultiMap<float,CAbstractStockItem*> mapSort;
		foreach(CAbstractStockItem* pItem,m_listStocks)
		{
			QString qsData = dataOfDisplay(pItem,m_iSortColumn);
			qsData = qsData.replace("%","");
			mapSort.insert(qsData.toFloat(),pItem);
		}
		if(m_sortOrder==Qt::AscendingOrder)
			setStocks(mapSort.values());
		else
		{
			QList<CAbstractStockItem*> list;
			QMultiMap<float,CAbstractStockItem*>::iterator iter = mapSort.begin();
			while(iter!=mapSort.end())
			{
				list.push_front(iter.value());
				++iter;
			}
			setStocks(list);
		}
	}
	update(m_rtHeader);
	update(m_rtClient);
}

QString CMarketTrendWidget::dataOfDisplay( CAbstractStockItem* itemData,int column )
{
	switch(column)
	{
	case 0:
		{
			//������
			if(m_mapStockIndex.contains(itemData))
				return QString("%1").arg(m_mapStockIndex[itemData]+1);
			return QString("Error");
		}
		break;
	case 1:
		{
			//��Ʊ����
			return itemData->getCode();
		}
		break;
	case 2:
		{
			//��Ʊ����
			return itemData->getName();
		}
		break;
	case 3:
		{
			//�Ƿ�
			if(_isnan(itemData->getIncrease()))
				return QString();
			return QString("%1%").arg(itemData->getIncrease(),0,'f',2);
		}
		break;
	case 4:
		{
			//����
			if(_isnan(itemData->getVolumeRatio()))
				return QString();
			return QString("%1").arg(itemData->getVolumeRatio(),0,'f',2);
		}
		break;
	case 5:
		{
			//������
			if(_isnan(itemData->getTurnRatio()))
				return QString();
			return QString("%1%").arg(itemData->getTurnRatio(),0,'f',2);
		}
		break;
	case 6:
		{
			//ǰ��
			if(_isnan(itemData->getLastClose()))
				return QString();

			return QString("%1").arg(itemData->getLastClose(),0,'f',2);
		}
		break;
	case 7:
		{
			//��
			if(_isnan(itemData->getOpenPrice()))
				return QString();

			return QString("%1").arg(itemData->getOpenPrice(),0,'f',2);
		}
		break;
	case 8:
		{
			//���
			if(_isnan(itemData->getHighPrice()))
				return QString();

			return QString("%1").arg(itemData->getHighPrice(),0,'f',2);
		}
		break;
	case 9:
		{
			//���
			if(_isnan(itemData->getLowPrice()))
				return QString();

			return QString("%1").arg(itemData->getLowPrice(),0,'f',2);
		}
		break;
	case 10:
		{
			//����
			if(_isnan(itemData->getNewPrice()))
				return QString();

			return QString("%1").arg(itemData->getNewPrice(),0,'f',2);
		}
		break;
	case 11:
		{
			//����
			if(_isnan(itemData->getTotalVolume()))
				return QString();

			return QString("%1").arg(itemData->getTotalVolume(),0,'f',0);
		}
		break;
	case 12:
		{
			//�ܶ�ֲ֣�
			if(_isnan(itemData->getTotalAmount()))
				return QString();

			return QString("%1").arg(itemData->getTotalAmount()/10000,0,'f',0);
		}
		break;
	case 13:
		{
			//����
			if(_isnan(itemData->getNowVolume()))
				return QString();

			return QString("%1").arg(itemData->getNowVolume(),0,'f',0);
		}
		break;
	case 14:
		{
			//����
			if(_isnan(itemData->getIncSpeed()))
				return QString();

			return QString("%1%").arg(itemData->getIncSpeed()*100,0,'f',2);
		}
		break;
	case 15:
		{
			//�ǵ�
			if(_isnan(itemData->getPriceFluctuate()))
				return QString();

			return QString("%1%").arg(itemData->getPriceFluctuate(),0,'f',2);
		}
		break;
	case 16:
		{
			//���
			if(_isnan(itemData->getAmplitude()))
				return QString();

			return QString("%1%").arg(itemData->getAmplitude()*100,0,'f',2);
		}
		break;
	case 17:
		{
			//����
			if(_isnan(itemData->getAvePrice()))
				return QString();

			return QString("%1").arg(itemData->getAvePrice(),0,'f',2);
		}
		break;
	case 18:
		{
			//��ӯ��
			if(_isnan(itemData->getPERatio()))
				return QString();
			return QString("%1").arg(itemData->getPERatio(),0,'f',2);
		}
		break;
	case 19:
		{
			//��ͨ��ֵ
			if(_isnan(itemData->getLTSZ()))
				return QString();
			return QString("%1").arg(itemData->getLTSZ(),0,'f',0);
		}
		break;
	case 20:
		{
			//����
			if(_isnan(itemData->getSellVOL()))
				return QString();
			return QString("%1").arg(itemData->getSellVOL(),0,'f',0);
		}
		break;
	case 21:
		{
			//����
			if(_isnan(itemData->getBuyVOL()))
				return QString();
			return QString("%1").arg(itemData->getBuyVOL(),0,'f',0);
		}
		break;
	case 22:
		{
			//ί����
			if(_isnan(itemData->getBIDVOL()))
				return QString();

			return QString("%1").arg(itemData->getBIDVOL(),0,'f',0);
		}
		break;
	case 23:
		{
			//ί����
			if(_isnan(itemData->getASKVOL()))
				return QString();

			return QString("%1").arg(itemData->getASKVOL(),0,'f',0);
		}
		break;
	case 24:
		{
			//ί��
			if(_isnan(itemData->getCommRatio()))
				return QString();

			return QString("%1%").arg(itemData->getCommRatio(),0,'f',2);
		}
		break;
	case 25:
		{
			//ί��
			if(_isnan(itemData->getCommSent()))
				return QString();

			return QString("%1").arg(itemData->getCommSent(),0,'f',0);
		}
		break;
	default:
		{
			return QString("NULL");
		}
		break;
	}
}

QColor CMarketTrendWidget::dataOfColor( CAbstractStockItem* itemData,int column )
{
	if(column == 1)
		return QColor(127,255,191);
	else if(column == 2)
		return QColor(255,255,0);
	else if(column == 3 || column == 15)
	{
		//�Ƿ�
		return GetColorByFloat(itemData->getIncrease());
	}
	else if(column == 7)
	{
		//��
		float f = itemData->getOpenPrice()-itemData->getLastClose();
		return GetColorByFloat(f);
	}
	else if(column == 8)
	{
		//���
		float f = itemData->getHighPrice()-itemData->getLastClose();
		return GetColorByFloat(f);
	}
	else if(column == 9)
	{
		//���
		float f = itemData->getLowPrice()-itemData->getLastClose();
		return GetColorByFloat(f);
	}
	else if(column == 10)
	{
		//���¼�
		float f = itemData->getNewPrice()-itemData->getLastClose();
		return GetColorByFloat(f);
	}
	else if(column == 17)
	{
		float f = itemData->getAvePrice()-itemData->getLastClose();
		return GetColorByFloat(f);
	}
	else if(column == 20 || column == 22)
		return QColor(255,80,80);
	else if(column == 21 || column == 23)
		return QColor(0,255,255);
	else if(column == 24)
	{
		float f = itemData->getCommRatio();
		return GetColorByFloat(f);
	}
	else if(column == 25)
	{
		float f = itemData->getCommSent();
		return GetColorByFloat(f);
	}

	return QColor(191,191,191);
}

QColor CMarketTrendWidget::dataOfFillColor( CAbstractStockItem* itemData,int column )
{
	if(column == 10)
	{
		//���¼�
		int iMax = itemData->getLastClose()*100*1.1+0.5;
		int iMin = itemData->getLastClose()*100*0.9+0.5;
		int iN = itemData->getNewPrice()*100+0.5;
		if(iMax<=iN)
		{
			//��ͣ
			return QColor(255,0,0);
		}
		else if(iMin>=iN)
		{
			//��ͣ
			return QColor(0,255,255);
		}
	}

	return QColor(0,0,0);
}


QRect CMarketTrendWidget::rectOfStock( CAbstractStockItem* pItem )
{
	if(m_mapStockIndex.contains(pItem))
	{
		int iRow = m_mapStockIndex[pItem];
		return QRect(m_rtClient.left(),(m_rtClient.top()+(iRow-showStockIndex)*m_iStockHeight),m_rtClient.width(),m_iStockHeight);
	}

	return QRect();
}

void CMarketTrendWidget::drawHeaders( QPainter& p )
{
	p.setPen(QColor(255,255,255));
	p.fillRect(m_rtHeader,QColor(0,0,0));
	int iCurX = m_rtHeader.left();
	int iCurIndex = 0;
	for (;iCurIndex<3;++iCurIndex)
	{
		QRect rtItem = QRect(iCurX,m_rtHeader.top(),m_listItemWidth[iCurIndex],m_rtHeader.height());
		p.drawRect(rtItem);
		if((m_iSortColumn == iCurIndex)&&iCurIndex!=0)
		{
			if(m_sortOrder == Qt::AscendingOrder)
			{
				p.drawText(rtItem,Qt::AlignCenter,QString("��%1").arg(m_listHeader[iCurIndex]));
			}
			else
			{
				p.drawText(rtItem,Qt::AlignCenter,QString("��%1").arg(m_listHeader[iCurIndex]));
			}
		}
		else
		{
			p.drawText(rtItem,Qt::AlignCenter,m_listHeader[iCurIndex]);
		}
		iCurX = iCurX+m_listItemWidth[iCurIndex];
	}

	iCurIndex = iCurIndex+showHeaderIndex;
	while(iCurX<m_rtHeader.right()&&iCurIndex<m_listHeader.size())
	{
		QRect rtItem = QRect(iCurX,m_rtHeader.top(),m_listItemWidth[iCurIndex],m_rtHeader.height());
		p.drawRect(rtItem);
		if((m_iSortColumn == iCurIndex))
		{
			if(m_sortOrder == Qt::AscendingOrder)
			{
				p.drawText(rtItem,Qt::AlignCenter,QString("��%1").arg(m_listHeader[iCurIndex]));
			}
			else
			{
				p.drawText(rtItem,Qt::AlignCenter,QString("��%1").arg(m_listHeader[iCurIndex]));
			}
		}
		else
		{
			p.drawText(rtItem,Qt::AlignCenter,m_listHeader[iCurIndex]);
		}
		iCurX = iCurX+m_listItemWidth[iCurIndex];
		++iCurIndex;
	}
}

void CMarketTrendWidget::drawStocks( QPainter& p )
{
	p.fillRect(m_rtClient,QColor(0,0,0));
	int iCurRow = showStockIndex;
	int iCurY = m_rtClient.top();
	while(iCurY<m_rtClient.bottom()&&iCurRow<m_listStocks.size())
	{
		QRect rtStock = QRect(m_rtClient.left(),iCurY,m_rtClient.width(),m_iStockHeight);
		CAbstractStockItem* pStock = m_listStocks[iCurRow];

		if(m_pSelectedStock == pStock)
		{
			p.fillRect(rtStock,QColor(50,50,50));
		}
		drawStock(p,rtStock,pStock);

		++iCurRow;
		iCurY = iCurY + m_iStockHeight;
	}
}

void CMarketTrendWidget::drawBottom( QPainter& p )
{
	p.fillRect(m_rtBottom,QColor(0,0,0));

	p.setPen(QColor(255,255,255));

	QList<QPair<CBlockInfoItem*,QRect>>::iterator iter = m_listBlocks.begin();
	while(iter!=m_listBlocks.end())
	{
		QRect rtBlock = iter->second;
		if(rtBlock.isValid())
		{
			p.drawRect(rtBlock);
			if(m_pSelectedBlock->isChildOf(iter->first))
				p.fillRect(rtBlock,QColor(127,127,127));
			p.drawText(rtBlock,Qt::AlignCenter,iter->first->getBlockName());
		}

		++iter;
	}
	drawBottomBtn(p);
}

void CMarketTrendWidget::drawStock( QPainter& p,const QRect& rtStock,CAbstractStockItem* pItem )
{
	int iCurX = rtStock.left();
	int iCurIndex = 0;
	for (;iCurIndex<3;++iCurIndex)
	{
		QRect rtItem = QRect(iCurX,rtStock.top(),m_listItemWidth[iCurIndex],rtStock.height());
		p.setPen(QColor(127,0,0));
		p.drawRect(rtItem);
		p.setPen(dataOfColor(pItem,iCurIndex));
		p.drawText(rtItem,Qt::AlignCenter,dataOfDisplay(pItem,iCurIndex));
		iCurX = iCurX+m_listItemWidth[iCurIndex];
	}

	iCurIndex = iCurIndex+showHeaderIndex;
	while(iCurX<m_rtHeader.right()&&iCurIndex<m_listHeader.size())
	{
		QRect rtItem = QRect(iCurX,rtStock.top(),m_listItemWidth[iCurIndex],rtStock.height());
		p.setPen(QColor(127,0,0));
		p.drawRect(rtItem);
		QColor clFill = dataOfFillColor(pItem,iCurIndex);
		if(clFill!=QColor(0,0,0))
		{
			p.fillRect(rtItem.adjusted(1,1,-1,-1),clFill);
			p.setPen(QColor(255,255,255));
			p.drawText(rtItem,Qt::AlignCenter,dataOfDisplay(pItem,iCurIndex));
		}
		else
		{
			p.setPen(dataOfColor(pItem,iCurIndex));
			p.drawText(rtItem,Qt::AlignCenter,dataOfDisplay(pItem,iCurIndex));
		}
		iCurX = iCurX+m_listItemWidth[iCurIndex];
		++iCurIndex;
	}
}

void CMarketTrendWidget::drawBottomBtn( QPainter& p )
{
	if(m_rtPreIndex.isValid())
	{
		QRect rtBtn = m_rtPreIndex.adjusted(1,1,0,-1);
		p.fillRect(rtBtn,QColor(200,200,200));
		QPainterPath path;
		path.moveTo(rtBtn.center().x()-m_iBottomHeight/2+4,rtBtn.center().y());
		path.lineTo(rtBtn.right()-4,rtBtn.top()+4);
		path.lineTo(rtBtn.right()-4,rtBtn.bottom()-4);
		p.fillPath(path,QColor(0,0,0));
	}
	if(m_rtNextIndex.isValid())
	{
		QRect rtBtn = m_rtNextIndex.adjusted(1,1,0,-1);
		p.fillRect(rtBtn,QColor(200,200,200));
		QPainterPath path;
		path.moveTo(rtBtn.center().x()+m_iBottomHeight/2-4,rtBtn.center().y());
		path.lineTo(rtBtn.left()+4,rtBtn.top()+4);
		path.lineTo(rtBtn.left()+4,rtBtn.bottom()-4);
		p.fillPath(path,QColor(0,0,0));
	}
}

void CMarketTrendWidget::getKeyWizData( const QString& keyword,QList<KeyWizData*>& listRet )
{
	foreach(CAbstractStockItem* pItem,m_listStocks)
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

void CMarketTrendWidget::keyWizEntered( KeyWizData* pData )
{
	if(pData->cmd == CKeyWizard::CmdStock)
	{
		clickedStock(reinterpret_cast<CAbstractStockItem*>(pData->arg));
		return;
	}

	return CBaseWidget::keyWizEntered(pData);
}

void CMarketTrendWidget::onBlockClicked( CBlockInfoItem* pBlock,int iCmd )
{
	if(!pBlock)
		return;
	if(iCmd == BLOCK_CMD_SHOW)
	{
		clickedBlock(pBlock);
	}
	else if(iCmd == BLOCK_CMD_ADD)
	{

	}
}

void CMarketTrendWidget::setBlock( const QString& block )
{
	return;
	if(!m_bLoaded)
		return;
	CBlockInfoItem* pBlock = CDataEngine::getDataEngine()->getStockBlock(block);
	if(pBlock)
	{
		if(m_pSelectedBlock == pBlock)
		{
			setStocks(pBlock->getAbsStockList());
			resortStocks();
			update();
			return;
		}

		{
			m_pSelectedStock = 0;
			setStocks(pBlock->getAbsStockList());
			m_pSelectedBlock = pBlock;
			{
				//��������ʽ
	//			m_sortOrder = (m_sortOrder==Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
	//			clickedHeader(0);
			}
			resortStocks();
		}
		update();
	}
}

