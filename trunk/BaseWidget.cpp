/************************************************************************/
/* �ļ����ƣ�BaseWidget.cpp
/* ����ʱ�䣺2012-11-16 10:59
/*
/* ��    �������л��ƴ��ڵĻ��֧࣬�ֲ�ֵȲ���
/************************************************************************/
#include "StdAfx.h"
#include "BaseWidget.h"
#include "KLineWidget.h"
#include "MarketTrendWidget.h"
#include "ColorBlockWidget.h"
#include "BColorBlockWidget.h"
#include "StockInfoWidget.h"
#include "RadarWidget.h"
#include "StockDealWidget.h"
#include "AllStockWidget.h"
#include "FlashWidget.h"
#include "KeyWizard.h"

CBaseWidget* CBaseWidget::createBaseWidget( CBaseWidget* parent/*=0*/, RWidgetType type/*=Basic*/ )
{
	switch(type)
	{
	case WidgetBasic:				//����ͼ
		return new CBaseWidget(parent);
		break;
	case WidgetKLine:					//K��ͼ
		return new CKLineWidget(parent);
		break;
	case WidgetMarketTrend:			//�г�����ͼ
		return new CMarketTrendWidget(parent);
		break;
	case WidgetSColorBlock:
		return new CColorBlockWidget(parent);
		break;
	case WidgetBColorBlock:
		return new CBColorBlockWidget(parent);
		break;
	case WidgetStockInfo:
		return new CStockInfoWidget(parent);
		break;
	case WidgetRadar:
		return new CRadarWidget(parent);
		break;
	case WidgetStockDeal:
		return new CStockDealWidget(parent);
		break;
	case WidgetAllStock:
		return new CAllStockWidget(parent);
		break;
	case WidgetFlash:
		return new CFlashWidget(parent);
		break;
	}

	return new CBaseWidget(parent);
}

CBaseWidget::CBaseWidget( CBaseWidget* parent /*= 0*/, RWidgetType type /*= Basic*/ )
	: QWidget(parent)
	, m_pParent(parent)
	, m_type(type)
	, m_bClearMode(false)
{
	setMouseTracking(true);
	//��Splitter���뵽Layout�У�����ʹ�������������
	m_pLayout = new QVBoxLayout();
	m_pLayout->setMargin(0);		//���Ĵ˴�������Panel�ı߿��С
	m_pSplitter = new QSplitter(Qt::Vertical,this);
	m_pSplitter->setHandleWidth(1);
	m_pSplitter->setFrameShadow(QFrame::Sunken);
	m_pSplitter->setOrientation(Qt::Horizontal);
	m_pSplitter->setMouseTracking(true);
	m_pLayout->addWidget(m_pSplitter);
	setLayout(m_pLayout);

	if(m_pParent == 0 && m_type==WidgetBasic)
	{
		//ȷ�����пɲ������Ĵ��ڶ����и����ڵ�
		m_pSplitter->addWidget(new CBaseWidget(this));
	}

	{
		//��ʼ����ʾ���ڵĿ��ٲ��ұ�
		m_listWidget.push_back(RWidgetOpData(WidgetBasic,"vb","��������"));
		m_listWidget.push_back(RWidgetOpData(WidgetKLine,"vk","K��ͼ"));
		m_listWidget.push_back(RWidgetOpData(WidgetMarketTrend,"vm","�г�����ͼ"));
		m_listWidget.push_back(RWidgetOpData(WidgetSColorBlock,"vs","��Ʊɫ��ͼ"));
		m_listWidget.push_back(RWidgetOpData(WidgetBColorBlock,"vl","���ɫ��ͼ"));
		m_listWidget.push_back(RWidgetOpData(WidgetStockInfo,"vi","������Ϣ"));
		m_listWidget.push_back(RWidgetOpData(WidgetRadar,"vr","�״����"));
		m_listWidget.push_back(RWidgetOpData(WidgetStockDeal,"vd","�ɽ�����ͼ"));
		m_listWidget.push_back(RWidgetOpData(WidgetFlash,"vf","�����л�ͼ"));
	}

	{
		m_listOperate.push_back(RWidgetOpData(InsertLeft,"il","����봰��"));
		m_listOperate.push_back(RWidgetOpData(InsertRight,"ir","�Ҳ��봰��"));
		m_listOperate.push_back(RWidgetOpData(InsertTop,"it","�ϲ��봰��"));
		m_listOperate.push_back(RWidgetOpData(InsertBottom,"ib","�²��봰��"));
		m_listOperate.push_back(RWidgetOpData(InsertRemove,"id","ɾ������"));
	}
	setFocusPolicy(Qt::WheelFocus);

	initMenu();			//��ʼ���˵�
}


