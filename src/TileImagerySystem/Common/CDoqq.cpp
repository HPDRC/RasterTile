#include "CDoqq.h"
#include "CImg.h"
#include "CHelper.h"
#include "CMemory.h"
#include "CConfig.h"
#include "CFile.h"
#include "..\..\CxImage\ximage.h"

// some defines and constants
#define DOQQ_MAX_LINE_LEN   (1024)
#define DOQQ_MAX_FIELD_LEN   (83)

// compressed doqq files must have fixed extension names
#define DOQQC_DATAFILE_EXT  "~j"
#define DOQQC_INDEXFILE_EXT "~i"

// for now, compressed doqq files hold small tiles compressed as jpg
#define DOQQC_TILE_IMAGE_FORMAT CXIMAGE_FORMAT_JPG

// doqq files have the following RGB order
#define DOQQ_R_OFFSET   (0)
#define DOQQ_G_OFFSET   (1)
#define DOQQ_B_OFFSET   (2)

// compressed doqq files have the following RGB order
#define DOQQC_R_OFFSET   (2)
#define DOQQC_G_OFFSET   (1)
#define DOQQC_B_OFFSET   (0)

// define fields
#define DOQQ_FIELD_HEADER_BEGIN "BEGIN_USGS_DOQ_HEADER"
#define DOQQ_FIELD_HEADER_END   "END_USGS_DOQ_HEADER"
#define DOQQ_FIELD_HEADER_END2   "END_USGS_HEADER"
#define DOQQ_FIELD_BAND_CONTENT "BAND_CONTENT"
#define DOQQ_FIELD_SAMPLES_AND_LINES "SAMPLES_AND_LINES"
#define DOQQ_FIELD_COORDINATE_ZONE "COORDINATE_ZONE"
#define DOQQ_FIELD_HORIZONTAL_RESOLUTION "HORIZONTAL_RESOLUTION"
#define DOQQ_FIELD_XY_ORIGIN "XY_ORIGIN"
#define DOQQ_FIELD_SOURCE_IMAGE_DATE "SOURCE_IMAGE_DATE"

#define DOQQ_TILE_WIDTH (400)
#define DOQQ_TILE_HEIGHT (400)

void CDoqq::ReadHeader(const char filePath[], const char buffer[], int bufferLength, SSourceMetaInfo *pSourceMetaInfo, char *imageDate, int *pHeaderLength)
{
	bufferLength = min(bufferLength, DOQQ_MAX_HEADER_LEN);

	// check if is valid doqq file
    int len = (int)strlen(DOQQ_FIELD_HEADER_BEGIN);
    for (int i=0; i<len; i++)
        if (buffer[i] != DOQQ_FIELD_HEADER_BEGIN[i])
            LogThrow("CDoqq: header info not found from %s", filePath);

	// parse lines (a new line starts after '\n')
    for (int i=0, j=0; i<bufferLength; i++)
    {
        if (buffer[i] == '\n')
        {
            if (i - j > DOQQ_MAX_LINE_LEN) continue;    // skip this line if it's too long which means there is probably a problem line
            if (ParseLine(buffer+j, i-j, pSourceMetaInfo, imageDate) == false)
            {
                *pHeaderLength = i+1;
                return;
            }
            j = i+1;
        }
    }

    LogThrow("CDoqq: end of header is not found", filePath);
}

bool CDoqq::ParseLine(const char line[], int length, SSourceMetaInfo *pSourceMetaInfo, char *imageDate)
{
    char name[DOQQ_MAX_FIELD_LEN], value[DOQQ_MAX_FIELD_LEN];
    memset(name, 0, DOQQ_MAX_FIELD_LEN);
    memset(value, 0, DOQQ_MAX_FIELD_LEN);

    int i, j;
    for (i=0; line[i]!=' ' && line[i]!='*' && i<length; i++)
        name[i] = line[i];

    j = i;
    for (; line[i]!='*' && i<length; i++)
        value[i-j] = line[i];

    return ParseField(name, value, pSourceMetaInfo, imageDate);
}

