#include "Resources.h"

#include <QtCore/QResource>
#include <QtWidgets/QApplication>
#include <QtCore/QDir>

bool Resources::load()
{
	bool ret = QResource::registerResource(qApp->applicationDirPath() + QDir::separator() + "UP3D.DeviceDiagResources.dll");

	return ret;
}