CBaseWidget::~CBaseWidget(void)
{
	clearChildren();
	delete m_pActRelate;
	delete m_pMenu;
	delete m_pSplitter;
}

void CBaseWidget::initMenu()
{
	m_pMenu = new QMenu(tr("���ư���˵�"));
	{
		//���ð�������
		QMenu* pMenuType = m_pMenu->addMenu(tr("���ð�������"));
		{
			foreach(const RWidgetOpData& _d,m_listWidget)
			{
				pMenuType->addAction(_d.desc,this,SLOT(onResetWidget()))->setData(_d.value);
			}
		}
		m_pMenu->addSeparator();
	}

	{
		//���ò��뷽ʽ
		foreach(const RWidgetOpData& _d,m_listOperate)
		{
			m_pMenu->addAction(_d.desc,this,SLOT(onInsertWidget()))->setData(_d.value);
		}
		m_pMenu->addSeparator();
		m_pActRelate = new QAction(tr("���������ڹ���"),m_pMenu);
		m_pMenu->addAction(m_pActRelate);
		m_pActRelate->setCheckable(true);
		m_pActRelate->setChecked(true);
	}
}

void CBaseWidget::realignSplitter()
{
	QList<int> listSize;
	listSize.push_back(-1);
	m_pSplitter->setSizes(listSize);
}

void CBaseWidget::resetParent( CBaseWidget* parent )
{
	m_pParent = parent;
	QWidget::setParent(parent);
}

QList<CBaseWidget*> CBaseWidget::getChildren()
{
	QList<CBaseWidget*> list;
	for(int i=0;i<m_pSplitter->count();++i)
	{
		list.push_back(reinterpret_cast<CBaseWidget*>(m_pSplitter->widget(i)));
	}
	return list;
}

void CBaseWidget::clearChildren()
{
	while(m_pSplitter->count()>0)
	{
		CBaseWidget* pPanel = static_cast<CBaseWidget*>(m_pSplitter->widget(0));
		if(pPanel)
			delete pPanel;
	}
}

int CBaseWidget::getSize()
{
	CBaseWidget* pParent = getParent();
	if(!pParent)
		return 100;
	QSplitter* pParentSplitter = pParent->getSplitter();
	if(!pParentSplitter)
		return 100;

	int iTotal = 0;
	int iSize = 0;
	QList<int> sizes = pParentSplitter->sizes();
	if(sizes.size()<2)
		return 100;

	for(int i=0;i<sizes.size();++i)
	{
		if(pParentSplitter->widget(i)==this)
			iSize = sizes[i];
		iTotal = iTotal + sizes[i];
	}

	return (iSize*100)/iTotal;
}

int CBaseWidget::getWidgetIndex( CBaseWidget* widget ) const
{
	return m_pSplitter->indexOf(widget);
}

void CBaseWidget::replaceWidget( int index, CBaseWidget* widget )
{
	QList<int> sizes = m_pSplitter->sizes();
	CBaseWidget* pWidgetPre = reinterpret_cast<CBaseWidget*>(m_pSplitter->widget(index));
	pWidgetPre->setParent(NULL);
	pWidgetPre->deleteLater();
	m_pSplitter->insertWidget(index,widget);
	QApplication::flush();
	m_pSplitter->setSizes(sizes);
	
	//�����´��ڻ�ȡ����
	if(::GetForegroundWindow() != widget->winId())
		::SetForegroundWindow(widget->winId());
}

