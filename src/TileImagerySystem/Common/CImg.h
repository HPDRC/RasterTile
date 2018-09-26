#pragma once
#include "global_includes.h"

class CImg
{
public:
    CxImage *pCxImage;
    
    // initiate this object
    void Init();

    // basic attributes of img
    int width, height, R_Offset, G_Offset, B_Offset;

    // is creation success
	bool isCreateSuccess;

public:
    // create empty bmp image with specified width and height
    CImg(int width, int height);

    // create image from buffer (will allocate new memory)
    CImg(U8 *buffer, int size, int imagetype = 0);

    ~CImg();

    U8* GetPixelPointer(int x, int y);

    U8* GetAlphaPointer(int x, int y);

    U8 GetAlpha(int x, int y);

    void SetAlpha(U8 alpha, int x, int y, int w, int h);

	int GetSizeInMemory();

    // return true if this img has a alpha layer of all transparent
    bool IsEmpty();

    // return true if this img has a alpha layer of all opaque
    bool IsFull();

    bool HasAlpha();

    // clear this image, paint entire image to black
    void Clear();

    // clear alpha layer as specified value (0 fully transparent)
    void ClearAlpha(U8 alpha = 0);

    // create alpha layer. return if success
    void CreateAlpha();

    // remove void pixels from image, will create alpha if any void pixel is removed.
    // pMasks: masked areas will not be trimmed
    void Trim(vector<SMask> *pMasks);

    // scanline algorithm used by trim functino
    void Trim_Scanline(vector<SShortXY> *pStack, int x, int y, U8 *status, int &totalCnt, bool isSetAlpha);

    // discard non-overlap pixels
    void PasteTo(CImg *imgDst, int x, int y);

    // resample to a new size
    void Resample(int width, int height);

    // return NULL if failed
    U8* EncodeAsJpeg(int jpegQuality, int *pBufferLength);

    // return NULL if failed
    U8* EncodeAsPng(int *pBufferLength);

    // Get grid completeness info of this img
    int GetGridCompleteness();
};
