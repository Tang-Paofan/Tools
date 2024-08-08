#ifndef __DEVICEDIAGPLUGINMANAGER_H__
#define __DEVICEDIAGPLUGINMANAGER_H__

#include "DeviceDiagInterface_Global.h"
#include "DeviceDiagInterface.h"

#include <set>

#define PluginManager DeviceDiagPluginManager::Instance()

class _EXPORT DeviceDiagPluginManager
{
public:
	static DeviceDiagPluginManager* Instance();

public:
	void addPlugin(DeviceDiagInterface* _plugin);
	std::set<DeviceDiagInterface*> pluginList();

private:
	DeviceDiagPluginManager();

private:
	std::set<DeviceDiagInterface*> pluginList_;
};

#endif // !__DEVICEDIAGPLUGINMANAGER_H__
