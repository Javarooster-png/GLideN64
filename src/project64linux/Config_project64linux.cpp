#include "../Config.h"

Config config;

void Config_LoadConfig()
{
	config.resetToDefaults();
	config.validate();
}

void Config_DoConfig(void* /*parent*/)
{
	Config_LoadConfig();
}
