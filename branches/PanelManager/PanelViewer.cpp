/*
============================================================================
�ļ�����	:	PanelViewer.cpp
����		:	���ǿ�
����ʱ��	:	2011-12-29 16:56
�޸�ʱ��	:	2011-12-29 16:57
˵��		:	����ʵ��Panel�Ļ��ƣ��Լ�Panel����Ӧ��һЩ������֧��Ƕ��
============================================================================
*/

#include "common.h"
#include "PanelViewer.h"
#include <QPainter>
#include <QLayout>
#include <QMessageBox>
#include "ListPanel.h"
#include "PanelProperty.h"
#include "PanelResource.h"

#include "ROIViewer.h"
#include "ReconsViewer.h"
#include "ReformsViewer.h"
#include "BullsEyeViewer.h"
#include "ImageViewer.h"

using namespace eGingkoPanelManager;

//��̬�����ĳ�ʼ��
CPanelResource* CPanelViewer::g_pPanelResource = 0;
CPanelViewer* CPanelViewer::g_pPanelFocusing = 0;
CPanelViewer* CPanelViewer::g_pPanelMain = 0;
QMap<int, QImage*> CPanelViewer::g_mapPixmapPanel;



bool CPanelViewer::g_bDraging = false;
bool CPanelViewer::g_bLoading = false;

CPanelViewer* CPanelViewer::createPanelViewer( int iType,QTreeWidgetItem* pItemParent,QWidget* parent/*=0*/ )
{
	switch (iType)
	{
	case GKG_VIEWER_ROI:
		return new CROIViewer(pItemParent,parent);
		break;
	case GKG_VIEWER_RECONS:
		return new CReconsViewer(pItemParent,parent);
		break;
	case GKG_VIEWER_REFORMS:
		return new CReformsViewer(pItemParent,parent);
		break;
	case GKG_VIEWER_BullsEye:
		return new CBullsEyeViewer(pItemParent,parent);
		break;
	case GKG_VIEWER_IMAGE:
		return new CImageViewer(pItemParent,parent);
		break;
	default:
		return new CPanelViewer(pItemParent,iType,parent);
		break;
	}

	return 0;
}

CPanelViewer::CPanelViewer( QTreeWidgetItem* pItemParent, int iType, QWidget* parent /*= 0*/ )
	: QWidget(parent),m_iTitleHeight(20),m_pParent(parent),
	m_pItemParent(pItemParent),m_bFocus(false),m_iDock(PanelFill),
	m_iViewerType(static_cast<GKG_VIEWERTYPE>(iType))
{
	static int g_iPanelCount = 0;
	m_qsName = QString(tr("Panel%1")).arg(g_iPanelCount);
	++g_iPanelCount;

	m_pImgBack = g_mapPixmapPanel[m_iViewerType];


	//��ʼ����ʽ
	setAcceptDrops(true);

	//��Splitter���뵽Layout�У�����ʹ�������������
	QVBoxLayout* pLayout = new QVBoxLayout();
	pLayout->setMargin(0);		//���Ĵ˴�������Panel�ı߿��С
	m_pSplitter = new QSplitter(Qt::Vertical,this);
	m_pSplitter->setHandleWidth(1);
	m_pSplitter->setFrameShadow(QFrame::Sunken);
	pLayout->addWidget(m_pSplitter);
	setLayout(pLayout);

	if(m_pItemParent)
	{
		//���ø��ڵ�
		m_pItem = new QTreeWidgetItem(m_pItemParent);
		m_pItem->setData(0,Qt::DisplayRole,m_qsName);
		m_pItem->setData(0,Qt::UserRole,(uint)this);
	}
	else
	{
		//���ڵ�Ϊ�գ���Ĭ�ϸýڵ�Ϊ�����ڽڵ�
		m_pItem = new QTreeWidgetItem(g_pPanelResource);
		m_pItem->setData(0,Qt::DisplayRole,tr("MainWindow"));
		m_pItem->setData(0,Qt::UserRole,(uint)this);
		m_qsName = tr("MainWindow");
		g_pPanelMain = this;
	}

	//���ô��ڵ���Сֵ�����ú�������⣬��ʱ�������á�
//	setMinimumSize(75,20);
}

CPanelViewer::~CPanelViewer(void)
{
	//��մ����д�������Դ
	delete m_pSplitter;
	delete m_pItem;
}

