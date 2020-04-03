#include "StdAfx.h"
#include "AbnomalSettingDlg.h"

bool CAbnomalSettingDlg::m_bInit = false;
QMap<RAbnomalType,RAbnomalValue*> CAbnomalSettingDlg::m_mapAbnomals;


void CAbnomalSettingDlg::initAbnomals()
{
	m_mapAbnomals[HighIncrease] = new RAbnomalValue(HighIncrease,AbnomalGreater,"�Ƿ�>=",(float)0.05);
	m_mapAbnomals[LowIncrease] = new RAbnomalValue(LowIncrease,AbnomalLesser,"�Ƿ�<=",(float)-0.05);
	m_mapAbnomals[HighTurnRatio] = new RAbnomalValue(HighTurnRatio,AbnomalGreater,"������>=",(float)0.05);
	m_mapAbnomals[HighVolumeRatio] = new RAbnomalValue(HighVolumeRatio,AbnomalGreater,"����>=",(float)0.05);
	m_mapAbnomals[HighAvgVolume] = new RAbnomalValue(HighAvgVolume,AbnomalGreater,"����>=",(float)0.05);
	m_mapAbnomals[HighPrice] = new RAbnomalValue(HighPrice,AbnomalGreater,"�ɼ�>=",(float)0.05);
	m_mapAbnomals[LowPrice] = new RAbnomalValue(LowPrice,AbnomalLesser,"�ɼ�<=",(float)0.05);
	m_mapAbnomals[HighLTSZ] = new RAbnomalValue(HighLTSZ,AbnomalGreater,"��ͨ��ֵ>=",(float)0.05);
	m_mapAbnomals[LowLTSZ] = new RAbnomalValue(LowLTSZ,AbnomalLesser,"��ͨ��ֵ<=",(float)0.05);
	m_mapAbnomals[HighMGSY] = new RAbnomalValue(HighMGSY,AbnomalGreater,"ÿ������>=",(float)0.05);
	m_mapAbnomals[HighPERatio] = new RAbnomalValue(HighPERatio,AbnomalGreater,"��ӯ��>=",(float)0.05);
	m_mapAbnomals[HighZYYWRatio] = new RAbnomalValue(HighZYYWRatio,AbnomalGreater,"��Ӫҵ��������>=",(float)0.05);
	m_mapAbnomals[HighYJYZ] = new RAbnomalValue(HighYJYZ,AbnomalGreater,"ҵ��Ԥ��>=",(float)0.05);
	m_mapAbnomals[HighYJZZ] = new RAbnomalValue(HighYJZZ,AbnomalGreater,"ҵ������>=",(float)0.05);
	m_mapAbnomals[HighYJHBZZ] = new RAbnomalValue(HighYJHBZZ,AbnomalGreater,"ҵ����������>=",(float)0.05);
	m_mapAbnomals[HighJZCSYL] = new RAbnomalValue(HighJZCSYL,AbnomalGreater,"���ʲ�������>=",(float)0.05);
	m_mapAbnomals[HighBuyVolume] = new RAbnomalValue(HighBuyVolume,AbnomalGreater,"ί��1+2+3>=",(float)0.05);
	m_mapAbnomals[HighSellVolume] = new RAbnomalValue(HighSellVolume,AbnomalGreater,"ί��1+2+3>=",(float)0.05);
	m_mapAbnomals[HighCommRatio] = new RAbnomalValue(HighCommRatio,AbnomalGreater,"ί��>=",(float)0.05);
	m_bInit = true;
}

void CAbnomalSettingDlg::ShowAbnomalDlg( QMap<RAbnomalType,float>& map,QWidget* parent /*= 0*/ )
{
	if(!m_bInit)
	{
		initAbnomals();
	}

	CAbnomalSettingDlg dlg(map,parent);
	if(QDialog::Accepted == dlg.exec())
	{
		map.clear();
		map = dlg.getAbnomalMap();
	}
}

CAbnomalSettingDlg::CAbnomalSettingDlg( const QMap<RAbnomalType,float>& map,QWidget* parent /*= 0*/ )
	: QDialog(parent)
	, m_mapAbnomal(map)
{
	initDlg();
}


CAbnomalSettingDlg::~CAbnomalSettingDlg(void)
{
}


void CAbnomalSettingDlg::initDlg()
{
	QGridLayout* pLayout = new QGridLayout(this);
	int iRow = 0;
	QMap<RAbnomalType,RAbnomalValue*>::iterator iter = m_mapAbnomals.begin();
	while(iter!=m_mapAbnomals.end())
	{
		QCheckBox* pCheckBox = new QCheckBox(this);
		QLineEdit* pLineEdit = new QLineEdit(this);

		pCheckBox->setText((*iter)->_Desc);
		pLineEdit->setValidator(new QDoubleValidator());
		pLayout->addWidget(pCheckBox,iRow,0);
		pLayout->addWidget(pLineEdit,iRow,1);

		if(m_mapAbnomal.contains(iter.key()))
		{
			pCheckBox->setCheckState(Qt::Checked);
			pLineEdit->setText(QString("%1").arg(m_mapAbnomal[iter.key()]));
		}
		else
		{
			pLineEdit->setText(QString("%1").arg((*iter)->_Default));
		}

		m_mapCtrls[iter.key()] = QPair<QLineEdit*,QCheckBox*>(pLineEdit,pCheckBox);

		++iRow;
		++iter;
	}


	QPushButton* pBtnOk = new QPushButton(tr("ȷ��"),this);
	QPushButton* pBtnCancel = new QPushButton(tr("ȡ��"),this);
	connect(pBtnOk,SIGNAL(clicked()),this,SLOT(onBtnOk()));
	connect(pBtnCancel,SIGNAL(clicked()),this,SLOT(onBtnCancel()));

	QHBoxLayout* pBtnLayout = new QHBoxLayout(this);
	pBtnLayout->addWidget(pBtnOk,1);
	pBtnLayout->addWidget(pBtnCancel,1);

	pLayout->addLayout(pBtnLayout,iRow,0,1,2);
	setLayout(pLayout);
}

void CAbnomalSettingDlg::onBtnOk()
{

	m_mapAbnomal.clear();
	QMap<RAbnomalType,QPair<QLineEdit*,QCheckBox*>>::iterator iter = m_mapCtrls.begin();
	while(iter!=m_mapCtrls.end())
	{
		if(iter->second->isChecked())
		{
			m_mapAbnomal[iter.key()]=iter->first->text().toFloat();
		}

		++iter;
	}
	accept();
}

void CAbnomalSettingDlg::onBtnCancel()
{
	reject();
}

QMap<RAbnomalType,float> CAbnomalSettingDlg::getAbnomalMap()
{
	return m_mapAbnomal;
}

