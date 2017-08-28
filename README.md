# Custom Query Flood Check (CQFC plugin)
Write custom protections against query flood
## WARNING
- Installing this plugin will disable the built-in query flood check, so you must your own protection!
- Use [this filterscript](PAWN/fs-original-check.pwn) to reimplement the default query flood check.
- For better performance, implement the protection directly into plugin source code to avoid AMX overhead.
## Callback
```pawn
forward OnQueryFloodCheck(queryType, binaryAddress); 
/*
 * Info:
 *  - called when a query is received
 *
 * Parameters:
 *  - queryType     : a single character corresponding to the packet identifier as explained here: http://wiki.sa-mp.com/wiki/Query_Mechanism#Opcodes
 *  - binaryAddress : IP address of requester as a 32-bit unsigned integer
 *
 * Returns:
 *  -   0 (ZERO)    : process the query
 *  - !=0 (NONZERO) : don't process the query
 *
 * Notes:
 *  - If any script returns NONZERO in this callback, then current query won't get processed.
 *  - If this callback is missing from all loaded scripts, then the plugin will **PROCESS ALL QUERIES** and will print a warning message in server log. 
 */
```
## How to install
Download the archive from [Releases page](https://github.com/spmn/samp-custom-query-flood-check/releases) and unpack it into your server directory. Then edit "server.cfg":
- Windows
```
filterscripts fs-original-check # or write your own protection
plugins samp-custom-query-flood-check.dll
```
- Linux
```
filterscripts fs-original-check # or write your own protection
plugins samp-custom-query-flood-check.so
```