bool CDoqq::ParseField(char name[], char value[], SSourceMetaInfo *pSourceMetaInfo, char *imageDate)
{
    if (strcmp(name, DOQQ_FIELD_HEADER_END)==0 || strcmp(name, DOQQ_FIELD_HEADER_END2)==0)
        return false;
    else if (strcmp(name, DOQQ_FIELD_SAMPLES_AND_LINES) == 0)
		sscanf(value, " %d %d", &pSourceMetaInfo->width, &pSourceMetaInfo->height);
    else if (strcmp(name, DOQQ_FIELD_COORDINATE_ZONE) == 0)
		sscanf(value, " %d", &pSourceMetaInfo->zone);
    else if (strcmp(name, DOQQ_FIELD_HORIZONTAL_RESOLUTION) == 0)
    {
		sscanf(value, " %lf", &pSourceMetaInfo->xScale);
        pSourceMetaInfo->yScale = -pSourceMetaInfo->xScale;
    }
    else if (strcmp(name, DOQQ_FIELD_XY_ORIGIN) == 0)
        sscanf(value, " %lf %lf", &pSourceMetaInfo->x, &pSourceMetaInfo->y);
    else if (strcmp(name, DOQQ_FIELD_SOURCE_IMAGE_DATE) == 0)
        strcpy(imageDate, value);
    return true;
}

