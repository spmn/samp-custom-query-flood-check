#include <vector>
#include <map>
#include <algorithm>

#include "samp-plugin-sdk/amx/amx.h"
#include "samp-plugin-sdk/plugincommon.h"

#include "urmem/urmem.hpp"

using logprintf_t = void(*)(char *format, ...);
logprintf_t logprintf;

extern void *pAMXFunctions;

std::vector<std::pair<AMX *, int>> callbackIds;

std::map<char, urmem::address_t> CheckQueryFloodCalls = {
#ifdef _WIN32
	{ 'p', 0x00497C6A },
	{ 'i', 0x00497CBE },
	{ 'c', 0x00497EAC },
	{ 'd', 0x00497FC3 },
	{ 'r', 0x00498149 },
	{ 'x', 0x004981AF }
#else
	{ 'p', 0x080D3438 },
	{ 'i', 0x080D34B3 },
	{ 'c', 0x080D3882 },
	{ 'd', 0x080D36C7 },
	{ 'r', 0x080D33EF },
	{ 'x', 0x080D3283 }
#endif
};

bool CheckForQueryFlood(char query, uint32_t binaryAddress)
{
	if (callbackIds.size() == 0)
	{
		logprintf("[CQFC] WARNING: No custom query flood check found.");
		logprintf("[CQFC] WARNING: Default policy: Allow all query packets.");
	}
	else
	{
		for (const auto& callback : callbackIds)
		{
			cell cbRet;
			const auto& cbAMX = callback.first;
			const auto& cbIdx = callback.second;

			amx_Push(cbAMX, binaryAddress);
			amx_Push(cbAMX, query);
			amx_Exec(cbAMX, &cbRet, cbIdx);

			if (cbRet != 0)
				return 1;
		}
	}
	return 0;
}

template<char query> uint32_t /*__cdecl*/ CheckForQueryFlood(uint32_t binaryAddress)
{
	return CheckForQueryFlood(query, binaryAddress);
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) 
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
    logprintf = reinterpret_cast<logprintf_t>(ppData[PLUGIN_DATA_LOGPRINTF]);
	logprintf("  Loading SA-MP Custom Query Flood Check v1.0 ...");

	if (logprintf != (void *)0x0048C8D0 && logprintf != (void *)0x080A91D0)
	{
		logprintf("  CQFC plugin is compatible only with SA-MP 0.3.7 R2-1.");
		return 0;
	}

	new urmem::hook(CheckQueryFloodCalls['p'], urmem::get_func_addr(CheckForQueryFlood<'p'>), urmem::hook::type::call);
	new urmem::hook(CheckQueryFloodCalls['i'], urmem::get_func_addr(CheckForQueryFlood<'i'>), urmem::hook::type::call);
	new urmem::hook(CheckQueryFloodCalls['c'], urmem::get_func_addr(CheckForQueryFlood<'c'>), urmem::hook::type::call);
	new urmem::hook(CheckQueryFloodCalls['d'], urmem::get_func_addr(CheckForQueryFlood<'d'>), urmem::hook::type::call);
	new urmem::hook(CheckQueryFloodCalls['r'], urmem::get_func_addr(CheckForQueryFlood<'r'>), urmem::hook::type::call);
	new urmem::hook(CheckQueryFloodCalls['x'], urmem::get_func_addr(CheckForQueryFlood<'x'>), urmem::hook::type::call);

	return 1;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload() { }

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)
{
	int cbIdx;
	if (amx_FindPublic(amx, "OnQueryFloodCheck", &cbIdx) == AMX_ERR_NONE)
	{
		callbackIds.push_back(std::make_pair(amx, cbIdx));
	}
	return AMX_ERR_NONE;
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx)
{
	callbackIds.erase(
		std::remove_if(callbackIds.begin(), callbackIds.end(), [amx](const std::pair<AMX *, int>& elem) {
			return elem.first == amx;
		}),
		callbackIds.end()
	);
    return AMX_ERR_NONE;
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION;
}