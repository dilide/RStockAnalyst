/*
============================================================================
�ļ�����	:	MainWindow.cpp
����		:	���ǿ�
����ʱ��	:	2011-12-29 16:56
�޸�ʱ��	:	2011-12-29 16:57
˵��		:	����������ڡ�
============================================================================
*/

#include "common.h"
#include "MainWindow.h"
#include "ConfigSettings.h"
#include "GValuesWidget.h"

using namespace eGingkoPanelManager;
using namespace eGingkoCommon;

CMainWindow* CMainWindow::m_pMainWindow = 0;
CMainWindow* CMainWindow::getMainWindow()
{
	if(!m_pMainWindow)
		m_pMainWindow = new CMainWindow();
	return m_pMainWindow;
}

CMainWindow::CMainWindow( QWidget* parent /*= NULL*/ )
	: QMainWindow(parent)
	, m_pMainWidget(0)
{
	//����panel��������ʾͼƬ
	//QImage* pPixmapCT = new QImage(QString("./images/CT.png"));
	//QImage* pPixmapPET = new QImage(QString("./images/PET.png"));
	//QImage* pPixmapPET_CT = new QImage(QString("./images/PETCT.png"));
	//CPanelViewer::g_mapPixmapPanel[GKG_MOD_CT] = pPixmapCT;
	//CPanelViewer::g_mapPixmapPanel[GKG_MOD_PET] = pPixmapPET;
	//CPanelViewer::g_mapPixmapPanel[GKG_MOD_PETCT] = pPixmapPET_CT;

	CPanelViewer::g_mapPixmapPanel[GKG_VIEWER_NORMAL] = new QImage("./images/panel.png");
	CPanelViewer::g_mapPixmapPanel[GKG_VIEWER_ROI] = new QImage("./images/roi.png");
	CPanelViewer::g_mapPixmapPanel[GKG_VIEWER_RECONS] = new QImage("./images/recons.png");
	CPanelViewer::g_mapPixmapPanel[GKG_VIEWER_REFORMS] = new QImage("./images/reforms.png");
	CPanelViewer::g_mapPixmapPanel[GKG_VIEWER_BullsEye] = new QImage("./images/bullseye.png");
	CPanelViewer::g_mapPixmapPanel[GKG_VIEWER_IMAGE] = new QImage("./images/image.png");

	//��ʼ��panel�ؼ�
	m_pListPanels = new CListPanel(this);
	m_pDockListViewers = new QDockWidget("Panel",this);
	m_pDockListViewers->setWidget(m_pListPanels);
	m_pDockListViewers->setFloating(false);
	m_pDockListViewers->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
	m_pDockListViewers->setMinimumHeight(180);

	//��ʼ�����Դ���
	m_pDockProperty = new QDockWidget(tr("Property"),this);
	m_pDockProperty->setFloating(false);
	m_pDockProperty->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
	m_pDockProperty->setMinimumHeight(150);

	//��ʼ��ȫ�ִ�����Դ
	//��ʼ��TreeWidget�ؼ�
	m_pPanelResource = new CPanelResource(this);
	CPanelViewer::g_pPanelResource = m_pPanelResource;		//��ֵ

	m_pDockResource = new QDockWidget(tr("Panel Resource"),this);
	m_pDockResource->setWidget(m_pPanelResource);
	m_pDockResource->setFloating(false);
	m_pDockResource->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
	m_pDockResource->setMinimumHeight(300);

	//��ʼ��ȫ������Dock
	m_pDockSettings = new QDockWidget(tr("Global Settings"),this);
	QWidget* pWidgetSettings = new QWidget(m_pDockSettings);
	QGridLayout* pLayoutSettings = new QGridLayout(pWidgetSettings);
	m_pEditName = new QLineEdit(tr("Static Brain demo"));
	m_pComboClinic = new QComboBox();
	m_pComboClinic->addItem(tr("Undefine"),GKG_CNC_NULL);		//δ����
	m_pComboClinic->addItem(tr("STBRAIN"),GKG_CNC_SBRN);		//��̬����
	m_pComboClinic->addItem(tr("CARDIAC"),GKG_CNC_PCRD);		//�ļ���л
	m_pComboClinic->addItem(tr("STWB"),GKG_CNC_SWBD);			//��̬ȫ��
	m_pComboClinic->setCurrentIndex(1);

	pLayoutSettings->addWidget(new QLabel(tr("Name:")),0,0,1,1);
	pLayoutSettings->addWidget(m_pEditName,0,1,1,1);

	pLayoutSettings->addWidget(new QLabel(tr("Clinic:")),1,0,1,1);
	pLayoutSettings->addWidget(m_pComboClinic,1,1,1,1);

	pLayoutSettings->addWidget(CGValuesWidget::getGValuesWidget(),2,0,1,2);

	pWidgetSettings->setLayout(pLayoutSettings);
	m_pDockSettings->setWidget(pWidgetSettings);

	addDockWidget(Qt::LeftDockWidgetArea,m_pDockListViewers);
	addDockWidget(Qt::LeftDockWidgetArea,m_pDockProperty);
	addDockWidget(Qt::LeftDockWidgetArea,m_pDockResource);
	addDockWidget(Qt::LeftDockWidgetArea,m_pDockSettings);

	//��ʼ��Menu Bar
	m_pMenuBar = new QMenuBar(this);
	//����ļ�����
	QMenu* pFileMenu = m_pMenuBar->addMenu(tr("File(&F)"));
	pFileMenu->addAction(tr("Open(&O)"),this,SLOT(OnLoadPanelFile()));
	pFileMenu->addAction(tr("Save(&S)"),this,SLOT(OnSavePanelFile()));
	//��ӱ༭����
	QMenu* pEditMenu = m_pMenuBar->addMenu(tr("Edit(&E)"));
	pEditMenu->addAction(tr("Clear(&C)"),this,SLOT(OnClearPanels()));
	//�����ͼ����
	QMenu* pViewMenu = m_pMenuBar->addMenu(tr("View(&V)"));
	{
		QList<QAction*> listAction = createPopupMenu()->actions();
		foreach(QAction* pAct, listAction)
		{
			pViewMenu->addAction(pAct);
		}
	}
	setMenuBar(m_pMenuBar);

	m_pViewWidget = new QWidget(this);
	m_pViewWidget->setMinimumWidth(400);
	setCentralWidget(m_pViewWidget);
}

