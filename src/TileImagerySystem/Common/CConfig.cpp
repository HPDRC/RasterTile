#include "CConfig.h"
#include "CFile.h"
#include "CSemaphore.h"

#define MAX_LINE_LEN (10240)
#define MAX_KEY_LEN (32)
#define MAX_SINGLE_MASK_TEXT_LEN (80)

char* CConfig::LOG_PATH = "C:\\logs\\TileImagerySystem.log";

bool CConfig::TRIM_ENABLED = false;
int CConfig::TRIM_PIXEL_VOID_RANGE[6] = {0,0,0,255,255,255};
int CConfig::TRIM_MIN_CLUSTER_SIZE = 0;
int CConfig::TRIM_REMOVE_NOISE = 0;

char* CConfig::SOURCE_PROJECTION = NULL;
char* CConfig::DEST_PROJECTION = NULL;

int CConfig::PRELOAD_THREAD_CNT = 1;
int CConfig::LOAD_THREAD_CNT = 1;
int CConfig::CALC_THREAD_CNT = 1;
int CConfig::WRITE_THREAD_CNT = 1;
double CConfig::MEMORY_LIMIT_SOURCE = 3072.0 * 1024 * 1024;
double CConfig::MEMORY_LIMIT_TILE = 4096.0 * 1024 * 1024;

vector<SMask> CConfig::masks;

void CConfig::Init(string configString)
{
    int textLength = (int)configString.length();
    const char* text = configString.c_str();
    char line[MAX_LINE_LEN];
    bool isEnd = false;
    for (int i=0, start=0; ; i++)
    {
        if (text[i] == '\r' || text[i] == '\n' || i == textLength)
        {
            if (i - start >= MAX_LINE_LEN)
                LogThrow("CConfig: line is too long to be parsed");
            memset(line, 0, MAX_LINE_LEN);
            strncpy(line, text + start, i - start);
            ParseLine(line);
            start = i + 1;
        }
        if (i == textLength) break;
    }
    Log("CConfig.Init:\n    %s,%d,%d,%d/%d/%d/%d/%d/%d (TRIM_ENABLED,TRIM_REMOVE_NOISE,TRIM_MIN_CLUSTER_SIZE,TRIM_PIXEL_VOID_RANGE)\n    %s,%s (SOURCE,DEST_PROJECTION)\n    %d,%d,%d,%d (PRELOAD,LOAD,CALC,WRITE_THREAD_CNT)\n    %d,%d (MEMORY_LIMIT_SOURCE,TILE)\n    %d (MASK_COUNT)",
        TRIM_ENABLED?"true":"false", TRIM_REMOVE_NOISE, TRIM_MIN_CLUSTER_SIZE, TRIM_PIXEL_VOID_RANGE[0], TRIM_PIXEL_VOID_RANGE[1], TRIM_PIXEL_VOID_RANGE[2], TRIM_PIXEL_VOID_RANGE[3], TRIM_PIXEL_VOID_RANGE[4], TRIM_PIXEL_VOID_RANGE[5],
        SOURCE_PROJECTION==NULL?"NULL":SOURCE_PROJECTION, DEST_PROJECTION==NULL?"NULL":DEST_PROJECTION,
        PRELOAD_THREAD_CNT, LOAD_THREAD_CNT, CALC_THREAD_CNT, WRITE_THREAD_CNT,
        (int)(MEMORY_LIMIT_SOURCE / 1024 / 1024), (int)(MEMORY_LIMIT_TILE / 1024 / 1024),
        masks.size());
}

void CConfig::Reset()
{
    masks.clear();
}

void CConfig::ParseLine(char line[])
{
    // if line starts with #, skip this line
    if (line[0] == '#') return;

    // find char '='
    int length = (int)strlen(line);
    int position = -1;
    for (int i=0; i<length; i++)
    {
        if (line[i] == '=')
        {
            position = i;
            break;
        }
    }
	
    // if not found, skip this line
    if (position == -1) return;

    // get key and value
    char key[MAX_KEY_LEN], value[MAX_LINE_LEN];
    strncpy(key, line, position);
    key[position] = 0;
    strcpy(value, line + position + 1);

    // parse them
    ParseKeyValue(Trim(key), Trim(value));
}

