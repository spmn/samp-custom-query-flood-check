#include <custom-query-flood-check>  // for OnQueryFloodCheck()

public OnQueryFloodCheck(queryType, binaryAddress)
{
	if(queryType == 'c' || queryType == 'd')
	{
		return 1; // don't send back the player list
	}
	return 0;
}