#pragma once
#include "global_includes.h"

#define DOQQ_MAX_HEADER_LEN   (300 * 1024)

// manage one doqq file
// for doqq file specification, see http://nationalmap.gov/standards/pdf/2DOQ1296.PDF
// but the doqq file in our system didn't follow this spec, check d_program_test project for details
// support up to 4GB size of doqq file
// compressed doqq files have layout as: [tile1(xn=x,yn=y+height-1)] [tile2(xn=x,yn=y+height-2)] ... [tile(xn=x,yn=y)] ... [tileN]
// doqq files have such layout:
//      [vertical strip1: height=imgHeight, width=(400-(utmx/res)%400)] [vertical strip2:height=imgHeight, width=400] ... [vertical stripn]
// for (*.doqq) files (which is newer than old doqq files don't have fixed extension name), utmx/res can be devided by 400
class CDoqq
{
private:
    // parse one line, return false if end of header is met
    static bool ParseLine(const char line[], int length, SSourceMetaInfo *pSourceMetaInfo, char *imageDate);

    // parse one field, return false if end of header is met
    static bool ParseField(char name[], char value[], SSourceMetaInfo *pSourceMetaInfo, char *imageDate);

public:
	// read header info from doqq
	static void ReadHeader(const char filePath[], const char buffer[], int bufferLength, SSourceMetaInfo *pSourceMetaInfo, char *imageDate, int *pHeaderLength);

    // load uncompressed doqq into CImg object
    static CImg* LoadDoqq(SSourceMetaInfo sourceMetaInfo, U8 *fileBuff, int fileLength, const char filePath[], bool isCompressed);
};
