#include "StartUtil.h"
#include <UP3D.ProductionProcessToolingInterface/DeviceDiagInterface.h>
#include <UP3D.ProductionProcessToolingInterface/DeviceDiagPluginManager.h>
#include <UP3D.ProductionProcessToolingUI/DeviceDiagnostics.h>
#include <UP3D.ProductionProcessToolingResources/Resources.h>

#include <locale>

#include <QApplication>
#include <QFile>
#include <QFileInfoList>
#include <QFontDatabase>
#include <QDir>
#include <QTextCodec>
#include <QPluginLoader>
#include <QDir>

auto StartUtil::loadPlugin(DeviceDiagnostics* wgt) -> void
{
	QDir pluginFolder("./plugins");
	QStringList files = pluginFolder.entryList(QDir::Files);
	for (const QString& fileName : files)
	{
		if (fileName.endsWith(".dll") && !fileName.contains("example", Qt::CaseInsensitive))
		{
			QString filePath = pluginFolder.absoluteFilePath(fileName);
			QPluginLoader pluginLoader(filePath);
			QJsonObject json = pluginLoader.metaData().value("MetaData").toObject();

			QObject* plugin = pluginLoader.instance();
			if (plugin)
			{
				DeviceDiagInterface* tInterfaceInfo = qobject_cast<DeviceDiagInterface*>(plugin);
				wgt->addNode(json.value("Name").toString(), tInterfaceInfo->getDetailInfoWidget(), json.value("Category").toString());
				PluginManager->addPlugin(tInterfaceInfo);
				qDebug() << "Plugin loaded successfully:" << filePath.toUtf8().data();
			}
			else
			{
				qDebug() << "Failed to load plugin" << pluginLoader.errorString();
			}
		}
	}
}

auto StartUtil::initResources() -> void
{
	 Resources::load();
}

auto StartUtil::loadQss(const QString & path) -> void
{
	QString qss;
	QDir dir(QDir::toNativeSeparators(path));
	QFileInfoList fileInfoList = dir.entryInfoList();
	for(const auto& i : fileInfoList)
	{
		if(i.suffix() != "qss")
			continue;

		QString fullPath = i.absoluteFilePath();
		QFile readFile(fullPath);
		readFile.open(QFile::ReadOnly);
		qss += readFile.readAll();
		readFile.close();
	}

	qss.replace("#replace#", qApp->applicationDirPath() + "/");
	qApp->setStyleSheet(qss);
}

auto StartUtil::initFont(const QString & path) -> bool
{
	QString fontPathR = path + "/AlibabaPuHuiTi-3-45-Light.ttf"; //"°¢Àï°Í°ÍÆÕ»ÝÌå light"
	QString fontPathN = path + "/AlibabaPuHuiTi-3-55-Regular.ttf"; //"°¢Àï°Í°ÍÆÕ»ÝÌå Regular"
	QString fontPathM = path + "/AlibabaPuHuiTi-3-65-Medium.ttf"; //"°¢Àï°Í°ÍÆÕ»ÝÌå Medium"
	QString fontPathB = path + "/AlibabaPuHuiTi-3-85-Bold.ttf"; //"°¢Àï°Í°ÍÆÕ»ÝÌå Bold"

	int fontIdN = QFontDatabase::addApplicationFont(fontPathN);
	int fontIdR = QFontDatabase::addApplicationFont(fontPathR);
	int fontIdM = QFontDatabase::addApplicationFont(fontPathM);
	int fontIdB = QFontDatabase::addApplicationFont(fontPathB);

	QString familyN = QFontDatabase::applicationFontFamilies(fontIdN).at(0);
	QString familyR = QFontDatabase::applicationFontFamilies(fontIdR).at(0);
	QString familyM = QFontDatabase::applicationFontFamilies(fontIdM).at(0);
	QString familyB = QFontDatabase::applicationFontFamilies(fontIdB).at(0);

	QFont font(familyM);
	QApplication::setFont(font);
	return true;
}

auto StartUtil::initTextCodec() -> void
{
	QTextCodec* codec = QTextCodec::codecForName("GBK");
	QTextCodec::setCodecForLocale(codec);
	setlocale(LC_ALL, "chs");
}

auto StartUtil::asyncInit() -> void
{
	for (const auto& item : PluginManager->pluginList())
	{
		item->asyncInit();
	}
}

auto StartUtil::unloadPlugin() -> void
{
	for (const auto& item : PluginManager->pluginList())
	{
		item->unload();
	}
}