CMainWindow::~CMainWindow(void)
{
	//����������ϵ���Դ
	delete m_pMainWidget;
	delete m_pMenuBar;
	delete m_pPanelResource;
	delete m_pListPanels;
}

void CMainWindow::setPropertyWidget( QWidget* widget )
{
	m_pDockProperty->setWidget(widget);
}

void CMainWindow::showEvent( QShowEvent* event )
{
	//central widget
	if(!m_pMainWidget)
	{
		QVBoxLayout* pLayout = new QVBoxLayout(m_pViewWidget);
		m_pMainWidget = CPanelViewer::createPanelViewer(GKG_VIEWER_NORMAL,0,m_pViewWidget);
		pLayout->addWidget(m_pMainWidget);
		m_pViewWidget->setLayout(pLayout);
	}
}

void CMainWindow::OnLoadPanelFile()
{
	QString qsFileName = QFileDialog::getOpenFileName(this,
		tr("Please select a xml file:"),CConfigSettings::getConfigSettings()->getValue("LastPath").toString(),
		"Xml File(*.xml)");

	//��ȡxml�ļ��е���Ϣ
	QFile file(qsFileName);
	if(!file.open(QFile::ReadOnly))
	{
		return;
	}

	//���浱ǰ�����λ�õ������ļ�
	CConfigSettings::getConfigSettings()->setValue("LastPath",QFileInfo(qsFileName).absolutePath());

	m_pMainWidget->clearChildren();
//	m_pCentralWidget = new CPanelViewer(0,this);
//	setCentralWidget(m_pCentralWidget);

	//
	CPanelViewer::g_bLoading = true;		//����Ϊ�����С�������
	QDomDocument doc;
	doc.setContent(file.readAll());
	QDomElement eleRoot = doc.firstChildElement("ROOT");
	QDomElement eleSetting,elePanel;
	if(!eleRoot.isNull())
	{
		eleSetting = eleRoot.firstChildElement("Settings");
		if(!eleSetting.isNull())
		{
			m_pEditName->setText(eleSetting.attribute("name",tr("Static Brain demo")));
			m_pComboClinic->setCurrentIndex(eleSetting.attribute("clinic","0").toInt());

			QDomElement eleValues = eleSetting.firstChildElement("Values");
			if(!eleValues.isNull())
			{
				QDomNodeList listNodes = eleValues.childNodes();
				for(int i=0; i<listNodes.size(); ++i)
				{
					QDomNode node = listNodes.at(i);
					QDomText nodeText;
					if(node.hasChildNodes())
					{
						nodeText= node.firstChild().toText();
						CGValuesWidget::getGValuesWidget()->updateGlobalValues(node.nodeName(),nodeText.data().toDouble());
					}
				}
			}

		}
		elePanel = eleRoot.firstChildElement("PANEL");
	}
	else
	{
		elePanel = doc.firstChildElement("PANEL");
	}

	m_pMainWidget->loadPanelInfo(elePanel);
	m_pMainWidget->updatePanel(true);
	m_pMainWidget->setPanelFocus();
	CGValuesWidget::getGValuesWidget()->updateGlobalValues();
	CPanelViewer::g_bLoading = false;		//����Ϊ�Ǽ����С�����
}