void CPanelViewer::paintEvent( QPaintEvent *event )
{
	QPainter painter;
	painter.begin(this);		//��ʼ����
	painter.setRenderHint(QPainter::Antialiasing, true);

	//��䱳��ɫ
	painter.fillRect(event->rect(), Qt::gray);
	painter.fillRect(event->rect().adjusted(2,2,-2,-2),Qt::black);

	if(m_pSplitter->count()==0)
	{
		//����title
		QRect rtTitle = event->rect().adjusted(0,1,0,m_iTitleHeight-event->rect().height());
		if(m_bFocus)
		{
			painter.fillRect(rtTitle.adjusted(2,0,-2,0), Qt::blue);
			painter.setPen(QPen(QBrush(Qt::white),2,Qt::SolidLine));
		}
		else
			painter.fillRect(rtTitle, Qt::lightGray);

		{
			//���Ʊ�������
			QFontMetrics metrics = painter.fontMetrics();
			QRect rtText = rtTitle.adjusted(5,2,-60,0);
			QString qsName = m_qsName;
			if(metrics.width(qsName)>rtText.width())
			{
				while(metrics.width(qsName+"...")>rtText.width()&&qsName.length()>1)
				{
					qsName = qsName.left(qsName.length()-1);
				}
				painter.drawText(rtText,Qt::AlignLeft,qsName+"...");
			}
			else
				painter.drawText(rtText,Qt::AlignLeft,qsName);
		}

		//��󻯰�ť
		QRect rtMaximum = rtTitle.adjusted(rtTitle.width()-60,2,-44,-2);
		painter.fillRect(rtMaximum,Qt::white);
		if(m_bFocus)
			painter.fillRect(rtMaximum.adjusted(2,2,-2,-2), Qt::blue);
		else
			painter.fillRect(rtMaximum.adjusted(2,2,-2,-2),Qt::lightGray);

		//��С����ť
		QRect rtMinimum = rtTitle.adjusted(rtTitle.width()-40,8,-24,-8);
		painter.fillRect(rtMinimum,Qt::white);

		//��󻯰�ť
		m_rtClose = rtTitle.adjusted(rtTitle.width()-20,2,-4,-2);
		QPen pen(QBrush(Qt::white),2,Qt::SolidLine);
		painter.setPen(pen);
		painter.drawLine(m_rtClose.topLeft(),m_rtClose.bottomRight());
		painter.drawLine(m_rtClose.topRight(),m_rtClose.bottomLeft());
	}

	
	//�����豸���ͻ���panel�������ʾͼƬ
	QRect imageRect = event->rect().adjusted(2,m_iTitleHeight+2,-2,-2);

	//if(imageRect.width()>=m_pImgBack->width()&&imageRect.height()>=m_pImgBack->height())
	//{
	//	//�����Ƶ��������ͼƬ��Сʱ����ȡ���Ļ�������
	//	imageRect = event->rect().adjusted(0,0,-4,-m_iTitleHeight-4);
	//	imageRect = QRect(imageRect.center()-QPoint(m_pImgBack->width()/2,m_pImgBack->height()/2),
	//		imageRect.center()+QPoint(m_pImgBack->width()/2,m_pImgBack->height()/2));
	//	imageRect = imageRect.adjusted(2,m_iTitleHeight+2,2,m_iTitleHeight+2);
	//}

	//���ֱ�����������󻯻��ơ�
	QImage img = m_pImgBack->scaled(imageRect.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
	if(imageRect.width()>img.width())
	{
		imageRect.setLeft(imageRect.left()+(imageRect.width()-img.width())/2);
		imageRect.setRight(imageRect.right()-(imageRect.width()-img.width())/2);
	}
	if(imageRect.height()>img.height())
	{
		imageRect.setTop(imageRect.top()+(imageRect.height()-img.height())/2);
		imageRect.setBottom(imageRect.bottom()-(imageRect.height()-img.height())/2);
	}
	painter.drawImage(imageRect,img);


	//���Ƹ���
	if (m_rtHighlighted.isValid())
	{
		painter.setBrush(QColor("#ffcccc"));
		painter.setPen(Qt::NoPen);
		painter.drawRect(m_rtHighlighted.adjusted(0, 0, -1, -1));
	}


	painter.end();		//��ɻ���
}

void CPanelViewer::dragEnterEvent( QDragEnterEvent *event )
{
	if (event->mimeData()->hasFormat("panel/new")||event->mimeData()->hasFormat("panel/move"))
	{
		event->accept();
		g_bDraging = true;
	}
	else
	{
		event->ignore();
		g_bDraging = false;
	}
}

void CPanelViewer::dragLeaveEvent( QDragLeaveEvent *event )
{
	m_rtHighlighted = QRect();
	update();
	event->accept();
}

void CPanelViewer::dragMoveEvent( QDragMoveEvent *event )
{
	if (event->mimeData()->hasFormat("panel/new"))
	{
		m_rtHighlighted = getHighlightedRect(event->pos());
		event->setDropAction(Qt::CopyAction);
		event->accept();
		g_bDraging = true;
	}
	else if(event->mimeData()->hasFormat("panel/move"))
	{
		//ȡ�����ƶ���panelָ��
		uint iPanelMoving = 0;
		QDataStream dataStream(&event->mimeData()->data("panel/move"), QIODevice::ReadOnly);
		dataStream>>iPanelMoving;

		CPanelViewer* pPanelMoving = reinterpret_cast<CPanelViewer*>(iPanelMoving);
		if(pPanelMoving&&!hasParent(pPanelMoving))
		{
			m_rtHighlighted = getHighlightedRect(event->pos());
			event->setDropAction(Qt::MoveAction);
			event->accept();
			g_bDraging = true;
		}
		else
		{
			m_rtHighlighted = QRect();
			event->ignore();
			g_bDraging = false;
		}
	}
	else
	{
		m_rtHighlighted = QRect();
		event->ignore();
		g_bDraging = false;
	}

	update();
}

void CPanelViewer::dropEvent( QDropEvent *event )
{
	m_rtHighlighted = QRect();
	update();
	g_bDraging = false;
//	g_bLoading = true;		//��ֹ�Զ�����Panel��ֵ

	if (event->mimeData()->hasFormat("panel/new"))
	{
		int iType = 0;	//get the type
		QDataStream dataStream(&event->mimeData()->data("panel/new"), QIODevice::ReadOnly);
		dataStream>>iType;
		//��Ӵ���
		PanelDirect direct = getDirect(event->pos());
		if(direct == PanelFill || (!m_pParent->inherits("eGingkoPanelManager::CPanelViewer")))
		{
			//ֱ����ӵ�splitter��
			CPanelViewer* pPanelChild = createPanelViewer(iType,m_pItem,this);
			pPanelChild->setPanelDock(PanelFill);

			m_pSplitter->addWidget(pPanelChild);
		}
		else if(m_pParent&&m_pParent->inherits("eGingkoPanelManager::CPanelViewer"))
		{
			//���ȡ�����ڣ����뵽�������е�splitter
			Qt::Orientation orien = (direct == PanelBottom||direct == PanelTop) ? Qt::Vertical : Qt::Horizontal;

			CPanelViewer* pPanelParent = static_cast<CPanelViewer*>(m_pParent);
			QSplitter* pSplitterParent = pPanelParent->getSplitter();

			if(pSplitterParent->count()>1&&pSplitterParent->orientation()!=orien)
			{
				//��������Panel��һ����Ϊ��Panel
				CPanelViewer* pPanelParentNew = createPanelViewer(GKG_VIEWER_NORMAL,m_pItemParent,m_pParent);
				pPanelParentNew->setPanelDock(getPanelDock());
				CPanelViewer* pPanelChild2 = createPanelViewer(iType,pPanelParentNew->getTreeItem(),pPanelParentNew);
				CPanelViewer* pPanelGrandparent = static_cast<CPanelViewer*>(m_pParent);
				QList<int> listSizesParent = pPanelGrandparent->getSplitter()->sizes();

				pPanelParentNew->getSplitter()->setOrientation(orien);
				pPanelGrandparent->getSplitter()->insertWidget(pPanelGrandparent->getSplitter()->indexOf(this),pPanelParentNew);
				resetParent(pPanelParentNew);
				pPanelGrandparent->getSplitter()->setSizes(listSizesParent);

				pPanelParentNew->getSplitter()->addWidget(this);
				if( direct == PanelTop || direct == PanelLeft )
				{
					pPanelChild2->setPanelDock(direct);
					setPanelDock(PanelFill);
					pPanelParentNew->getSplitter()->insertWidget(0,pPanelChild2);
				}
				else
				{
					pPanelChild2->setPanelDock(PanelFill);
					setPanelDock(pPanelParentNew->getSplitter()->orientation()==Qt::Horizontal ? PanelLeft : PanelTop);
					pPanelParentNew->getSplitter()->addWidget(pPanelChild2);
				}
				pPanelParentNew->resizeSplitter();
			}
			else
			{
				CPanelViewer* pPanelChild = createPanelViewer(iType,pPanelParent->getTreeItem(),pPanelParent);
				pSplitterParent->setOrientation(orien);

				int iIndex = pSplitterParent->indexOf(this);
				QList<int> listSizes = pSplitterParent->sizes();
				if(listSizes.size()>iIndex)
				{
					listSizes[iIndex] = listSizes[iIndex]/2;
					listSizes.insert(iIndex,listSizes[iIndex]);
				}

				if( direct == PanelTop || direct == PanelLeft )
				{
					pPanelChild->setPanelDock(direct);
					pSplitterParent->insertWidget(iIndex,pPanelChild);
				}
				else
				{
					if(pSplitterParent->indexOf(this)==(pSplitterParent->count()-1))
					{
						pPanelChild->setPanelDock(PanelFill);
						setPanelDock(pSplitterParent->orientation()==Qt::Horizontal ? PanelLeft : PanelTop);
					}
					else
						pPanelChild->setPanelDock(pSplitterParent->orientation()==Qt::Horizontal ? PanelLeft : PanelTop);
					pSplitterParent->insertWidget(iIndex+1,pPanelChild);
				}
				pSplitterParent->setSizes(listSizes);
			}
		}
	}
	else if(event->mimeData()->hasFormat("panel/move"))
	{
		//ȡ�����ƶ���panelָ��
		uint iPanelMoving = 0;
		QDataStream dataStream(&event->mimeData()->data("panel/move"), QIODevice::ReadOnly);
		dataStream>>iPanelMoving;
		CPanelViewer* pPanelMoving = reinterpret_cast<CPanelViewer*>(iPanelMoving);
		if((!pPanelMoving)||hasParent(pPanelMoving))
			return;

		//�ƶ�����
		PanelDirect direct = getDirect(event->pos());
		if(direct == PanelFill || (!m_pParent->inherits("eGingkoPanelManager::CPanelViewer")))
		{
			//ֱ����ӵ�splitter��
			pPanelMoving->resetParent(this);
			pPanelMoving->setPanelDock(direct);
			m_pSplitter->addWidget(pPanelMoving);
		}
		else if(m_pParent&&m_pParent->inherits("eGingkoPanelManager::CPanelViewer"))
		{
			//���ȡ�����ڣ����뵽�������е�splitter
			Qt::Orientation orien = (direct == PanelBottom||direct == PanelTop) ? Qt::Vertical : Qt::Horizontal;

			CPanelViewer* pPanelParent = static_cast<CPanelViewer*>(m_pParent);
			QSplitter* pSplitterParent = pPanelParent->getSplitter();

			if(pSplitterParent->count()>1&&pSplitterParent->orientation()!=orien)
			{
				//��������Panel��һ����Ϊ��Panel
				CPanelViewer* pPanelParentNew = createPanelViewer(GKG_VIEWER_NORMAL,m_pItemParent,m_pParent);
				pPanelParentNew->setPanelDock(getPanelDock());
				CPanelViewer* pPanelGrandparent = static_cast<CPanelViewer*>(m_pParent);
				QList<int> listSizesParent = pPanelGrandparent->getSplitter()->sizes();

				pPanelParentNew->getSplitter()->setOrientation(orien);
				pPanelGrandparent->getSplitter()->insertWidget(pPanelGrandparent->getSplitter()->indexOf(this),pPanelParentNew);
				resetParent(pPanelParentNew);
				pPanelGrandparent->getSplitter()->setSizes(listSizesParent);

				pPanelParentNew->getSplitter()->addWidget(this);
				pPanelMoving->resetParent(pPanelParentNew);
				if( direct == PanelTop || direct == PanelLeft )
				{
					pPanelMoving->setPanelDock(direct);
					pPanelParentNew->getSplitter()->insertWidget(0,pPanelMoving);
				}
				else
				{
					pPanelMoving->setPanelDock(PanelFill);
					setPanelDock(pPanelParentNew->getSplitter()->orientation()==Qt::Horizontal ? PanelLeft : PanelTop);
					pPanelParentNew->getSplitter()->addWidget(pPanelMoving);
				}
			}
			else
			{
				pSplitterParent->setOrientation(orien);
				pPanelMoving->resetParent(pPanelParent);

				if( direct == PanelTop || direct == PanelLeft )
				{
					pPanelMoving->setPanelDock(direct);
					int iIndex = pSplitterParent->indexOf(this);
					QList<int> listSizes=pSplitterParent->sizes();
					if(listSizes.size()>iIndex)
					{
						listSizes[iIndex] = listSizes[iIndex]/2;
						listSizes.insert(iIndex,listSizes[iIndex]);
					}
					pSplitterParent->insertWidget(iIndex,pPanelMoving);
					pSplitterParent->setSizes(listSizes);
				}
				else
				{
					if(pSplitterParent->indexOf(this)==(pSplitterParent->count()-1))
					{
						pPanelMoving->setPanelDock(PanelFill);
						setPanelDock(pSplitterParent->orientation()==Qt::Horizontal ? PanelLeft : PanelTop);
					}
					else
						pPanelMoving->setPanelDock(pSplitterParent->orientation()==Qt::Horizontal ? PanelLeft : PanelTop);
					pSplitterParent->insertWidget(pSplitterParent->indexOf(this)+1,pPanelMoving);
				}
			}
		}

		pPanelMoving->setPanelFocus();
	}

//	g_bLoading = false;
//	g_pPanelMain->updatePanel(true);		//����λ�úʹ�С
}

QRect CPanelViewer::getHighlightedRect( const QPoint& ptCurrent )
{
	PanelDirect direct = getDirect(ptCurrent);
	if(!m_pParent->inherits("eGingkoPanelManager::CPanelViewer"))
	{
		//��������ڲ��Ǵ�CPanelViewer�̳е�
		return rect().adjusted(2,m_iTitleHeight+2,-2,-2);
	}
	switch(direct)
	{
	case PanelFill:
		{
			return rect().adjusted(2,m_iTitleHeight+2,-2,-2);
		}
		break;
	case PanelLeft:
		{
			return rect().adjusted(0,m_iTitleHeight,-(rect().width()/2),0);
		}
		break;
	case PanelRight:
		{
			return rect().adjusted(rect().width()/2,m_iTitleHeight,0,0);
		}
		break;
	case PanelTop:
		{
			return rect().adjusted(0,m_iTitleHeight,0,-((rect().height()-m_iTitleHeight)/2));
		}
		break;
	case PanelBottom:
		{
			return rect().adjusted(0,(rect().height()+m_iTitleHeight)/2,0,0);
		}
		break;
	}
	return QRect();
}

CPanelViewer::PanelDirect CPanelViewer::getDirect( const QPoint& ptCurrent )
{
	QPoint ptCenter = rect().center();			//���ĵ�
	QPoint ptSub = ptCenter - ptCurrent;		//�����ĵ�
	if(ptSub.y()==0||rect().height()==0)
	{
		return PanelFill;
	}

	float fDivCurrent = (float)ptSub.x()/(float)ptSub.y();			//��ǰ�Ƕ�
	float fDiv = (float)rect().width()/(float)rect().height();		//����ĽǶ�

	if((ptCurrent - ptCenter).manhattanLength() < ((ptCenter-rect().topLeft()).manhattanLength())/4)
	{
		return PanelFill;
	}
	else
	{
		//�ڲ�
		if(fDivCurrent<fDiv)
		{
			if(ptCenter.y()>ptCurrent.y())
			{
				//top
				return PanelTop;
			}
			else
			{
				//bottom
				return PanelBottom;
			}
		}
		else
		{
			if(ptCenter.x()>ptCurrent.x())
			{
				//left
				return PanelLeft;
			}
			else
			{
				//right
				return PanelRight;
			}
		}
	}
}

void CPanelViewer::mousePressEvent( QMouseEvent * event )
{
	if(event->button() == Qt::LeftButton)
		m_ptMousePress = event->pos();
	else
		m_ptMousePress = QPoint(0xFFFFFF,0xFFFFFF);

	//�������Լ���Panel��δ��ý���ʱ�����ø�Panel��ý���
	if(!(isPanelFocus()||isPanelParentFocus()))
		setPanelFocus();

	if(m_rtClose.contains(event->pos()))
	{
		if(!m_pParent->inherits("eGingkoPanelManager::CPanelViewer"))
		{
			//�����MainWindow��ֱ�ӷ��ء�
			return;
		}
		if( g_pPanelFocusing == this )
		{
			g_pPanelFocusing = 0;
			CPanelProperty::getPanelProperty(m_iViewerType)->setPanelViewer(0);
		}


		if(m_pParent&&m_pParent->inherits("eGingkoPanelManager::CPanelViewer"))
		{
			deleteLater();
			CPanelViewer* pPanelParent = static_cast<CPanelViewer*>(m_pParent);
			if(pPanelParent->getSplitter()->count()<2&&pPanelParent->getViewerType()==GKG_VIEWER_NORMAL)
			{
				if(pPanelParent->getParent()&&pPanelParent->getParent()->inherits("eGingkoPanelManager::CPanelViewer"))
				{
					pPanelParent->deleteLater();
				}
			}
		}

		return;
	}
}

void CPanelViewer::mouseMoveEvent( QMouseEvent * event )
{
	if(g_pPanelFocusing && (m_ptMousePress-event->pos()).manhattanLength()>5)
	{
		//ִ����ק
		QDrag *drag = new QDrag(this);

		//����Panel��ָ����뵽mimeData��
		QMimeData *mimeData = new QMimeData;
		QByteArray itemData;
		QDataStream dataStream(&itemData, QIODevice::WriteOnly);
		dataStream << (uint)g_pPanelFocusing;
		mimeData->setData("panel/move",itemData);

		QPixmap pixmap(g_pPanelFocusing->size());
		g_pPanelFocusing->render(&pixmap);
		drag->setMimeData(mimeData);
		drag->setPixmap(pixmap);
		drag->setHotSpot(event->globalPos()-g_pPanelFocusing->mapToGlobal(QPoint(0,0)));

		Qt::DropAction dropAction = drag->exec(Qt::MoveAction);
	}
}

void CPanelViewer::mouseReleaseEvent( QMouseEvent * event )
{
	if((!m_bFocus)&&(!g_bDraging))
		setPanelFocus();
	g_bDraging = false;
	m_ptMousePress = QPoint(0xFFFFFF,0xFFFFFF);
}

void CPanelViewer::setPanelFocus()
{
	setFocus();
	if(g_pPanelFocusing)
	{
		g_pPanelFocusing->killPanelFocus();
	}
	m_bFocus = true;
	g_pPanelFocusing = this;
	CPanelProperty::getPanelProperty(m_iViewerType)->setPanelViewer(this);
	g_pPanelResource->setCurrentItem(m_pItem);
	update();
}

bool CPanelViewer::isPanelParentFocus()
{
	CPanelViewer* pParent = static_cast<CPanelViewer*>(getParent());
	while(pParent&&pParent->inherits("eGingkoPanelManager::CPanelViewer"))
	{
		if((static_cast<CPanelViewer*>(pParent))->isPanelFocus())
		{
			return true;
		}
		else
		{
			pParent = static_cast<CPanelViewer*>(pParent->getParent());
		}
	}
	return false;
}

void CPanelViewer::resetParent( QWidget* parent )
{
	m_pParent = parent;
	QWidget::setParent(parent);
	if(m_pParent->inherits("eGingkoPanelManager::CPanelViewer"))
	{
		m_pItemParent->removeChild(m_pItem);
		static_cast<CPanelViewer*>(m_pParent)->getTreeItem()->addChild(m_pItem);
		m_pItemParent = static_cast<CPanelViewer*>(m_pParent)->getTreeItem();
	}
}

void CPanelViewer::resizeSplitter()
{
	QList<int> listSize;
	listSize.push_back(-1);
	m_pSplitter->setSizes(listSize);
}

bool CPanelViewer::hasParent( QWidget* pWidget )
{
	CPanelViewer* pParent = this;
	while(pParent&&pParent->inherits("eGingkoPanelManager::CPanelViewer"))
	{
		if(pParent == pWidget)
		{
			return true;
		}
		else
		{
			pParent = static_cast<CPanelViewer*>(pParent->getParent());
		}
	}
	return false;
}

void CPanelViewer::resizeEvent( QResizeEvent* event )
{
	//��С�����ı��Ǹ��������������ʾ�����ݡ�
	updatePanel(false);
	if(isPanelFocus())
		CPanelProperty::getPanelProperty(m_iViewerType)->setPanelViewer(g_pPanelFocusing);

	return QWidget::resizeEvent(event);
}

void CPanelViewer::showEvent( QShowEvent* event )
{
	setPanelFocus();
	return QWidget::showEvent(event);
}

bool CPanelViewer::loadPanelInfo( const QDomElement& elePanel )
{
	if(!elePanel.isElement())
		return false;

	//��ȡ����
	QDomElement eleName = elePanel.firstChildElement("NAME");
	if(eleName.isElement())
		setPanelName(eleName.text());

	//��ȡ����
	QDomElement eleX = elePanel.firstChildElement("X");
	if(eleX.isElement())
		setPanelX(eleX.text().toInt());
	QDomElement eleY = elePanel.firstChildElement("Y");
	if(eleY.isElement())
		setPanelY(eleY.text().toInt());

	//��ȡ���
	QDomElement eleWidth = elePanel.firstChildElement("WIDTH");
	if(eleWidth.isElement())
		setPanelWidth(eleWidth.text().toInt());
	QDomElement eleHeight = elePanel.firstChildElement("HEIGHT");
	if(eleHeight.isElement())
		setPanelHeight(eleHeight.text().toInt());

	QDomElement eleDock = elePanel.firstChildElement("DOCK");
	if(eleDock.isElement())
		setPanelDock((PanelDirect)eleDock.text().toInt());

	//��ʾ�ؼ�����
	QDomElement eleViewerType = elePanel.firstChildElement("VIEWTYPE");
	if(eleViewerType.isElement())
		m_iViewerType = (GKG_VIEWERTYPE)eleViewerType.text().toInt();

	//������Panel
	QDomElement eleChild = elePanel.lastChildElement("PANEL");
	while(eleChild.isElement())
	{
		int iType = GKG_VIEWER_NORMAL;
		QDomElement eleChildType = eleChild.firstChildElement("VIEWTYPE");
		if(eleChildType.isElement())
		{
			bool bOK;
			iType = eleChildType.text().toInt(&bOK);
			if(!bOK)
				iType = GKG_VIEWER_NORMAL;
		}
		CPanelViewer* pPanelChild = CPanelViewer::createPanelViewer(iType,m_pItem,this);
		m_pSplitter->addWidget(pPanelChild);
		pPanelChild->loadPanelInfo(eleChild);
		eleChild = eleChild.previousSiblingElement("PANEL");
	}

	return true;
}

bool CPanelViewer::savePanelInfo( QDomDocument& doc,QDomElement& elePanel )
{
	//����Panel��Ϣ
	if(!elePanel.isElement())
		return false;

	//��ȡ����
	QDomElement eleName = doc.createElement("NAME");
	eleName.appendChild(doc.createTextNode(getPanelName()));
	elePanel.appendChild(eleName);

	//��ȡ����
	QDomElement eleX = doc.createElement("X");
	eleX.appendChild(doc.createTextNode(QString("%1").arg(getPanelX())));
	elePanel.appendChild(eleX);
	QDomElement eleY = doc.createElement("Y");
	eleY.appendChild(doc.createTextNode(QString("%1").arg(getPanelY())));
	elePanel.appendChild(eleY);

	//��ȡ���
	QDomElement eleWidth = doc.createElement("WIDTH");
	eleWidth.appendChild(doc.createTextNode(QString("%1").arg(getPanelWidth())));
	elePanel.appendChild(eleWidth);
	QDomElement eleHeight = doc.createElement("HEIGHT");
	eleHeight.appendChild(doc.createTextNode(QString("%1").arg(getPanelHeight())));
	elePanel.appendChild(eleHeight);

	QDomElement eleDock = doc.createElement("DOCK");
	eleDock.appendChild(doc.createTextNode(QString("%1").arg(getPanelDock())));
	elePanel.appendChild(eleDock);


	//������Panel
	QList<CPanelViewer*> listPanel = getChildren();
	if(listPanel.size()<1)
	{
		//û����Panel
		QDomElement eleViewerType = doc.createElement("VIEWTYPE");
		eleViewerType.appendChild(doc.createTextNode(QString("%1").arg(m_iViewerType)));
		elePanel.appendChild(eleViewerType);
	}
	else
	{
		//������Panel
		foreach(CPanelViewer* pChild,listPanel)
		{
			static int iSplitterCount = 1;			//Splitter�ĸ���

			QDomElement eleChild = doc.createElement("PANEL");
			elePanel.appendChild(eleChild);
			pChild->savePanelInfo(doc,eleChild);
			if(pChild!=listPanel.last())
			{
				//����Splitter��
				QDomElement eleSplitter = doc.createElement("SPLITTER");
				elePanel.appendChild(eleSplitter);

				QDomElement eleSplitterName = doc.createElement("NAME");
				eleSplitterName.appendChild(doc.createTextNode(QString("Splitter%1").arg(iSplitterCount)));
				eleSplitter.appendChild(eleSplitterName);

				QDomElement eleSplitterDock = doc.createElement("DOCK");
				eleSplitterDock.appendChild(doc.createTextNode(m_pSplitter->orientation()==Qt::Horizontal ? QString("3") : QString("1")));			//��������������������
				eleSplitter.appendChild(eleSplitterDock);

				++iSplitterCount;
			}
		}
	}


	return true;
}

QList<CPanelViewer*> CPanelViewer::getChildren()
{
	QList<CPanelViewer*> listChild;		//��ȡ���е���panel
	for(int i = 0; i < m_pSplitter->count(); ++i)
	{
		CPanelViewer* pPanel = static_cast<CPanelViewer*>(m_pSplitter->widget(i));
		listChild.push_front(pPanel);
	}

	return listChild;
}

bool CPanelViewer::clearChildren()
{
	while(m_pSplitter->count()>0)
	{
		CPanelViewer* pPanel = static_cast<CPanelViewer*>(m_pSplitter->widget(0));
		if(pPanel)
			delete pPanel;
	}

	return true;
}

void CPanelViewer::updatePanel( bool bSave /*= false*/ )
{
	if(bSave)
	{
		//���µ�����
		///////////////////////////
		QList<CPanelViewer*> listChild = getChildren();		//��ȡ���е���panel
		QList<int> listSize;								//���ڴ��������Panel�Ĵ�С

		QMap<int,CPanelViewer*> mapPanel;					//���ڶ���Panel����λ�ý�������
		foreach(CPanelViewer* pChild,listChild)
		{
			pChild->updatePanel(true);						//�ȸ�����Panel

			if(pChild->getPanelDock() == PanelLeft)
				m_pSplitter->setOrientation(Qt::Horizontal);
			else if(pChild->getPanelDock() == PanelTop)
				m_pSplitter->setOrientation(Qt::Vertical);

			mapPanel.insert(pChild->getPositon(),pChild);
			listSize.push_back(pChild->getSize());
		}

		m_pSplitter->setSizes(listSize);										//���ñ���
	}
	else if(!g_bLoading)
	{
		//������ڼ�����ʱ�����Ա������и���
		//�ӽ�����µ�����
		if(m_pParent&&m_pParent->inherits("eGingkoPanelManager::CPanelViewer"))
		{
			CPanelViewer* pPanelParent = static_cast<CPanelViewer*>(m_pParent);
//			QSplitter* pSplitterParent = pPanelParent->getSplitter();
			m_iX =  m_pParent->width()>0 ? 
				(mapToParent(rect().topLeft()).x()*100)/(m_pParent->width()) : 0;

			m_iY = m_pParent->height()>0 ? 
				((mapToParent(rect().topLeft()).y()*100)/(m_pParent->height())) : 0;

			int iParentWidth = pPanelParent->width();
			int iParentHeight = pPanelParent->height();
			if(pPanelParent->getSplitter()->orientation() == Qt::Horizontal)
				iParentWidth = iParentWidth-pPanelParent->getChildrenCount()*2+2;
			else
				iParentHeight = iParentHeight-pPanelParent->getChildrenCount()*2+2;

			m_iWidth = m_pParent->width()>0 ?
				(float((rect().width()*100)/float(iParentWidth)+0.5)) : 0;

			m_iHeight = m_pParent->height()>0 ?
				(float((rect().height()*100)/float(iParentHeight)+0.5)) : 0;
		}
		else if(m_pParent)
		{
			//��Panel
			m_iX = 0;
			m_iY = 0;
			m_iHeight =100;
			m_iWidth = 100;
		}
	}
}