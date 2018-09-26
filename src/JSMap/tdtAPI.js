////////////////////////////////////////////////////////////
//
// Project: Tianditu API
// Author:  Silk_Blade@CJDBY
//
////////////////////////////////////////////////////////////

var REIFITNEI_PAM_LABOLG_TDT = 0; //reverse of "TDT_GLOBAL_MAP_IDENTIFIER". provide a unique ID to each Tianditu Map... ^_^
function tdtMap(containerID, initialLongtitude, initialLatitude, initialLevel)
{
	//Public consts
	//// Map types
	this.MapType = new Object();
	this.MapType.VECTOR = function (){return "tdtVectorMap"};
	this.MapType.IMAGE = function (){return "tdtImageMap"};
	//// Tile size
	this.TILE_SIZE = 256;
	//// Mouse events
	this.MouseEvent = new Object();
	this.MouseEvent.MOUSE_DOWN = function (){return "tdtMouseDown"};
	this.MouseEvent.MOUSE_UP = function (){return "tdtMouseUp"};
	this.MouseEvent.MOUSE_MOVE = function (){return "tdtMouseMove"};
	this.MouseEvent.MOUSE_CLICK = function (){return "tdtMouseClick"};
	this.MouseEvent.MOUSE_DOUBLE_CLICK = function (){return "tdtMouseDoubleClick"};
	
	//Private consts
	var LNG_MIN = -180.0, LNG_MAX = 180.0;
	var LAT_MIN = -90.0, LAT_MAX = 90.0;
	var LV_MIN = 2; LV_MAX = 18;
	
	//Private varities
	var root = this;
	
	var _isIE = document.all ? true : false;
	
	var _objMapContainer;		//Map container object
	var _containerX, _containerY;
	var _objTileContainer;		//Tile container, contains all 256x256 tiles
	var _objPanelContainer;		//Put all control panels here.
	
	var _centerLng, _centerLat;			//Coordinate of map center
	var _mapLevel;						//Current map zoom level
	var _mapWidth = 0, _mapHeight = 0;	//Current map size
	var _mapType = this.MapType.VECTOR;	//Current map type. { this.MapType.VECTOR | this.MapType.IMAGE }, DEFAULT=this.MapType.VECTOR
	var _mapCenterPixelX, _mapCenterPixelY;	//Map center in pixel
	
	var _currentTiles = new Array();	//Contains displayed tiles
	
	var _mouseEvents = new Object();	//Contains callback functions of mouse events
	_mouseEvents.EVENTS_MOUSE_DOWN = new Array();
	_mouseEvents.EVENTS_MOUSE_UP = new Array();
	_mouseEvents.EVENTS_MOUSE_MOVE = new Array();
	_mouseEvents.EVENTS_MOUSE_CLICK = new Array();
	_mouseEvents.EVENTS_MOUSE_DOUBLE_CLICK = new Array();
	
	var _mapOnDragInterval = -1;
	var _mapMouseStartX, _mapMouseStartY;
	var _mapMouseX, _mapMouseY;
	
	//Constructor
	Initial();
	function Initial()
	{
		//Get initial map parameters
		_objMapContainer = document.getElementById(containerID);	//Get map container object
		_containerX = _objMapContainer.offsetLeft;
		_containerY = _objMapContainer.offsetTop;
		_centerLng = Clip(initialLongtitude, LNG_MIN, LNG_MAX);	//Get map center coordinate
		_centerLat = Clip(initialLatitude, LAT_MIN, LAT_MAX);
		_mapLevel = Clip(initialLevel, LV_MIN, LV_MAX);			//Get map zoom level
		
		//Get map size
		_mapWidth = parseInt( _objMapContainer.style.width );
		_mapHeight = parseInt( _objMapContainer.style.height );
		
		//Create empty map
		CreateMapDOM();
		
		//Fill map DOM with map tiles
		RedrawMap(_centerLng, _centerLat, _mapLevel);
	}
	
	//Public functions
	this.SetMapCenter = function(inLng, inLat, inLevel)
	{
		RedrawMap(inLng, inLat, inLevel);
	}
	
	this.MoveOffset = function (xoffset, yoffset)
	{
		_mapCenterPixelX += xoffset;
		_mapCenterPixelY += yoffset;
		
		var newCenterLatlng = PixelXYToLatLong(_mapCenterPixelX, _mapCenterPixelY, _mapLevel);
		_centerLng = newCenterLatlng.longitude;
		_centerLat = newCenterLatlng.latitude;
		
		RedrawMapByPixel(_mapCenterPixelX, _mapCenterPixelY);
	}
	
	this.ZoomTo = function (inLevel)
	{
		RedrawMap(_centerLng, _centerLat, inLevel);
	}
	
	this.GetLevel = function()
	{
		return _mapLevel;
	}
	
	this.SetMapType = function(newType)
	{
		if(newType == root.MapType.VECTOR)
		{
			_mapType = root.MapType.VECTOR;
			RedrawMapByPixel(_mapCenterPixelX, _mapCenterPixelY);
		}
		else if(newType == root.MapType.IMAGE)
		{
			_mapType = root.MapType.IMAGE;
			RedrawMapByPixel(_mapCenterPixelX, _mapCenterPixelY);
		}
	}
	
	this.AddEventListener = function( eventType, eventCallbackFunction )
	{
		switch(eventType)
		{
			case this.MouseEvent.MOUSE_DOWN:
				AddEvent(_mouseEvents.EVENTS_MOUSE_DOWN, eventCallbackFunction);
				break;
			case this.MouseEvent.MOUSE_UP:
				AddEvent(_mouseEvents.EVENTS_MOUSE_UP, eventCallbackFunction);
				break;
			case this.MouseEvent.MOUSE_MOVE:
				AddEvent(_mouseEvents.EVENTS_MOUSE_MOVE, eventCallbackFunction);
				break;
			case this.MouseEvent.MOUSE_CLICK:
				AddEvent(_mouseEvents.EVENTS_MOUSE_CLICK, eventCallbackFunction);
				break;
			case this.MouseEvent.MOUSE_DOUBLE_CLICK:
				AddEvent(_mouseEvents.EVENTS_MOUSE_DOUBLE_CLICK, eventCallbackFunction);
				break;
		}
	}
	
	this.RemoveEventListener = function( eventType, eventCallbackFunction )
	{
		switch(eventType)
		{
			case this.MouseEvent.MOUSE_DOWN:
				RemoveEvent(_mouseEvents.EVENTS_MOUSE_DOWN, eventCallbackFunction);
				break;
			case this.MouseEvent.MOUSE_UP:
				RemoveEvent(_mouseEvents.EVENTS_MOUSE_UP, eventCallbackFunction);
				break;
			case this.MouseEvent.MOUSE_MOVE:
				RemoveEvent(_mouseEvents.EVENTS_MOUSE_MOVE, eventCallbackFunction);
				break;
			case this.MouseEvent.MOUSE_CLICK:
				RemoveEvent(_mouseEvents.EVENTS_MOUSE_CLICK, eventCallbackFunction);
				break;
			case this.MouseEvent.MOUSE_DOUBLE_CLICK:
				RemoveEvent(_mouseEvents.EVENTS_MOUSE_DOUBLE_CLICK, eventCallbackFunction);
				break;
		}
	}
	
	this.StartDrag = function()
	{
		if(-1 == _mapOnDragInterval)
		{
			_mapMouseStartX = _mapMouseX;
			_mapMouseStartY = _mapMouseY;
			_mapOnDragInterval = window.setInterval(MapFollowMouse, 20);
		}
	}
	
	this.StopDrag = function()
	{
		window.clearInterval(_mapOnDragInterval);
		_mapOnDragInterval = -1;
	}
	
	//Private functions
	function CreateMapDOM()
	{
		//Create internal div
		var tileContailerString = "<div id='tdtInternalDiv_TileContainer" + REIFITNEI_PAM_LABOLG_TDT + "' class='tdtTileContainer'" + "style='width:" + _mapWidth + "; height:" + _mapHeight + "'></div>";
		var panelContailerString = "<div id='tdtInternalDiv_PanelContainer" + REIFITNEI_PAM_LABOLG_TDT + "' class='tdtPanelContainer'" + "style='max-width:" + _mapWidth + "; max-height:" + _mapHeight + "'></div>";
		
		_objMapContainer.innerHTML = tileContailerString + panelContailerString;
		
		//Get internal div objects
		_objTileContainer = document.getElementById("tdtInternalDiv_TileContainer" + REIFITNEI_PAM_LABOLG_TDT);
		_objPanelContainer = document.getElementById("tdtInternalDiv_PanelContainer" + REIFITNEI_PAM_LABOLG_TDT);
		
		//create MOUSE EVENT receivers
		_objTileContainer.onmousedown = MapOnMouseDownListener;
		_objTileContainer.onmouseup = MapOnMouseUpListener;
		_objTileContainer.onmousemove = (_isIE)?MapOnMouseMoveListener_IE:MapOnMouseMoveListener;
		_objTileContainer.onclick = MapOnMouseClickListener;
		_objTileContainer.ondblclick = MapOnMouseDoubleClickListener;
		
		//update ID
		REIFITNEI_PAM_LABOLG_TDT++;
		
		//say something :)
		_objPanelContainer.innerHTML = "tdtAPI - By Silk_Blade@CJDBY";
	}
	
	////MOUSE EVENT receivers
	function MapOnMouseDownListener(e)
	{
		if(null != _mouseEvents.EVENTS_MOUSE_DOWN)
		{
			LoadMouseEvent(_mouseEvents.EVENTS_MOUSE_DOWN, e);
		}
	}
	
	function MapOnMouseUpListener(e)
	{
		if(null != _mouseEvents.EVENTS_MOUSE_UP)
		{
			LoadMouseEvent(_mouseEvents.EVENTS_MOUSE_UP, e);
		}
	}
	
	function MapOnMouseMoveListener(e)
	{
		if(null != _mouseEvents.EVENTS_MOUSE_MOVE)
		{
			LoadMouseEvent(_mouseEvents.EVENTS_MOUSE_MOVE, e);
		}
		
		_mapMouseX = e.clientX - _containerX;
		_mapMouseY = e.clientY - _containerY;
	}
	
	function MapOnMouseMoveListener_IE()
	{
		if(null != _mouseEvents.EVENTS_MOUSE_MOVE)
		{
			LoadMouseEvent(_mouseEvents.EVENTS_MOUSE_MOVE, event);
		}
		
		_mapMouseX = event.clientX - _containerX;
		_mapMouseY = event.clientY - _containerY;
	}
	
	function MapOnMouseClickListener(e)
	{
		if(null != _mouseEvents.EVENTS_MOUSE_CLICK)
		{
			LoadMouseEvent(_mouseEvents.EVENTS_MOUSE_CLICK, e);
		}
	}
	
	function MapOnMouseDoubleClickListener(e)
	{
		if(null != _mouseEvents.EVENTS_MOUSE_DOUBLE_CLICK)
		{
			LoadMouseEvent(_mouseEvents.EVENTS_MOUSE_DOUBLE_CLICK, e);
		}
	}
	
	function MapFollowMouse()
	{
		root.MoveOffset(_mapMouseStartX - _mapMouseX, _mapMouseStartY - _mapMouseY);
		_mapMouseStartX = _mapMouseX;
		_mapMouseStartY = _mapMouseY;
	}
	
	////Map and coordinate related functions
	function RedrawMap(inLng, inLat, inLevel)
	{
		//tests
		//_objPanelContainer.innerHTML = "<p>longitude=" + _centerLng + ", latitude=" + _centerLat + ", level=" + _mapLevel + ", size:" + _mapWidth + "x" + _mapHeight +"</p>";
		//_objPanelContainer.innerHTML = '<input id="btnZoomOut" type="button" onclick="" value="-" style="width:30px; height:30px"/>';
		_centerLng = Clip(inLng, LNG_MIN, LNG_MAX);
		_centerLat = Clip(inLat, LAT_MIN, LAT_MAX);
		_mapLevel = Clip(inLevel, LV_MIN, LV_MAX);
		
		var mapCenterPixel = LatLongToPixelXY(_centerLat, _centerLng, _mapLevel);
		RedrawMapByPixel(mapCenterPixel.pixelX, mapCenterPixel.pixelY);
	}
	
	function RedrawMapByPixel(centerPx, centerPy)
	{
		//get the tiles cover the map
		var neededTiles = GetTiles(centerPx, centerPy, _mapLevel, _mapWidth, _mapHeight);
		//_objPanelContainer.innerHTML += "<p>Got neededTiles:" + neededTiles.length + "</p>";
		//compare with current tiles, ADD new tiles, REMOVE invisible tiles
		var n,c;
		var isFound;
		var tileObj;
		////reset current tiles' "reuse" status
		for(c=0; c<_currentTiles.length; c++)
		{
			_currentTiles[c].reuse = false;
		}
		////compare, reuse old tile / add new tile
		for(n=0; n<neededTiles.length; n++)
		{
			isFound = false;
			for(c=0; c<_currentTiles.length; c++)
			{
				if(neededTiles[n].tid == _currentTiles[c].tid)
				{
					//reuse old tile
					tileObj = document.getElementById(neededTiles[n].tid+"_A");
					tileObj.style.left = neededTiles[n].Px + "px";
					tileObj.style.top = neededTiles[n].Py + "px";
					
					tileObj = document.getElementById(neededTiles[n].tid+"_B");
					tileObj.style.left = neededTiles[n].Px + "px";
					tileObj.style.top = neededTiles[n].Py + "px";
					//set flags
					_currentTiles[c].reuse = true;
					isFound = true;
					break;
				}
			}
			if(!isFound) // if no reuseable tile has been found ...
			{
				//create new tile
				//Background
				//_objPanelContainer.innerHTML += "<p>Add new tile: " + neededTiles[n].imgAddress + "</p>";
				tileObj = new Image();
				tileObj.id = neededTiles[n].tid + "_B";
				tileObj.src = neededTiles[n].imgBackground;
				tileObj.style.left = neededTiles[n].Px + "px";
				tileObj.style.top = neededTiles[n].Py + "px";
				tileObj.style.height = "256px";
				tileObj.style.width = "256px";
				tileObj.style.zIndex = 2;
				tileObj.style.position = "absolute";
				tileObj.style.overflow = "hidden";
				tileObj.onmousedown = function() {return false;};
				tileObj.onmousemove = function() {return false;};
				_objTileContainer.appendChild(tileObj);
				
				//Address
				tileObj = new Image();
				tileObj.id = neededTiles[n].tid + "_A";
				tileObj.src = neededTiles[n].imgAddress;
				tileObj.style.left = neededTiles[n].Px + "px";
				tileObj.style.top = neededTiles[n].Py + "px";
				tileObj.style.height = "256px";
				tileObj.style.width = "256px";
				tileObj.style.zIndex = 3;
				tileObj.style.position = "absolute";
				tileObj.style.overflow = "hidden";
				tileObj.onmousedown = function() {return false;};
				tileObj.onmousemove = function() {return false;};
				_objTileContainer.appendChild(tileObj);
			}	
		}
		//// remove useless tiles
		for(c=0; c<_currentTiles.length; c++)
		{
			if(!(_currentTiles[c].reuse))
			{
				tileObj = document.getElementById(_currentTiles[c].tid+"_A");
				_objTileContainer.removeChild(tileObj);
				tileObj = document.getElementById(_currentTiles[c].tid+"_B");
				_objTileContainer.removeChild(tileObj);
			}
		}
		//
		_currentTiles = neededTiles;
		//record center point position
		_mapCenterPixelX = centerPx;
		_mapCenterPixelY = centerPy;
	}
	
	function GetTiles(centerPx, centerPy, mapLevel, mapWidth, mapHeight)
	{
		var tilesArray = new Array();
		
		var pixelLeftTop = new PixelXY( centerPx - mapWidth/2, centerPy - mapHeight/2);
		var tileLeftTop = PixelXYToTileXY( pixelLeftTop.pixelX, pixelLeftTop.pixelY);
		
		var pixRightBottom = new PixelXY(centerPx + mapWidth/2, centerPy + mapHeight/2);
		var tileRightBottom = PixelXYToTileXY( pixRightBottom.pixelX, pixRightBottom.pixelY);
		
		//test
		//_objPanelContainer.innerHTML += "<p>X:"+tileLeftTop.tileX + "~" + tileRightBottom.tileX + ", Y:" + tileLeftTop.tileY + "~" + tileRightBottom.tileY + "</p>";
		
		for(var tileXN = tileLeftTop.tileX; tileXN <= tileRightBottom.tileX; tileXN++)
		{
			for(var tileYN = tileLeftTop.tileY; tileYN <= tileRightBottom.tileY; tileYN++)
			{
				//_objPanelContainer.innerHTML += "<p>new tile: XN=" + tileXN + ", YN=" + tileYN + "</p>";
				var tileinfo = new TileInfo(tileXN, tileYN, _mapLevel, _mapType, centerPx, centerPy, mapWidth, mapHeight);
				tilesArray.push(tileinfo);
			}
		}
		return tilesArray;
	}
	
	function MapSize (levelOfDetail)
	{
		return 256 << levelOfDetail;
	}
	
	function LatLongToPixelXY (latitude, longitude, levelOfDetail)
	{
		var latitude = Clip(latitude, LAT_MIN, LAT_MAX);
		var longitude = Clip(longitude, LNG_MIN, LNG_MAX);

		var x = (longitude + 180) / 360;
		var y = (90 - latitude) / 180;

		var mapSize = parseInt(MapSize(levelOfDetail));
		var pixelX = parseInt(Clip(x * mapSize + 0.5, 0, mapSize - 1));
		var pixelY = parseInt(Clip(y * (mapSize/2) + 0.5, 0, (mapSize/2) - 1));

		return new PixelXY(pixelX, pixelY);
	}
	
	function PixelXYToLatLong (pixelX, pixelY, levelOfDetail)
	{
		var mapSize = MapSize(levelOfDetail);
		var x = Clip(pixelX, 0, mapSize - 1) / mapSize;
		var y = Clip(pixelY, 0, mapSize/2 - 1) / (mapSize/2);

		var latitude = 90 - 180 * y;
		var longitude = 360 * x - 180;
		
		return new GDC(latitude, longitude);
	}
	
	function ScreenXYToLatLong(pixelX, pixelY)
	{
		var mapPixelX = _mapCenterPixelX - (_mapWidth/2) + pixelX;
		var mapPixelY = _mapCenterPixelY - (_mapHeight/2) + pixelY;
		return PixelXYToLatLong (mapPixelX, mapPixelY, _mapLevel);
	}
	
	function PixelXYToTileXY (pixelX, pixelY)
	{
		var tileX = parseInt(pixelX / 256);
		var tileY = parseInt(pixelY / 256);
		
		return new TileXY(tileX, tileY);
	}

	function TileXYToPixelXY (tileX, tileY)
	{
		var pixelX = tileX * 256;
		var pixelY = tileY * 256;
		
		return new PixelXY(pixelX, pixelY);
	}
	
	function GetAddressImageTString(mapType, level)
	{
		var t;
		if(mapType == root.MapType.VECTOR)
		{
			switch(level)
			{
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
					t="AB0512_Anno";
					break;
				case 11:
				case 12:
					t="B0627_EMap1112";
					break;
				case 13:
				case 14:
				case 15:
				case 16:
				case 17:
				case 18:
					t="siwei0608";
					break;
				default:
					t=false;
					break;
			}
		}
		else if(mapType == root.MapType.IMAGE)
		{
			switch(level)
			{
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
					t="A0610_ImgAnno";
					break;
				case 11:
				case 12:
				case 13:
				case 14:
					t="B0530_eImgAnno";
					break;
				case 15:
				case 16:
				case 17:
				case 18:
					t="siweiAnno68";
					break;
				default:
					t=false;
					break;
			}
		}
		return t;
	}
	
	function GetBackgroundImageTString(mapType, level)
	{
		var t;
		if(mapType == root.MapType.VECTOR)
		{
			switch(level)
			{
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
					t="A0512_EMap";
					break;
				case 11:
				case 12:
					t="B0627_EMap1112";
					break;
				case 13:
				case 14:
				case 15:
				case 16:
				case 17:
				case 18:
					t="siwei0608";
					break;
				default:
					t=false;
					break;
			}
		}
		else if(mapType == root.MapType.IMAGE)
		{
			switch(level)
			{
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
					t="sbsm0210";
					break;
				case 11:
					t="e11";
					break;
				case 12:
					t="e12";
					break;
				case 13:
					t="e13";
					break;
				case 14:
					t="eastdawnall";
					break;
				case 15:
				case 16:
				case 17:
				case 18:
					t="sbsm1518";
					break;
				default:
					t=false;
					break;
			}
		}
		return t;
	}
	
	////Events
	//////General
	function AddEvent( eventsArray, eventCallbackFunction)
	{
		// Ignore the new event if the event is already registered
		var isEventAdded = false;
		for(var i=0; i<eventsArray.length; i++)
		{
			if( eventCallbackFunction == eventsArray[i])
			{
				isEventAdded = true;
				break;
			}
		}
		// Add new event
		if(!isEventAdded)
		{
			eventsArray.push( eventCallbackFunction );
		}
	}
	
	function RemoveEvent( eventsArray, eventCallbackFunction)
	{
		// Search and Destory !
		for(var i=0; i<eventsArray.length; i++)
		{
			if( eventCallbackFunction == eventsArray[i])
			{
				eventsArray.splice(i,1);
				break;
			}
		}
	}
	//////Mouse
	function LoadMouseEvent( eventsArray, e)
	{
		for(var i=0; i<eventsArray.length; i++)
		{
			eventsArray[i]( CreateMouseEventReturner(e) );
		}
	}
	
	function CreateMouseEventReturner( e )
	{
		var ee = (_isIE)?event:e;
		var newEvent = new Object();
		newEvent.X = ee.clientX - _containerX;
		newEvent.Y = ee.clientY - _containerY;
		var tempLatLng = ScreenXYToLatLong(newEvent.X, newEvent.Y);
		newEvent.latitude = tempLatLng.latitude;
		newEvent.longitude = tempLatLng.longitude;
		
		return newEvent;
	}
	//////Keyboard
	
	//Private "Classes"
	function TileInfo(Tx, Ty, L, MapType, centerPx, centerPy, mapW, mapH)
	{
		var server = Tx%8;
		var Toverlay=GetAddressImageTString(MapType, L);
		var Timage=GetBackgroundImageTString(MapType, L);
		
		this.tid = "tdt_" + Tx + "_" + Ty + "@" + L + ":" + MapType;
		
		if(Toverlay)
			this.imgAddress = "http://localhost/TileImageryReader/default.aspx?x="+Tx+"&y="+Ty+"&z="+L;
		else
			this.imgAddress = false;
		if(Timage)
			this.imgBackground = "http://localhost/TileImageryReader/default.aspx?x="+Tx+"&y="+Ty+"&z="+L;
		else
			this.imgBackground = false;
		
		this.Px = Tx*256 - centerPx + (mapW/2);
		this.Py = Ty*256 - centerPy + (mapH/2);
		
		this.reuse = false;
		
		//_objPanelContainer.innerHTML += "<p>create tile:" + this.tid +", "+ Toverlay +"</p>";
	}
	
	function GDC(lat, lng)
	{
		this.latitude = lat;
		this.longitude = lng;
	}

	function PixelXY(pixelX, pixelY)
	{
		this.pixelX = pixelX;
		this.pixelY = pixelY;
	}

	function TileXY(tileX, tileY)
	{
		this.tileX = tileX;
		this.tileY = tileY;
	}
	
	//Private functions - non map related miscs
	function Clip( input, limitMin, limitMax)
	{
		return Math.min( Math.max(input, limitMin), limitMax);
	}
}