void CMainWindow::OnSavePanelFile()
{
	QString qsFileName = QFileDialog::getSaveFileName(this,
		tr("Please input a file name:"),CConfigSettings::getConfigSettings()->getValue("LastPath").toString(),
		"Xml File(*.xml)");

	if(qsFileName.isEmpty())
		return;

	QFile file(qsFileName);
	if(!file.open(QFile::WriteOnly))
		return;

	//���浱ǰ�����λ�õ������ļ�
	CConfigSettings::getConfigSettings()->setValue("LastPath",QFileInfo(qsFileName).absolutePath());

	QDomDocument doc;
	doc.appendChild(doc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"utf-8\"" ));

	QDomElement eleRoot = doc.createElement("ROOT");
	QDomElement eleSettings = doc.createElement("Settings");
	eleSettings.setAttribute("name",m_pEditName->text());
	eleSettings.setAttribute("clinic",m_pComboClinic->currentIndex());

	QDomElement eleValues = doc.createElement("Values");
	eleSettings.appendChild(eleValues);
	QMap<QString,double> mapValues = CGValuesWidget::getGValuesWidget()->getValues();
	for(QMap<QString,double>::iterator iter = mapValues.begin(); iter!=mapValues.end(); ++iter)
	{
		QDomElement eleValue = doc.createElement(iter.key());
		eleValue.appendChild(doc.createTextNode(QString("%1").arg(iter.value())));
		eleValues.appendChild(eleValue);
	}


	doc.appendChild(eleRoot);
	eleRoot.appendChild(eleSettings);

	QDomElement elePanel = doc.createElement("PANEL");
	eleRoot.appendChild(elePanel);
	m_pMainWidget->savePanelInfo(doc,elePanel);

	QPixmap pixmap(m_pMainWidget->size());
	m_pMainWidget->render(&pixmap);
	pixmap.save(qsFileName.left(qsFileName.length()-3)+"jpg");
	
	file.write(doc.toByteArray());
}
