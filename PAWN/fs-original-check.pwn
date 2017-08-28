#include <a_samp>                    // for GetTickCount()
#include <custom-query-flood-check>  // for OnQueryFloodCheck()

/*
 * PAWN equivalent of the original function from SA-MP 0.3.7 server as dissasembled by kurta999 in his YSF fork: 
 * https://github.com/kurta999/YSF/blob/YSF_/src/Hooks.cpp#L390
 */
public OnQueryFloodCheck(queryType, binaryAddress)
{
	static dwLastQueryTick = 0;
	static lastBinAddr = 0;

	if(!dwLastQueryTick)
	{
		dwLastQueryTick = GetTickCount();
		lastBinAddr = binaryAddress;
		return 0;
	}
	if(lastBinAddr != binaryAddress)
	{
		if(GetTickCount() - dwLastQueryTick < 25)
			return 1; // this return here is responsible for blocking queries
		dwLastQueryTick = GetTickCount();
		lastBinAddr = binaryAddress;
	}
	return 0;
}