bool CBaseWidget::loadPanelInfo( const QDomElement& eleWidget )
{
	//��ȡWidget��Ϣ
	if(!eleWidget.isElement())
		return false;

	//��ȡ����
	QDomElement eleName = eleWidget.firstChildElement("name");
	if(eleName.isElement())
		setWidgetName(eleName.text());

	//��ˬģʽ
	QDomElement eleClearMode = eleWidget.firstChildElement("ClearMode");
	if(eleClearMode.isElement())
		m_bClearMode = static_cast<bool>(eleClearMode.text().toInt());

	//��ȡ�ؼ������з�ʽ
	QDomElement eleAlign = eleWidget.firstChildElement("align");
	if(eleAlign.isElement())
		m_pSplitter->setOrientation(static_cast<Qt::Orientation>(eleAlign.text().toInt()));

	//��ȡ������ʽ
	QDomElement eleRelate = eleWidget.firstChildElement("relate");
	if(eleRelate.isElement())
		m_pActRelate->setChecked(eleRelate.text().toInt());

	QList<int> sizes;
	QDomElement eleChild = eleWidget.firstChildElement("widget");
	while(eleChild.isElement())
	{
		QDomElement eleChildSize = eleChild.firstChildElement("size");
		if(eleChildSize.isElement())
			sizes.push_back(eleChildSize.text().toInt());

		QDomElement eleChildType = eleChild.firstChildElement("type");
		RWidgetType typeChild = WidgetBasic;
		if(eleChildType.isElement())
			typeChild = static_cast<RWidgetType>(eleChildType.text().toInt());

		CBaseWidget* pWidget = createBaseWidget(this,typeChild);
		m_pSplitter->addWidget(pWidget);
		pWidget->loadPanelInfo(eleChild);

		eleChild = eleChild.nextSiblingElement("widget");
	}
	m_pSplitter->setSizes(sizes);

	return true;
}

bool CBaseWidget::savePanelInfo( QDomDocument& doc,QDomElement& eleWidget )
{
	//����Panel��Ϣ
	if(!eleWidget.isElement())
		return false;

	//��ȡ����
	QDomElement eleName = doc.createElement("name");
	eleName.appendChild(doc.createTextNode(getWidgetName()));
	eleWidget.appendChild(eleName);

	//��ˬģʽ
	QDomElement eleClearMode = doc.createElement("ClearMode");
	eleClearMode.appendChild(doc.createTextNode(QString("%1").arg(static_cast<int>(m_bClearMode))));
	eleWidget.appendChild(eleClearMode);

	//��ȡ�ؼ�����
	QDomElement eleType = doc.createElement("type");
	eleType.appendChild(doc.createTextNode(QString("%1").arg(m_type)));
	eleWidget.appendChild(eleType);

	//��ȡ������ʽ
	QDomElement eleRelate = doc.createElement("relate");
	eleRelate.appendChild(doc.createTextNode(QString("%1").arg(isRelate())));
	eleWidget.appendChild(eleRelate);

	//��ȡ�ؼ���Splitter�����з�ʽ
	QDomElement eleAlgin = doc.createElement("align");
	eleAlgin.appendChild(doc.createTextNode(QString("%1").arg(m_pSplitter->orientation())));
	eleWidget.appendChild(eleAlgin);

	//��ȡ�ؼ��ڸ������еĴ�С
	QDomElement eleSize = doc.createElement("size");
	eleSize.appendChild(doc.createTextNode(QString("%1").arg(getSize())));
	eleWidget.appendChild(eleSize);

	//������Panel
	QList<CBaseWidget*> listChildren = getChildren();
	foreach(CBaseWidget* pChild,listChildren)
	{
		QDomElement eleChild = doc.createElement("widget");
		eleWidget.appendChild(eleChild);
		pChild->savePanelInfo(doc,eleChild);
	}

	return true;
}

void CBaseWidget::setStockCode( const QString& code )
{
	QList<CBaseWidget*> children = getChildren();
	foreach(CBaseWidget* p,children)
	{
		if(p->isRelate())
			p->setStockCode(code);
	}
}

void CBaseWidget::setBlock( const QString& block )
{
	QList<CBaseWidget*> children = getChildren();
	foreach(CBaseWidget* p,children)
	{
		if(p->isRelate())
			p->setBlock(block);
	}
}

void CBaseWidget::onBlockClicked( CBlockInfoItem* /*pBlock*/,int /*iCmd*/ )
{

}

void CBaseWidget::paintEvent( QPaintEvent* )
{
	QPainter p(this);
	p.fillRect(this->rect(),QColor(0,0,0));
	p.setPen(QColor(255,255,255));
	p.drawText(this->rect(),Qt::AlignCenter,QString(tr("�����Ҽ�ѡ���������")));
}

void CBaseWidget::contextMenuEvent( QContextMenuEvent* e )
{
	QMenu* pMenuCustom = getCustomMenu();
	if(pMenuCustom)
		pMenuCustom->exec(e->globalPos());
	else
		m_pMenu->exec(e->globalPos());
	return e->accept();
}