CImg* CDoqq::LoadDoqq(SSourceMetaInfo sourceMetaInfo, U8 *fileBuff, int fileLength, const char filePath[], bool isCompressed)
{
    // create empty img
    CImg *pImg = new CImg(sourceMetaInfo.width, sourceMetaInfo.height);
	pImg->R_Offset = isCompressed ? DOQQC_R_OFFSET : DOQQ_R_OFFSET;
    pImg->G_Offset = isCompressed ? DOQQC_G_OFFSET : DOQQ_G_OFFSET;
    pImg->B_Offset = isCompressed ? DOQQC_B_OFFSET : DOQQ_B_OFFSET;

	// get header length
	SSourceMetaInfo tmpSourceMetaInfo;
	int headerLength;
	char imageDate[MAX_PATH_LEN];
	ReadHeader(filePath, (char*)fileBuff, fileLength, &tmpSourceMetaInfo, imageDate, &headerLength);
	U8 *data = fileBuff + headerLength;

	if (!isCompressed)
	{
		// check if file length is correct
		bool isBlackWhite = false;
		if (fileLength == headerLength + sourceMetaInfo.width * sourceMetaInfo.height * 3)
			isBlackWhite = false;
		else if (fileLength == headerLength + sourceMetaInfo.width * sourceMetaInfo.height)
			isBlackWhite = true;
		else
			LogThrow("CDoqq: incorrect file length. file path: %s", filePath);
	
		// calculate width of strips
		int borderLeft = DOQQ_TILE_WIDTH - CHelper::Floor(sourceMetaInfo.x/sourceMetaInfo.xScale) % DOQQ_TILE_WIDTH;
		int stripCnt = (int)ceil((double)(sourceMetaInfo.width - borderLeft) / DOQQ_TILE_WIDTH) + 1;
		int* widthArray = new int[stripCnt];
		widthArray[0] = borderLeft;
		for (int i=1; i<stripCnt-1; i++)
			widthArray[i] = DOQQ_TILE_WIDTH;
		widthArray[stripCnt-1] = (sourceMetaInfo.width - borderLeft) % DOQQ_TILE_WIDTH;
		if (widthArray[stripCnt-1] == 0)
			widthArray[stripCnt-1] = DOQQ_TILE_WIDTH;

		// check is widthArray is correct
		int totalWidth = 0;
		for (int i=0; i<stripCnt; i++)
			totalWidth += widthArray[i];
		if (totalWidth != sourceMetaInfo.width)
			LogThrow("CDoqq: incorrect widthArray, %s", filePath);

		// begin to load
		// for color image, read file to buffer directly
		if (!isBlackWhite)
		{
			for (int i=0, offsetw=0; i<stripCnt; i++)	// foreach vertical strip
			{
				for (int h=0; h<sourceMetaInfo.height; h++)	// foreach line in the strip
				{
					memcpy(pImg->GetPixelPointer(offsetw, h), data, widthArray[i] * 3);
					data += widthArray[i] * 3;
				}
				offsetw += widthArray[i];
			}
		}
		// for blackwhite image, read file to buffer, then extend 1byte to 3bytes
		else
		{
			for (int i=0, offsetw=0; i<stripCnt; i++)	// foreach vertical strip
			{
				for (int h=0; h<sourceMetaInfo.height; h++)	// foreach line in the strip
				{
					U8 *buff = pImg->GetPixelPointer(offsetw, h);
					for (int k=0; k<widthArray[i]; k++)		// extend 1 byte to 3 bytes
						buff[k*3] = buff[k*3+1] = buff[k*3+2] = data[k];
                    data += widthArray[i];
				}
				offsetw += widthArray[i];
			}
		}

		delete[] widthArray;
	}
	else
	{
		// calculate width and height of tiles
		int borderLeft = DOQQ_TILE_WIDTH - CHelper::Floor(sourceMetaInfo.x/sourceMetaInfo.xScale) % DOQQ_TILE_WIDTH;
		int borderUp = CHelper::Floor(sourceMetaInfo.y/sourceMetaInfo.xScale) % DOQQ_TILE_HEIGHT;
		if (borderUp == 0)
			borderUp = DOQQ_TILE_HEIGHT;
		int tileCntW = CHelper::Ceil((double)(sourceMetaInfo.width - borderLeft) / DOQQ_TILE_WIDTH) + 1;
		int tileCntH = CHelper::Ceil((double)(sourceMetaInfo.height - borderUp) / DOQQ_TILE_HEIGHT) + 1;
		int tileCntTotal = tileCntW * tileCntH;
		int* widthArray = new int[tileCntW];
		int* heightArray = new int[tileCntH];
		widthArray[0] = borderLeft;
		heightArray[0] = borderUp;
		for (int i=1; i<tileCntW-1; i++)
			widthArray[i] = DOQQ_TILE_WIDTH;
		for (int i=1; i<tileCntH-1; i++)
			heightArray[i] = DOQQ_TILE_HEIGHT;
		if (tileCntW > 1)
		{
			widthArray[tileCntW-1] = (sourceMetaInfo.width - borderLeft) % DOQQ_TILE_WIDTH;
			if (widthArray[tileCntW-1] == 0)
				widthArray[tileCntW-1] = DOQQ_TILE_WIDTH;
		}
		if (tileCntH > 1)
		{
			heightArray[tileCntH-1] = (sourceMetaInfo.height - borderUp) % DOQQ_TILE_HEIGHT;
			if (heightArray[tileCntH-1] == 0)
				heightArray[tileCntH-1] = DOQQ_TILE_HEIGHT;
		}

		// calculate tiles' offsets in the image
		int* offsetxArray = new int[tileCntW];
		int* offsetyArray = new int[tileCntH];
		offsetxArray[0] = offsetyArray[0] = 0;
		for (int i=1; i<tileCntW; i++)
			offsetxArray[i] = offsetxArray[i-1] + widthArray[i-1];
		for (int i=1; i<tileCntH; i++)
			offsetyArray[i] = offsetyArray[i-1] + heightArray[i-1];

		// open ~i file (index file which records every tile's offset and length)
		string indexPath = CHelper::PathChangeExt(filePath, DOQQC_DATAFILE_EXT, DOQQC_INDEXFILE_EXT);
		CFile indexFile(indexPath.c_str(), false);

		// check index file's length
		int indexFileLength = (int)indexFile.GetSize();
		if (indexFileLength != tileCntTotal * 8)
			LogThrow("CDoqq: index file's length is incorrect: %s", filePath);

		// seek index to the very beginning
		indexFile.SeekStrict(0, SEEK_SET);

		// allocate memory for array
		int* offsetInFileArray = new int[tileCntTotal];
		int* lengthArray = new int[tileCntTotal];

		// read all offsets into array
		S64 imgLength = 0;
		for (int i=0; i<tileCntTotal; i++)
		{
			indexFile.ReadStrict(&imgLength, sizeof(imgLength));
			offsetInFileArray[i] = (int)imgLength;
		}
		for (int i=0; i<tileCntTotal-1; i++)
			lengthArray[i] = offsetInFileArray[i+1] - offsetInFileArray[i];
		lengthArray[tileCntTotal-1] = (fileLength - headerLength) - offsetInFileArray[tileCntTotal-1];

		// foreach vertical tile strip
		for (int tilew=0, tileIndex=0; tilew<tileCntW; tilew++)
		{
			// foreach tile in this strip
			//for (int tileh=tileCntH-1; tileh>=0; tileh--, tileIndex++)
			for (int tileh=0; tileh<tileCntH; tileh++, tileIndex++)
			{
				// create CImg object to hold this compressed image
				CImg imgCompressed(data+offsetInFileArray[tileIndex], lengthArray[tileIndex], DOQQC_TILE_IMAGE_FORMAT);
            
				// skip file if corrupted
				if (imgCompressed.isCreateSuccess==false || imgCompressed.width==0 || imgCompressed.height==0)
					continue;

				// check if width and height matches expected value
				if (imgCompressed.width != widthArray[tilew] || imgCompressed.height != heightArray[tileh])
					LogThrow("CDoqq: compressed doqq's tile has unexpected width and height: %s", filePath);

				// foreach line in the tile
                imgCompressed.PasteTo(pImg, offsetxArray[tilew], offsetyArray[tileh]);
			}
		}

		// clean up
		delete[] offsetxArray, delete[] offsetyArray;
		delete[] offsetInFileArray, delete[] lengthArray;
		delete[] widthArray, delete[] heightArray;
	}

	return pImg;
}
