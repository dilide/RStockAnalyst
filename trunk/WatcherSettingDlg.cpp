#include "StdAfx.h"
#include "WatcherSettingDlg.h"
#include "RadarWatcher.h"
#include "BlockInfoItem.h"
#include "WatcherEditDlg.h"


CWatcherSettingDlg::CWatcherSettingDlg( QWidget* parent /*= NULL*/ )
	: QDialog(parent)
	, m_bForSel(false)
	, m_bInit(true)
{
	initDlg();
}

CWatcherSettingDlg::CWatcherSettingDlg( const QList<int>& listUsed,QWidget* parent/*=NULL*/ )
	: QDialog(parent)
	, m_bForSel(true)
	, m_listSel(listUsed)
	, m_bInit(true)
{
	initDlg();
}

void CWatcherSettingDlg::initDlg()
{
	setMinimumSize(500,200);
	setWindowTitle(tr("��ǰ�����״�"));
	m_btnAdd.setText(tr("����"));
	m_btnDel.setText(tr("ɾ��"));
	m_btnEdit.setText(tr("�༭"));
	m_btnRefresh.setText(tr("ˢ��"));

	connect(&m_btnAdd,SIGNAL(clicked()),this,SLOT(onAddWatcher()));
	connect(&m_btnDel,SIGNAL(clicked()),this,SLOT(onDelWatcher()));
	connect(&m_btnEdit,SIGNAL(clicked()),this,SLOT(onEditWatcher()));
	connect(&m_btnRefresh,SIGNAL(clicked()),this,SLOT(onRefresh()));

	m_listWatchers.setHeaderLabels(QStringList()<<tr("ID")<<tr("����")<<tr("��ֵ")<<tr("���ʱ��")<<tr("��ǰ���")<<tr("Ŀ����"));
	m_listWatchers.setSelectionMode(QTreeWidget::SingleSelection);
	connect(&m_listWatchers,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(onEditWatcher()));

	QGridLayout* pLayout = new QGridLayout(this);
	pLayout->addWidget(&m_listWatchers,0,0,8,8);
	pLayout->addWidget(&m_btnAdd,9,0,2,2);
	pLayout->addWidget(&m_btnDel,9,2,2,2);
	pLayout->addWidget(&m_btnEdit,9,4,2,2);
	pLayout->addWidget(&m_btnRefresh,9,6,2,2);

	setLayout(pLayout);

	onRefresh();
}


CWatcherSettingDlg::~CWatcherSettingDlg(void)
{
}

QList<int> CWatcherSettingDlg::getSelIDs()
{
	QList<int> listIDs;
	int iCount = m_listWatchers.topLevelItemCount();
	for (int i=0;i<iCount;++i)
	{
		QTreeWidgetItem* pItem = m_listWatchers.topLevelItem(i);
		if(Qt::Checked == pItem->checkState(0))
		{
			CRadarWatcher* pWatcher = reinterpret_cast<CRadarWatcher*>(pItem->data(0,Qt::UserRole).toUInt());
			if(pWatcher)
			{
				listIDs.push_back(pWatcher->getId());
			}
		}
	}

	return listIDs;
}


void CWatcherSettingDlg::onRefresh()
{
	if(!m_bInit)
	{
		m_listSel = getSelIDs();
	}
	m_listWatchers.clear();
	QList<CRadarWatcher*> list = CRadarManager::getRadarManager()->getRadarWatchers();
	foreach(CRadarWatcher* pWatcher,list)
	{
		CBlockInfoItem* pDestBlock = pWatcher->getDestBlock();
		CBlockInfoItem* pWatcherBlock = pWatcher->getBlock();
		QStringList listValue;
		listValue<<QString("%1").arg(pWatcher->getId())
			<<QString("%1").arg(CRadarManager::getTypeName(pWatcher->getType()))
			<<QString("%1").arg(pWatcher->getHold())
			<<QString("%1").arg(pWatcher->getSec())
			<<(pWatcherBlock ? pWatcherBlock->getName() : "NULL")
			<<(pDestBlock ? pDestBlock->getName() : "NULL");
		QTreeWidgetItem* pItem = new QTreeWidgetItem(listValue);
		if(m_bForSel)
		{
			if(m_listSel.contains(pWatcher->getId()))
				pItem->setCheckState(0,Qt::Checked);
			else
				pItem->setCheckState(0,Qt::Unchecked);
		}
		pItem->setData(0,Qt::UserRole,reinterpret_cast<uint>(pWatcher));
		m_listWatchers.addTopLevelItem(pItem);
	}

	m_bInit = false;
}

void CWatcherSettingDlg::onAddWatcher()
{
	CWatcherEditDlg dlg(0,this);
	if(QDialog::Accepted == dlg.exec())
		onRefresh();
}

void CWatcherSettingDlg::onDelWatcher()
{
	QList<QTreeWidgetItem*> listSel = m_listWatchers.selectedItems();
	if(listSel.size()>0)
	{
		CRadarWatcher* pWatcher = reinterpret_cast<CRadarWatcher*>(listSel[0]->data(0,Qt::UserRole).toUInt());
		if(QMessageBox::Yes == QMessageBox::warning(this,tr("����"),
			tr("ȷ��Ҫɾ��ѡ��ļ����״���"),QMessageBox::Yes|QMessageBox::No))
		{
			CRadarManager::getRadarManager()->removeWatcher(pWatcher->getId());
			onRefresh();
		}
	}
	else
	{
		QMessageBox::information(this,tr("��Ϣ"),tr("û��ѡ��Ҫɾ������Ϣ��"));
	}
}

void CWatcherSettingDlg::onEditWatcher()
{
	QList<QTreeWidgetItem*> listSel = m_listWatchers.selectedItems();
	if(listSel.size()>0)
	{
		CRadarWatcher* pWatcher = reinterpret_cast<CRadarWatcher*>(listSel[0]->data(0,Qt::UserRole).toUInt());
		CWatcherEditDlg dlg(pWatcher,this);
		if(QDialog::Accepted == dlg.exec())
			onRefresh();
	}
	else
	{
		QMessageBox::information(this,tr("��Ϣ"),tr("û��ѡ��Ҫ�༭����Ϣ��"));
	}
}
