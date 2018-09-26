#pragma once
#include "includes.h"
#include "..\Common\CSemaphore.h"

// a node of the pyramid
class CPyramidNode : CSemaphore
{
private:
    static const U8 ALL_FILLED;
    static const U8 COMPLETED;
    static const int FILLED_COUNT[];

	void GenerateImgFromSon(CPyramidNode *pSon, int offsetw, int offseth);

public:
    // point to its loader
	CLoader *pLoader;

    // point to its img
    CImg *pImg;

    // this node depends on how many other source or nodes to generate
    int dependencyCount;

    // its xy
	STileXY tilexy;

    bool hasPixelFromSource;

public:
    CPyramidNode(CLoader *pLoader);

    void Reset(const STileXY &tilexy, int dependencyCount);

    CPyramidNode* GetParent();

    // decrease the dependency count and trigger events once no dependency exists
    void DecreaseDependency();
    
    // allocate memory for the img
    void AllocateMemoryForImg(bool ignoreMemoryLimit);
	
	// convert alpha layer from isFilled to transparency
	void ConvertAlphaLayer();

    // generate img from its son
    void GenerateImgFromSon(CPyramidNode *pSon);

    // generate img from source
    void GenerateImgFromSource(CSource* pSource);

    // create a task to generate this node's img from source
    void CreateGenerateImgFromSourceTask(CSource* pSource);
    
    // task function, generate this node's img from source
    static void TaskFunc_GenerateImgFromSource(CSource *pSource, CPyramidNode *pNode);
};
