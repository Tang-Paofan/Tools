#include <UP3D.ProductionProcessToolingUI/DeviceDiagnostics.h>

#include "StartUtil.h"

#include <QApplication>
#include <QSystemSemaphore>
#include <QSharedMemory>

#include <QSettings>
#include <QCoreApplication>
#include <QDir>

#include <Windows.h>


QSystemSemaphore gAppSema("UPDeviceDiahnostics", 1, QSystemSemaphore::Open);
QSharedMemory gAppSharedMem_("UPDeviceDiahnostics-Sys");

int main(int argc, char** argv)
{
	gAppSema.acquire();
	if (gAppSema.error() != QSystemSemaphore::NoError)
	{
		/// ��ȡ�ź���ʧ��, ֱ���˳�
		return EXIT_FAILURE;
	}

	// ��ʼ�����ű���Ҫ����Application��ʼ��֮ǰ
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
	
	// ��Ӳ��·��
	QApplication::addLibraryPath("./plugins");

	// ����Application
	QApplication app(argc, argv);
	QDir::setCurrent(app.applicationDirPath() + "/");

	// ����QSS
	StartUtil::loadQss(qApp->applicationDirPath() + "/Resource/Skin");

	// ��������
	StartUtil::initFont(qApp->applicationDirPath() + "/Resource/FontFamily");

	// ����������̬��
	QSettings settings("HKEY_CURRENT_USER\\Software\\UP3D\\Intraoral", QSettings::NativeFormat);
	QString oralInstallPath = settings.value("Path").toString();
	QString path = oralInstallPath + ";" + qgetenv("PATH");
	qputenv("PATH", path.toLocal8Bit());

	// ����������
	DeviceDiagnostics* wgt = new DeviceDiagnostics();
	wgt->show();

	// ���ز��
	StartUtil::loadPlugin(wgt);

	wgt->defaultSelectItem();
	wgt->expandAll();

#if SHOW_TEST
	{
		mainWindow->addNode(u8"test1", std::make_shared<TestWidget>("lightblue"), tempnode);
		mainWindow->addNode(u8"test2", std::make_shared<TestWidget>("green"), tempnode);
	}
#endif

	StartUtil::asyncInit();

	auto result = app.exec();

	// ж�ز��
	StartUtil::unloadPlugin();

	return result;
}