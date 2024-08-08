#include "DeviceDiagPluginManager.h"

DeviceDiagPluginManager* DeviceDiagPluginManager::Instance()
{
    static DeviceDiagPluginManager ins;
    return &ins;
}

void DeviceDiagPluginManager::addPlugin(DeviceDiagInterface* _plugin)
{
    pluginList_.insert(_plugin);
}

std::set<DeviceDiagInterface*> DeviceDiagPluginManager::pluginList()
{
    return pluginList_;
}

DeviceDiagPluginManager::DeviceDiagPluginManager()
{
}
