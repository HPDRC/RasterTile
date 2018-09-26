#pragma once
#include "includes.h"
#include "..\Common\CSemaphore.h"

// manage a source image
class CSource : CSemaphore
{
private:
	// used in preload and load
	U32 fileLength;
    U8* fileBuffer;

    // path of source file
    string filePath;

    void GenerateLocalMasks(vector<SMask> *pMasks);

public:
    // which loader is this source belongs to
	CLoader *pLoader;

	// image of this source
	CImg *pImg;

    // the meta info of this source
    SSourceMetaInfo sourceMetaInfo;

    // a list of pyramid nodes which links to this source
    vector<STileXY> pyramidNodesList;

	// the count of unfinished source convert task
	int ungeneratedTileCnt;

    // this source should load to which level
    int loadLevel;

    // the tile boundary of this source
    int tilexmin, tileymin, tilexmax, tileymax;
    
public:
	// sourceId: id of the source in this load
    CSource(CLoader *pLoader, SSourceMetaInfo sourceMetaInfo, string filePath);
    ~CSource();

    bool ContainsPoint(int x, int y);
    bool ContainsTile(STileXY tilexy);

    void CalculateTileBoundary();
    void CreatePyramidNodes();              // create pyramid nodes which are generated from this source
	int DecreaseUngeneratedTileCnt();       // when one relative tile is generated, this function should be called
    
    void Preload();     // preload means load file into memory
    void Load();        // load means load memory into CImg
    void Release();     // release means release file buffer from memory, only CImg object is left
	
	void CreateGenerateRelativeTilesTask(); // create tasks which generates it's relative tiles
    void CreatePreloadTask();               // create a task to start loading the source
    
	static void TaskFunc_Preload(CSource *pSource, CPyramidNode* pNode);    // task function, preload source
	static void TaskFunc_Load(CSource *pSource, CPyramidNode* pNode);       // task function, load source
};
