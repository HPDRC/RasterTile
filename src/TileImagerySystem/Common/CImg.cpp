#include "CImg.h"
#include "CMemory.h"
#include "CConfig.h"
#include "..\..\CxImage\ximage.h"

CImg::CImg(int width, int height)
{
	if ((double)width * height * BYTE_PER_PIXEL > 0xFFFFFFFF)
		LogThrow("CImg: size of image is too large to be supported");

    // create image
    pCxImage = new CxImage(width, height, BPP, CXIMAGE_FORMAT_BMP);
    Init();
}

CImg::CImg(U8 *buffer, int size, int imagetype)
{
    // load image
    pCxImage = new CxImage(buffer, size, imagetype);

	// if is black white, convert to color image
	if (pCxImage->GetBpp() == 8)
	{
		CxImage *newImage = new CxImage(pCxImage->GetWidth(), pCxImage->GetHeight(), BPP, CXIMAGE_FORMAT_BMP);
		int height = (int)pCxImage->GetHeight();
		int width = (int)pCxImage->GetWidth();
		for (int h=0; h<height; h++)
		{
			U8 *bw = pCxImage->GetBits(h);
			U8 *color = newImage->GetBits(h);
			for (int w=0; w<width; w++)
				color[w*3] = color[w*3+1] = color[w*3+2] = bw[w];
		}
		delete pCxImage;
        pCxImage = newImage;
	}
    
    Init();
}

void CImg::Init()
{
    // for lots of images, this RGB byte order is used (like JPG, PNG, TIFF and so on)
    // change it if necessary.
    R_Offset = 2, G_Offset = 1, B_Offset = 0;

	isCreateSuccess = (pCxImage->GetBpp() != 24) ? false: true;
    width = pCxImage->GetWidth();
    height = pCxImage->GetHeight();
}

CImg::~CImg()
{
    delete pCxImage;
}

U8* CImg::GetPixelPointer(int x, int y)
{
    // NOTE: CxImage uses bottom-up line order
    return pCxImage->GetBits(height-1-y) + x * BYTE_PER_PIXEL;
}

U8* CImg::GetAlphaPointer(int x, int y)
{
    // NOTE: CxImage uses bottom-up line order
    return pCxImage->AlphaGetPointer(x, height-1-y);
}

U8 CImg::GetAlpha(int x, int y)
{
    // NOTE: CxImage uses bottom-up line order
    return pCxImage->AlphaGet(x, height-1-y);
}

void CImg::SetAlpha(U8 alpha, int x, int y, int w, int h)
{
    int y2 = y + h;
    for (; y<y2; y++)
        memset(GetAlphaPointer(x, y), alpha, w);
}

int CImg::GetSizeInMemory()
{
	return width * height * (BYTE_PER_PIXEL + 1);
}

bool CImg::IsEmpty()
{
    for (int h=0; h<height; h++)
        for (int w=0; w<width; w++)
            if (GetAlpha(w, h) != ALPHA_TRANSPARENT)
                return false;
    return true;
}

bool CImg::IsFull()
{
    for (int h=0; h<height; h++)
        for (int w=0; w<width; w++)
            if (GetAlpha(w, h) != ALPHA_OPAQUE)
                return false;
    return true;
}

int CImg::GetGridCompleteness()
{
    // treat image without alpha as complete
    if (!HasAlpha())
        return 0xFFFF;
    else
    {
        int gridCompleteness = 0;
        int gridWidth = width / TILE_GRID_X_CNT;
        int gridHeight = height / TILE_GRID_Y_CNT;
        for (int h=0; h<TILE_GRID_Y_CNT; h++)
        {
            for (int w=0; w<TILE_GRID_X_CNT; w++)
            {
                int offset = h * TILE_GRID_X_CNT + w;
                int w1 = w * gridWidth, w2 = (w+1) * gridWidth;
                int h1 = h * gridHeight, h2 = (h+1) * gridHeight;
                bool isGridComplete = true;
                for (int i=w1; i<w2; i++)
                    for (int j=h1; j<h2; j++)
                        if (GetAlpha(i, j) != ALPHA_OPAQUE)
                            isGridComplete = false;
                if (isGridComplete)
                    gridCompleteness |= (1<<offset);
            }
        }
        return gridCompleteness;
    }
}

void CImg::Clear()
{
    pCxImage->Clear();
}

bool CImg::HasAlpha()
{
    return pCxImage->AlphaIsValid();
}

void CImg::ClearAlpha(U8 alpha)
{
    pCxImage->AlphaSet(alpha);
}

void CImg::CreateAlpha()
{
    pCxImage->AlphaCreate();
}

