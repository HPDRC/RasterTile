#pragma once

// length of dat file should be strictly smaller than this value
#define MAX_DATA_FILE_LEN   (1<<30)

// length of image should be strictly smaller than this value
#define MAX_IMG_LEN         MAX_DATA_FILE_LEN

// max possible length of path
#define MAX_PATH_LEN        (256)

// max possible length of origin name
#define MAX_ORIGIN_NAME_LEN (20)

// the extension name of data file
#define DATA_EXT_NAME       ".tdata"

// the extension name of metadata file
#define META_EXT_NAME       ".tmeta"

// indicate there is no next record
#define NONE				(-1)

// define the jpeg quality used to store image, 0 to 100
#define JPEG_QUALITY        (85)

// define generate tiles until this level
#define TOP_TILE_LEVEL      (0)

// define images in this sytem should have how many bytes per pixel. 
// Input images will always be converted into this standard
#define BYTE_PER_PIXEL      (3)
#define BPP                 (BYTE_PER_PIXEL * 8)

// only used in reader. Split a tile into how many grids to patch and generate best available 
#define TILE_GRID_X_CNT  (4)
#define TILE_GRID_Y_CNT  (4)