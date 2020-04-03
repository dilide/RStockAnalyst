#include <QApplication>
#include <iostream>
#include <QtGui>
#include "STKDRV.h"
#include "MainWindow.h"
#include "DataEngine.h"
#include "ColorManager.h"
#include "KLineWidget.h"
#include "SplashDlg.h"
#include "RadarWatcher.h"

luaL_Reg* g_pFuncs = 0;			//������Ҫע���lua����
QString g_native = "";			//luaԴ��
QVector<HINSTANCE> g_vDll;		//���صĺ�����չ��̬���ӿ�

int loadAllFunc()
{
	int iDllCount = 0;
	QMap<const char*,lua_CFunction> g_func;

	QDir dirFunc(qApp->applicationDirPath() + "/plugin/");
	QFileInfoList list = dirFunc.entryInfoList(QStringList()<<"*.dll");
	foreach (const QFileInfo& v,list)
	{
		HINSTANCE hDll = LoadLibrary(v.absoluteFilePath().toStdWString().data());
		if(hDll)
		{
			g_vDll.push_back(hDll);
			qDebug()<<"Load form "<<v.absoluteFilePath();
			QMap<const char*,lua_CFunction>* (WINAPI* pfnALlFuncs)()= NULL;
			pfnALlFuncs = \
				(QMap<const char*,lua_CFunction>*(WINAPI *)()) GetProcAddress(hDll,"ExportAllFuncs");
			if(pfnALlFuncs)
			{
				QMap<const char*,lua_CFunction>* _funcs = (*pfnALlFuncs)();

				g_func.unite(*_funcs);
				qDebug()<<"Load funcs from\""<<v.baseName()<<"\":";
				delete _funcs;
			}
			else
			{
				qDebug()<<"Load Dll Error!";
			}
			//FreeLibrary(hDll);
		}
	}

	QMap<const char*,lua_CFunction>::iterator iter = g_func.begin();
	g_pFuncs= new luaL_Reg[g_func.size()+1];
	luaL_Reg* pFunc = g_pFuncs;
	qDebug()<<"---------Funcs-----------";
	while(iter!=g_func.end())
	{
		qDebug()<<iter.key();
		pFunc->name = iter.key();
		pFunc->func = iter.value();
		//			lua_register(m_pL,iter.key().toAscii(),iter.value());
		++iter;
		++pFunc;
	}
	pFunc->name = NULL;
	pFunc->func = NULL;
	return iDllCount;
}

void OnShowSplashMessage(const QString& msg,int iPro)
{
	CSplashDlg::getSplashDlg()->showMessage(msg,iPro);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	{
		//����lua��غ���
		QFile file(qApp->applicationDirPath()+"/native.lua");
		if(file.open(QFile::ReadOnly))
		{
			g_native = file.readAll();
		}
		loadAllFunc();
	}

	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));

	CSplashDlg::getSplashDlg()->show();

	CSplashDlg::getSplashDlg()->showMessage(QObject::tr("������ʽ��"),10);
	QFile file(":/res/qss/FeiHu.qss");
	if(file.open(QFile::ReadOnly))
	{
		QString qsStyleSheet = file.readAll();
		app.setStyleSheet(qsStyleSheet);
	}

	//��ʼ����ɫ��
	CColorManager::initAll();

	CSplashDlg::getSplashDlg()->showMessage(QObject::tr("��װ����"),20);
	CSTKDRV::InitStockDrv();

	CMainWindow::getMainWindow()->hide();


	//������Ϣ���ȴ��ں���������
	CDataEngine::messageShowed = &OnShowSplashMessage;


	CSplashDlg::getSplashDlg()->showMessage(QObject::tr("��ʼ������..."),40);
	app.processEvents();
	CDataEngine::importData();				//��ʼ������


	//��ʼ��������
	CMainWindow::getMainWindow()->initTemplates();	//��ʼ��ģ��
	CMainWindow::getMainWindow()->initBlockMenus();	//��ʼ�����˵�

	CSplashDlg::getSplashDlg()->getSplashDlg()->showMessage(QObject::tr("��������..."),99);
	CMainWindow::getMainWindow()->showMaximized();

	//�����������ݽӿ�
	CSplashDlg::getSplashDlg()->showMessage(QObject::tr("�����������ݽӿ�"),30);
	app.processEvents();
	if(!CMainWindow::getMainWindow()->setupStockDrv())
	{
		return app.exit();
	}

	CSplashDlg::getSplashDlg()->hide();

	app.exec();

	CSplashDlg::getSplashDlg()->show();

	CSplashDlg::getSplashDlg()->showMessage(QObject::tr("���ڱ�������..."),10);
	CMainWindow::getMainWindow()->saveTemplates();	//��������ģ��

	
	CSplashDlg::getSplashDlg()->showMessage(QObject::tr("���ڵ�������..."),20);

	CDataEngine::exportData();				//��������

	CSplashDlg::getSplashDlg()->showMessage(QObject::tr("�����������..."),99);

	//���̵�������
	//CDataEngine::exportCloseData();

	//�ͷ���Դ
	delete CMainWindow::getMainWindow();
	CDataEngine::releaseDataEngine();

	foreach(HINSTANCE hDll,g_vDll)
	{
		FreeLibrary(hDll);
	}

	return 1;
}