void CImg::PasteTo(CImg *imgDst, int x, int y)
{
    // do nothing if they don't overlap
    if (x <= -width || y <= -height) return;
    if (x >= imgDst->width || y>= imgDst->height) return;

    
    // calc the real coordinates
    int fromx, fromy, fromw, fromh, tox, toy;
    fromx = x<0 ? -x : 0;
    fromy = y<0 ? -y : 0;
    fromw = x<0 ? min(x+width, imgDst->width) : min(width, imgDst->width-x);
    fromh = y<0 ? min(y+height, imgDst->height) : min(height, imgDst->height-y);
    tox = x<0 ? 0 : x;
    toy = y<0 ? 0 : y;

    int pasteBytes = fromw * BYTE_PER_PIXEL;
    for (int h=0; h<fromh; h++)
        memcpy(imgDst->GetPixelPointer(tox, toy+h), this->GetPixelPointer(fromx, fromy+h), pasteBytes);
}

void CImg::Resample(int width, int height)
{
    pCxImage->Resample(width, height);
    this->width = pCxImage->GetWidth();
    this->height = pCxImage->GetHeight();
}

U8* CImg::EncodeAsJpeg(int jpegQuality, int *pBufferLength)
{
    U8 *buffer = NULL;
    pCxImage->SetJpegQuality(jpegQuality);
    pCxImage->Encode(buffer, *pBufferLength, CXIMAGE_FORMAT_JPG);
    return buffer;
}

U8* CImg::EncodeAsPng(int *pBufferLength)
{
    U8 *buffer = NULL;
    pCxImage->Encode(buffer, *pBufferLength, CXIMAGE_FORMAT_PNG);
    return buffer;
}

void CImg::Trim(vector<SMask> *pMasks)
{
    int maskCnt = (int)pMasks->size();
    bool useMask = (maskCnt == 0) ? false : true;
    bool hasVoidPixel = false;

    // skip if is fully covered by SKIP mask, and not covered by any REMOVE mask
    SRectF sourceRect;
    sourceRect.top = sourceRect.left = 0;
    sourceRect.right = width;
    sourceRect.bottom = height;
    bool hasRemove = false;
    for (int i=0; i<maskCnt; i++)
    {
        if (pMasks->at(i).type == 3)
        {
            hasRemove = true;
            break;
        }
    }
    if (!hasRemove)
    {
        for (int i=0; i<maskCnt; i++)
        {
            if (pMasks->at(i).type == 1 && pMasks->at(i).rect.FullyCovers(sourceRect))
                return;
        }
    }

    // allocate status array
    U8 *status = new U8[width * height];  // last bit: isVisited. last but one bit: toRemove. last but two bit: isRemoved
    memset(status, 0, width * height);

    // first, scan entire img and complete toRemove array
    for (int h=0; h<height; h++)
    {
        int offset = h * width;
        for (int w=0; w<width; w++)
        {
            bool SKIP = false, REMOVE = false;
            int NEWRANGE = -1;
            if (useMask)
            {
                for (int i=0; i<maskCnt; i++)
                {
                    // skip if the mask is not effective for this point
                    if (!pMasks->at(i).rect.ContainsPoint(w, h))
                        continue;
                    if (pMasks->at(i).type==1)
                    {
                        SKIP = true;
                        break;
                    }
                    else if (pMasks->at(i).type==2)
                        NEWRANGE = i;
                    else if (pMasks->at(i).type==3)
                    {
                        REMOVE = true;
                        break;
                    }
                }
            }
            if (SKIP)
                continue;
            else if (REMOVE)
            {
                status[offset + w] = 2;
                hasVoidPixel = true;
                continue;
            }
            else // regular or newrange
            {
                U8 *pixel = GetPixelPointer(w, h);
                if (NEWRANGE == -1)
                {
                    if ((pixel[R_Offset]<CConfig::TRIM_PIXEL_VOID_RANGE[0] && pixel[G_Offset]<CConfig::TRIM_PIXEL_VOID_RANGE[1] && pixel[B_Offset]<CConfig::TRIM_PIXEL_VOID_RANGE[2]) ||
                        (pixel[R_Offset]>CConfig::TRIM_PIXEL_VOID_RANGE[3] && pixel[G_Offset]>CConfig::TRIM_PIXEL_VOID_RANGE[4] && pixel[B_Offset]>CConfig::TRIM_PIXEL_VOID_RANGE[5]))
                    {
                        status[offset + w] = 2;
                        hasVoidPixel = true;
                    }
                }
                else
                {
                    if ((pixel[R_Offset]<pMasks->at(NEWRANGE).voidRange[0] && pixel[G_Offset]<pMasks->at(NEWRANGE).voidRange[1] && pixel[B_Offset]<pMasks->at(NEWRANGE).voidRange[2]) ||
                        (pixel[R_Offset]>pMasks->at(NEWRANGE).voidRange[3] && pixel[G_Offset]>pMasks->at(NEWRANGE).voidRange[4] && pixel[B_Offset]>pMasks->at(NEWRANGE).voidRange[5]))
                    {
                        status[offset + w] = 2;
                        hasVoidPixel = true;
                    }
                }
            }
        }
    }

    // return if all pixels are valid
    if (!hasVoidPixel)
    {
        delete[] status;
        return;
    }

    // then, try flood fill on every to remove pixel
    // first count flood fill count, if OK, then do real removal
    hasVoidPixel = false;
    int totalCnt;
    vector<SShortXY> stack;
    for (int h=0; h<height; h++)
    {
        int offset = h * width;
        for (int w=0; w<width; w++)
        {
            // to start flood fill, a point must be toRemove and !isVisited and !isRemoved
            if (status[offset + w] != 2) continue;

            // flood fill
            Trim_Scanline(&stack, w, h, status, totalCnt, false);

            // skip small clusters
            if (totalCnt < CConfig::TRIM_MIN_CLUSTER_SIZE) continue;
            
            // remove area, but first create alpha layer if not created before
            if (hasVoidPixel == false)
            {
                hasVoidPixel = true;
                CreateAlpha();
                ClearAlpha(ALPHA_OPAQUE);
            }
            Trim_Scanline(&stack, w, h, status, totalCnt, true);
        }
    }

    delete[] status;

    // at last, remove noise pixels (has no neighbor, or only one neighbor, or two opposite neighbors)
    if (hasVoidPixel)
    {
        const int xOffset[4] = {0, 1, 0, -1};
        const int yOffset[4] = {1, 0, -1, 0};
        for (int k=0; k<CConfig::TRIM_REMOVE_NOISE; k++)
        {
            int noiseCnt = 0;
            for (int h=0; h<height; h++)
            {
                for (int w=0; w<width; w++)
                {
                    int n = 0;
                    U8 *alpha = GetAlphaPointer(w, h);
                    if (*alpha != ALPHA_OPAQUE) continue;
                    for (int i=0; i<4; i++)
                    {
                        int w1 = w + xOffset[i], h1 = h + yOffset[i];
                        if (w1<0||w1>=width||h1<0||h1>=height)
                            continue;
                        if (GetAlpha(w1, h1) == ALPHA_OPAQUE)
                            n |= 1<<i;
                    }
                    if (n==0 || n==1 || n==2 || n==4 || n==8 || n==5 || n==10)
                    {
                        *alpha = ALPHA_TRANSPARENT;
                        noiseCnt++;
                    }
                }
            }
            if (noiseCnt == 0) break;
        }
    }
}

