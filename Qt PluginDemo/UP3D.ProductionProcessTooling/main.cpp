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
		/// 获取信号量失败, 直接退出
		return EXIT_FAILURE;
	}

	// 初始化缩放倍率要放在Application初始化之前
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
	
	// 添加插件路径
	QApplication::addLibraryPath("./plugins");

	// 构造Application
	QApplication app(argc, argv);
	QDir::setCurrent(app.applicationDirPath() + "/");

	// 加载QSS
	StartUtil::loadQss(qApp->applicationDirPath() + "/Resource/Skin");

	// 加载字体
	StartUtil::initFont(qApp->applicationDirPath() + "/Resource/FontFamily");

	// 加载依赖动态库
	QSettings settings("HKEY_CURRENT_USER\\Software\\UP3D\\Intraoral", QSettings::NativeFormat);
	QString oralInstallPath = settings.value("Path").toString();
	QString path = oralInstallPath + ";" + qgetenv("PATH");
	qputenv("PATH", path.toLocal8Bit());

	// 创建主窗口
	DeviceDiagnostics* wgt = new DeviceDiagnostics();
	wgt->show();

	// 加载插件
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

	// 卸载插件
	StartUtil::unloadPlugin();

	return result;
}