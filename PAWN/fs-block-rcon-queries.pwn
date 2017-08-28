#include <custom-query-flood-check>  // for OnCheckForQueryFlood()

public OnQueryFloodCheck(queryType, binaryAddress)
{
	if(queryType == 'x')
	{
		return 1; // don't allow RCON queries to get processed
	}
	return 0;
}