void CImg::Trim_Scanline(vector<SShortXY> *pStack, int x, int y, U8 *status, int &totalCnt, bool isSetAlpha)
{
    // This scanline algorithm is not optimized to the maximum because the performance here is not very critical, but the low memory usage is the main reason why I use it.
    // If optimize this algorithm to the maximum, the code would be much more complicated and may cause bug.
    pStack->clear();
    totalCnt = 0;
    int valid = isSetAlpha ? 3 : 2;

    // push first element
    SShortXY element;
    element.x = x, element.y = y;
    pStack->push_back(element);

    // scanline
    while (!pStack->empty())
    {
        // pop
        element = pStack->back();
        pStack->pop_back();
        
        // expand to the left and right
        int x1 = element.x, x2 = element.x, offset = element.y * width;
        while(x1 >= 0 && status[offset + x1] == valid) x1--;
        x1++;
        while(x2 < width && status[offset + x2] == valid) x2++;
        x2--;
        if (x1 > element.x || x2 < element.x) continue;

        // expand up and down
        int spanUp = 0, spanDown = 0;
        for (int i=x1; i<=x2; i++)
        {
            status[offset + i] |= isSetAlpha ? 4 : 1;
            totalCnt++;
            if (isSetAlpha)
                *GetAlphaPointer(i, element.y) = ALPHA_TRANSPARENT;

            // expand up
            if(spanUp==0 && element.y > 0 && status[offset - width + i] == valid) 
            {
                SShortXY son;
                son.x = i, son.y = element.y - 1;
                pStack->push_back(son);
                spanUp = 1;
            }
            else if(spanUp==1 && element.y > 0 && status[offset - width + i] != valid)
            {
                spanUp = 0;
            }

            // expand down
            if(spanDown==0 && element.y < height-1 && status[offset + width + i] == valid) 
            {
                SShortXY son;
                son.x = i, son.y = element.y + 1;
                pStack->push_back(son);
                spanDown = 1;
            }
            else if(spanDown==1 && element.y < height-1 && status[offset + width + i] != valid)
            {
                spanDown = 0;
            }
        }
    }
}