void CConfig::ParseKeyValue(char key[], char value[])
{
    if (strcmpi(key, "LOG_PATH") == 0)
        LOG_PATH = GetStringValue(value);
    else if (strcmpi(key, "TRIM_ENABLED") == 0)
        TRIM_ENABLED = GetIntValue(value) == 0 ? false : true;
    else if (strcmpi(key, "TRIM_REMOVE_NOISE") == 0)
        TRIM_REMOVE_NOISE = GetIntValue(value);
    else if (strcmpi(key, "TRIM_MIN_CLUSTER_SIZE") == 0)
        TRIM_MIN_CLUSTER_SIZE = GetIntValue(value);
    else if (strcmpi(key, "TRIM_PIXEL_VOID_RANGE") == 0)
        sscanf(value, "%d,%d,%d,%d,%d,%d", &TRIM_PIXEL_VOID_RANGE[0], &TRIM_PIXEL_VOID_RANGE[1], &TRIM_PIXEL_VOID_RANGE[2], &TRIM_PIXEL_VOID_RANGE[3], &TRIM_PIXEL_VOID_RANGE[4], &TRIM_PIXEL_VOID_RANGE[5]);
    else if (strcmpi(key, "SOURCE_PROJECTION") == 0)
        SOURCE_PROJECTION = GetStringValue(value);
    else if (strcmpi(key, "DEST_PROJECTION") == 0)
        DEST_PROJECTION = GetStringValue(value);
    else if (strcmpi(key, "LOAD_THREAD_CNT") == 0)
        LOAD_THREAD_CNT = GetIntValue(value);
    else if (strcmpi(key, "CALC_THREAD_CNT") == 0)
        CALC_THREAD_CNT = GetIntValue(value);
    else if (strcmpi(key, "WRITE_THREAD_CNT") == 0)
        WRITE_THREAD_CNT = GetIntValue(value);
	else if (strcmpi(key, "MEMORY_LIMIT_SOURCE") == 0)
		MEMORY_LIMIT_SOURCE = GetIntValue(value) * 1024.0 * 1024.0;
    else if (strcmpi(key, "MEMORY_LIMIT_TILE") == 0)
		MEMORY_LIMIT_TILE = GetIntValue(value) * 1024.0 * 1024.0;
    else if (strcmpi(key, "TRIM_MASKS") == 0)
    {
        // reset masks
        masks.clear();
        int textLength = (int)strlen(value);
        if (textLength > 0)
        {
            char maskStr[MAX_SINGLE_MASK_TEXT_LEN];
            char maskType[MAX_SINGLE_MASK_TEXT_LEN];
            for (int i=0, start=0; ; i++)
            {
                if (value[i] == '&' || i == textLength)
                {
                    if (i - start >= MAX_SINGLE_MASK_TEXT_LEN)
                        LogThrow("CConfig: mask text is too long to be parsed");
                    memset(maskStr, 0, MAX_SINGLE_MASK_TEXT_LEN);
                    memset(maskType, 0, MAX_SINGLE_MASK_TEXT_LEN);
                    strncpy(maskStr, value + start, i - start);

                    // convert maskStr to mask
                    SMask mask;
                    int eleCnt = sscanf(maskStr, "%lf,%lf,%lf,%lf,%s", &mask.rect.top, &mask.rect.right, &mask.rect.bottom, &mask.rect.left, maskType);
                    if (eleCnt!=5)
                        LogThrow("CConfig: mask text is in a wrong format");
                    if (strcmpi(maskType, "SKIP") == 0)
                        mask.type = 1;
                    else if (strcmpi(maskType, "REMOVE") == 0)
                        mask.type = 3;
                    else
                    {
                        mask.type = 2;
                        eleCnt = sscanf(maskType, "NEWRANGE,%d,%d,%d,%d,%d,%d", &mask.voidRange[0], &mask.voidRange[1], &mask.voidRange[2], &mask.voidRange[3], &mask.voidRange[4], &mask.voidRange[5]);
                        if (eleCnt != 6)
                            LogThrow("CConfig: mask's type is not recognized");
                    }

                    // add mask to list
                    masks.push_back(mask);
                    start = i + 1;
                }
                if (i >= textLength) break;
            }
        }
    }
}

char* CConfig::GetStringValue(char value[])
{
    char *result = new char[strlen(value)];
    strcpy(result, value);
    return result;
}

int CConfig::GetIntValue(char value[])
{
    int result;
    sscanf(value, "%d", &result);
    return result;
}

double CConfig::GetDoubleValue(char value[])
{
    double result;
    sscanf(value, "%lf", &result);
    return result;
}

char* CConfig::Trim(char str[])
{
    int length = (int)strlen(str);
    for (int i=length-1; i>=0; i--)
    {
        if (str[i]==' ' || str[i]=='\t' || str[i]=='\r' || str[i]=='\n')
            str[i] = 0;
        else
            break;
    }
    for (int i=0; i<length; i++)
    {
        if (str[i]==' ' || str[i]=='\t' || str[i]=='\r' || str[i]=='\n')
            continue;
        return str+i;
    }
    return str;
}