void CBaseWidget::keyPressEvent( QKeyEvent* e )
{
	//���β���Ҫ�İ���
	int iKey = e->key();
	if( Qt::Key_Escape == iKey || Qt::Key_Backspace == iKey)
		return;

	QString _t = e->text().trimmed();
	if(!_t.isEmpty())
	{
		//�������̾���Ի���
		CKeyWizard::getKeyWizard()->showWizard(this,_t);
	}
}

void CBaseWidget::mousePressEvent( QMouseEvent* /*e*/ )
{
	CKeyWizard* pWizard = CKeyWizard::getKeyWizard();
	if(pWizard->isVisible())
	{
		if(pWizard->getCurrentWidget() == this)
		{
			pWizard->showWizard(this);
		}
		else
		{
			pWizard->hide();
		}
	}
}

void CBaseWidget::onLeftInsert()
{
	//�����
	QSplitter* pSplitterParent = m_pParent->getSplitter();		//�����ڵ�Splitterָ��
	int iIndex = pSplitterParent->indexOf(this);				//���ڵ�������
	if(iIndex<0)
		return;

	if((pSplitterParent->orientation() == Qt::Horizontal) ||
		(pSplitterParent->count() == 1))
	{
		//ˮƽ�����Splitter ���� ��ֱ���򣬵�ֻ��һ���Ӵ���
		pSplitterParent->setOrientation(Qt::Horizontal);
		pSplitterParent->insertWidget(iIndex,new CBaseWidget(m_pParent));
		m_pParent->realignSplitter();
	}
	else														//��ֱ����ģ����Ѿ������˴�ֱ����
	{
		CBaseWidget* pWidgetParent = new CBaseWidget(m_pParent);
		pSplitterParent->insertWidget(iIndex,pWidgetParent);
		resetParent(pWidgetParent);

		pWidgetParent->getSplitter()->setOrientation(Qt::Horizontal);
		pWidgetParent->getSplitter()->addWidget(new CBaseWidget(pWidgetParent));
		pWidgetParent->getSplitter()->addWidget(this);

		pWidgetParent->realignSplitter();
	}
}

void CBaseWidget::onRightInsert()
{
	//�Ҳ���
	QSplitter* pSplitterParent = m_pParent->getSplitter();		//�����ڵ�Splitterָ��
	int iIndex = pSplitterParent->indexOf(this);				//���ڵ�������
	if(iIndex<0)
		return;

	if((pSplitterParent->orientation() == Qt::Horizontal) ||
		(pSplitterParent->count() == 1))
	{
		//ˮƽ�����Splitter ���� ��ֱ���򣬵�ֻ��һ���Ӵ���
		pSplitterParent->setOrientation(Qt::Horizontal);
		pSplitterParent->insertWidget(iIndex+1,new CBaseWidget(m_pParent));
		m_pParent->realignSplitter();
	}
	else														//��ֱ����ģ����Ѿ������˴�ֱ����
	{
		CBaseWidget* pWidgetParent = new CBaseWidget(m_pParent);
		pSplitterParent->insertWidget(iIndex,pWidgetParent);
		resetParent(pWidgetParent);

		pWidgetParent->getSplitter()->setOrientation(Qt::Horizontal);
		pWidgetParent->getSplitter()->addWidget(this);
		pWidgetParent->getSplitter()->addWidget(new CBaseWidget(pWidgetParent));

		pWidgetParent->realignSplitter();
	}
}

void CBaseWidget::onTopInsert()
{
	//�ϲ���
	QSplitter* pSplitterParent = m_pParent->getSplitter();		//�����ڵ�Splitterָ��
	int iIndex = pSplitterParent->indexOf(this);				//���ڵ�������
	if(iIndex<0)
		return;

	if((pSplitterParent->orientation() == Qt::Vertical) ||
		(pSplitterParent->count() == 1))
	{
		//ˮƽ�����Splitter ���� ��ֱ���򣬵�ֻ��һ���Ӵ���
		pSplitterParent->setOrientation(Qt::Vertical);
		pSplitterParent->insertWidget(iIndex,new CBaseWidget(m_pParent));
		m_pParent->realignSplitter();
	}
	else														//��ֱ����ģ����Ѿ������˴�ֱ����
	{
		CBaseWidget* pWidgetParent = new CBaseWidget(m_pParent);
		pSplitterParent->insertWidget(iIndex,pWidgetParent);
		resetParent(pWidgetParent);

		pWidgetParent->getSplitter()->setOrientation(Qt::Vertical);
		pWidgetParent->getSplitter()->addWidget(new CBaseWidget(pWidgetParent));
		pWidgetParent->getSplitter()->addWidget(this);

		pWidgetParent->realignSplitter();
	}
}

