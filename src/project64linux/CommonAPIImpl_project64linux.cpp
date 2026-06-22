#include "../PluginAPI.h"
#include "../RSP.h"

#include <algorithm>
#include <cstring>
#include <dlfcn.h>
#include <string>
#include <unistd.h>

extern "C" void Project64LinuxSwapBuffers();

namespace
{
void(CALL * g_swapBuffers)(void) = nullptr;

void CopyPath(wchar_t * dst, const std::string & path)
{
	if (dst == nullptr)
		return;

	std::mbstate_t state{};
	const char * src = path.c_str();
	std::mbsrtowcs(dst, &src, PLUGIN_PATH_SIZE - 1, &state);
	dst[PLUGIN_PATH_SIZE - 1] = L'\0';
}
}

int PluginAPI::InitiateGFX(const GFX_INFO & _gfxInfo)
{
	_initiateGFX(_gfxInfo);
	g_swapBuffers = _gfxInfo.SwapBuffers;
	REG.SP_STATUS = _gfxInfo.SP_STATUS_REG;
	RDRAMSize = _gfxInfo.RDRAM_SIZE != 0 ? _gfxInfo.RDRAM_SIZE - 1 : 0;
	return TRUE;
}

void PluginAPI::FindPluginPath(wchar_t * _strPath)
{
	std::string path;
	Dl_info info{};
	if (dladdr(reinterpret_cast<void *>(&Project64LinuxSwapBuffers), &info) != 0 && info.dli_fname != nullptr)
	{
		path = info.dli_fname;
	}
	else
	{
		char exePath[PLUGIN_PATH_SIZE] = {};
		const ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
		path.assign(exePath, len > 0 ? static_cast<size_t>(len) : 0);
	}

	std::replace(path.begin(), path.end(), '\\', '/');
	const std::string::size_type pos = path.find_last_of('/');
	if (pos != std::string::npos)
		path.erase(pos);
	if (path.empty())
		path = ".";

	CopyPath(_strPath, path);
}

void PluginAPI::GetUserDataPath(wchar_t * _strPath)
{
	FindPluginPath(_strPath);
}

void PluginAPI::GetUserCachePath(wchar_t * _strPath)
{
	FindPluginPath(_strPath);
}

extern "C" void Project64LinuxSwapBuffers()
{
	if (g_swapBuffers != nullptr)
		g_swapBuffers();
}
