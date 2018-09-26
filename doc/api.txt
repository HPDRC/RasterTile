# RasterTile APIs

## Introduction
List of APIs:
- bing_tile: return bing projection tiles
- bing_tile_exist: check if a bing projection tile exists
- bing_sourcelist: return a list of sources available within a certain boundary at a certain zoom level
- bing_mosaic_fixed: return bing projection image at a certain resolution (zoom level) with custom size
- bing_mosaic_any: return bing projection image at any resolution with custom size
- wms: provide wms service
- utm: same api as legacy utm service

## Parameters used by all APIs
Parameter   | Value Type | Description
:--------   | :--------- | :----------
server      | String     | mountain.cs.fiu.edu
debug       | bool       | optional, show debug information instead if "debug=true"
source      | string     | optional, filter query by source, case-insensitive. "BEST_AVAILABLE" will be used if this param is missing
date        | string     | optional, filter query by date, in a format like this: 20151007. Tiles from all dates will be used if this param is missing

## ////////////////////////////////// bing_tile API //////////////////////////////////
##### Format
- http://{server}/RasterService/bing_tile.aspx?x={x}&y={y}&z={z}&source={source}&date={date}
    - Return: 256*256 raster image under bing projection

##### Example
- http://mountain.cs.fiu.edu/RasterService/bing_tile.aspx?x=0&y=0&z=0

## ////////////////////////////////// bing_tile_exist API //////////////////////////////////
##### Format
- http://{server}/RasterService/bing_tile_exist.aspx?x={x}&y={y}&z={z}&source={source}&date={date}
    - Return: "1" or "0" indicating whether specified tile exists

##### Example
- http://mountain.cs.fiu.edu/RasterService/bing_tile_exist.aspx?x=0&y=0&z=0

## ////////////////////////////////// bing_sourcelist API //////////////////////////////////
##### Format
- http://{server}/RasterService/bing_sourcelist.aspx?z={z}&bbox={bbox}
	- Return: a list of sources in json format: {"success":true, "error_message":"", "sources":[{"name":"BEST_AVAILABLE", "dates":["20150109", "20150709"]}]}

##### Parameters
Parameter   | Value Type | Description
:--------   | :--------- | :----------
bbox        | string     | bounding box. Format: "top,right,bottom,left", top means latitude of top-left corner of the bbox. When top=bottom and left=right, it returns the sources covering that point.

## ////////////////////////////////// bing_mosaic_fixed API //////////////////////////////////
##### Format
- http://{server}/RasterService/bing_mosaic_fixed.aspx?projection=bing&z={z}&top={top}&left={left}&width={width}&height={height}&source={source}&date={date}
	- Return: a width*height raster mosaic image under bing projection. Note that this API only return images at a certain bing zoom level, but it's faster than the other mosaic API and should always be preferred if zoom level is known.

##### Parameters
Parameter   | Value Type | Description
:--------   | :--------- | :----------
z           | int        | level of requested image
top         | double     | latitude of topleft corner, in signed degree format, range from -90 to 90
left        | double     | longitude of topleft corner, in signed degree format, range from -180 to 180
width       | int        | width of requested image
height      | int        | height of requested image

## ////////////////////////////////// bing_mosaic_any API //////////////////////////////////
##### Format
- http://{server}/RasterService/bing_mosaic_any.aspx?projection=bing&width={width}&height={height}&bbox={bbox}&source={source}&date={date}
	- Return: a width*height raster mosaic image under bing projection. Note that this API return images at any resolution

##### Parameters
Parameter   | Value Type | Description
:--------   | :--------- | :----------
bbox        | string     | bounding box. Format: "xmin,ymin,xmax,ymax", in EPSG 3857 coordinate

## ////////////////////////////////// wms API //////////////////////////////////
##### Format
- http://{server}/RasterService/wms.aspx?request=getcapabilities
	- Return: an xml description file about wms service supported by the server
- http://{server}/RasterService/wms.aspx?request=getmap&width={width}&height={height}&bbox={bbox}
	- Return: a width*height raster mosaic image under bing projection

##### Parameters
Parameter   | Value Type | Description
:--------   | :--------- | :----------
bbox        | string     | bounding box. Format: "xmin,ymin,xmax,ymax", in EPSG 3857 coordinate