void CBaseWidget::onBottomInsert()
{
	//�²���
	QSplitter* pSplitterParent = m_pParent->getSplitter();		//�����ڵ�Splitterָ��
	int iIndex = pSplitterParent->indexOf(this);				//���ڵ�������
	if(iIndex<0)
		return;

	if((pSplitterParent->orientation() == Qt::Vertical) ||
		(pSplitterParent->count() == 1))
	{
		//ˮƽ�����Splitter ���� ��ֱ���򣬵�ֻ��һ���Ӵ���
		pSplitterParent->setOrientation(Qt::Vertical);
		pSplitterParent->insertWidget(iIndex+1,new CBaseWidget(m_pParent));
		m_pParent->realignSplitter();
	}
	else														//��ֱ����ģ����Ѿ������˴�ֱ����
	{
		CBaseWidget* pWidgetParent = new CBaseWidget(m_pParent);
		pSplitterParent->insertWidget(iIndex,pWidgetParent);
		resetParent(pWidgetParent);

		pWidgetParent->getSplitter()->setOrientation(Qt::Vertical);
		pWidgetParent->getSplitter()->addWidget(this);
		pWidgetParent->getSplitter()->addWidget(new CBaseWidget(pWidgetParent));

		pWidgetParent->realignSplitter();
	}
}

void CBaseWidget::onInsertWidget()
{
	QAction* pAct = reinterpret_cast<QAction*>(sender());
	insertWidget(static_cast<RWidgetInsert>(pAct->data().toInt()));
}

void CBaseWidget::insertWidget(RWidgetInsert _i)
{
	switch (_i)
	{
	case InsertLeft:
		onLeftInsert();
		break;
	case InsertRight:
		onRightInsert();
		break;
	case InsertTop:
		onTopInsert();
		break;
	case InsertBottom:
		onBottomInsert();
		break;
	case InsertRemove:
		deleteLater();
		break;
	default:
		break;
	}
}

void CBaseWidget::onResetWidget()
{
	QAction* pAct = reinterpret_cast<QAction*>(sender());
	//��ȡ�����ô�������
	setWidgetType(static_cast<RWidgetType>(pAct->data().toInt()));
}

void CBaseWidget::setWidgetType(RWidgetType _t)
{
	int iIndex = m_pParent->getWidgetIndex(this);						//��ȡ��ǰ�������ڵ�����
	if(iIndex>=0)
	{
		m_pParent->replaceWidget(iIndex,createBaseWidget(m_pParent,_t));	//�滻����
	}
}

void CBaseWidget::getKeyWizData( const QString& keyword,QList<KeyWizData*>& listRet )
{
	foreach(const RWidgetOpData& _d,m_listWidget)
	{
		if(_d.key.indexOf(keyword)>-1)
		{
			KeyWizData* pData = new KeyWizData;
			pData->cmd = CKeyWizard::CmdWidget;
			pData->arg = (void*)_d.value;
			pData->desc = _d.desc;
			listRet.push_back(pData);
			if(listRet.size()>20)
				return;
		}
	}
	
	foreach(const RWidgetOpData& _d,m_listOperate)
	{
		if(_d.key.indexOf(keyword)>-1)
		{
			KeyWizData* pData = new KeyWizData;
			pData->cmd = CKeyWizard::CmdInsert;
			pData->arg = (void*)_d.value;
			pData->desc = _d.desc;
			listRet.push_back(pData);
			if(listRet.size()>20)
				return;
		}
	}
}

void CBaseWidget::keyWizEntered( KeyWizData* pData )
{
	if(pData->cmd == CKeyWizard::CmdWidget)
	{
		setWidgetType(static_cast<RWidgetType>((int)(pData->arg)));
		return;
	}
	else if(pData->cmd == CKeyWizard::CmdInsert)
	{
		insertWidget(static_cast<RWidgetInsert>((int)(pData->arg)));
		return;
	}
}

