#include "../PluginAPI.h"
#include "../Config.h"
#include "../GLideN64.h"
#include <Revision.h>
#include "../DisplayWindow.h"

#include <cstdio>

void PluginAPI::DllAbout(void* /*_hParent*/)
{
	std::fprintf(stdout, "%s\n", pluginNameWithRevision);
}

void PluginAPI::CaptureScreen(const char * const _Directory)
{
	dwnd().setCaptureScreen(_Directory);
}

void PluginAPI::DllConfig(void* /*_hParent*/)
{
	Config_LoadConfig();
}

void PluginAPI::GetDllInfo(PLUGIN_INFO * PluginInfo)
{
	PluginInfo->Version = 0x105;
	PluginInfo->Type = PLUGIN_TYPE_VIDEO;
	std::snprintf(PluginInfo->Name, sizeof(PluginInfo->Name), "%s", pluginNameWithRevision);
}

void PluginAPI::DrawStatus(const char * lpString, int32_t /*RightAlign*/)
{
	if (lpString != nullptr)
		std::fprintf(stdout, "%s\n", lpString);
}
