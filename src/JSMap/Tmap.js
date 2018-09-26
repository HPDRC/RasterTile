var Tmap = function(latitude, longitude, level, container) {
    var tileSystem = new TileSystem();
    var tiles = new Array();
    var coordinate = new CoordinatePoint(latitude, longitude);
    var lev = level;
    var width = parseInt(container.style.width);
    var height = parseInt(container.style.height);
    var container = container;
    var pixel = tileSystem.LatLongToPixelXY(coordinate.lat, coordinate.lon, lev);
    var bound = new BoundBox();
this.Level=level;


    var GetBound = function() {
        bound.pixel1.pixelX = pixel.pixelX - width / 2;
        bound.pixel1.pixelY = pixel.pixelY - height / 2;
        bound.pixel2.pixelX = pixel.pixelX + width / 2;
        bound.pixel2.pixelY = pixel.pixelY + height / 2;
        bound.coordinate1 = tileSystem.PixelXYToLatLong(bound.pixel1.pixelX, bound.pixel1.pixelY, lev);
        bound.coordinate2 = tileSystem.PixelXYToLatLong(bound.pixel2.pixelX, bound.pixel2.pixelY, lev);
    }


    this.Move = function(dtX, dtY) {
        pixel.pixelX -= dtX;
        pixel.pixelY -= dtY;
        coordinate = tileSystem.PixelXYToLatLong(pixel.pixelX, pixel.pixelY, lev);
        RequestTiles();
    }

    this.MoveTo = function(latitude, longitude) {
        coordinate.lat = latitude;
        coordinate.lon = longitude;
        pixel = tileSystem.LatLongToPixelXY(coordinate.lat, coordinate.lon, lev);
        RequestTiles();
    }

    this.ZoomTo = function(level) {
        if (level < 1 || level > 17) return;
        lev = level;
        pixel = tileSystem.LatLongToPixelXY(coordinate.lat, coordinate.lon, lev);
this.Level=lev;
        RequestTiles();
    }

    var RequestTiles = function() {
        GetBound();
        var t1 = new TileXY();
        var t2 = new TileXY();
        t1 = tileSystem.PixelXYToTileXY(bound.pixel1.pixelX, bound.pixel1.pixelY);
        t2 = tileSystem.PixelXYToTileXY(bound.pixel2.pixelX, bound.pixel2.pixelY);
        for (i = t1.tileX; i <= t2.tileX; i++) {
            for (j = t1.tileY; j <= t2.tileY; j++) {
                var id = i + "," + j + "," + lev;
                var px = i * 256 - bound.pixel1.pixelX;
                var py = j * 256 - bound.pixel1.pixelY;

                var mytile; // = new Tile();
                if (tiles[id]) {
                    mytile = tiles[id];
                    mytile.PX = px;
                    mytile.PY = py;
                    mytile.BitMap.style.left = px + "px";
                    mytile.BitMap.style.top = py + "px";
                }
                else {
                    mytile = new Tile(id, i, j, lev);
                    mytile.PX = px;
                    mytile.PY = py;
                    //var quadKey = tileSystem.TileXYToQuadKey(mytile.TileX, mytile.TileY, mytile.Level);
                    mytile.BitMap = new Image();
                    /*
                    if (mytile.Level < 11)
                        mytile.BitMap.src = "http://cir01.tf.cs.fiu.edu/tile/a.php?tile=a" + quadKey + ".jpeg";
                    else
                        mytile.BitMap.src = "http://terranode-178.cs.fiu.edu/TileImagery/default.aspx?quadkey=" + quadKey;
                    */
                    var mapType = "";
                    switch(mytile.Level)
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
                    		//mapType = "sbsm0210";
                    		mapType = "A0512_EMap"
                    		break;
                    	case 11:
                    		//mapType = "e11";
                    		//break;
                    	case 12:
                    		mapType = "B0627_EMAP";
                    		break;
                    	case 13:
                    		//mapType = "e13";
                    		//break;
                    	case 14:
                    		//mapType = "eastdawnall";
                    		//break;
                    	case 15:
                    	case 16:
                    	case 17:
                    	case 18:
                    		//mapType = "sbsm1518";
                    		mapType = "siwei0608";
                    		break;
                    }
                    var serverID = mytile.TileX - (8*Math.floor(mytile.TileX/8));
                    mytile.BitMap.src = "http://tile"+serverID+".tianditu.com/DataServer?T="+mapType+"&X="+mytile.TileX+"&Y="+mytile.TileY+"&L="+mytile.Level;
                    mytile.BitMap.style.left = px + "px";
                    mytile.BitMap.style.top = py + "px";
                    mytile.BitMap.style.height = "256px";
                    mytile.BitMap.style.width = "256px";
                    mytile.BitMap.style.position = "absolute";
                    mytile.BitMap.onmousedown = function() { return false; };
                    container.appendChild(mytile.BitMap);
                    tiles[id] = mytile;
                }
            }
        }

        //DrawLegend(tg);
    }
    GetBound();
    RequestTiles();
}

var Tile = function(id, tileX, tileY, level) {
    this.ID = id;
    this.TileX = tileX;
    this.TileY = tileY;
    this.Level = level;
    this.BitMap = null;
    this.PX = 0;
    this.PY = 0;
    this.isDownloading = false;
    this.isInUse = false;
}

var BoundBox = function() {
    this.pixel1 = new PixelPoint();
    this.pixel2 = new PixelPoint();
    this.coordinate1 = new CoordinatePoint();
    this.coordinate2 = new CoordinatePoint();
}

var TileSystem = function() {
    //var EarthRadius = 6378137;
    var MinLatitude = -90;
    var MaxLatitude = 90;
    var MinLongitude = -180;
    var MaxLongitude = 180;
    var Clip = function(n, minValue, maxValue) {
        return Math.min(Math.max(n, minValue), maxValue);
    }

    this.MapSize = function(levelOfDetail) {
        return 256 << levelOfDetail;
    }

    this.LatLongToPixelXY = function(latitude, longitude, levelOfDetail) {
        var latitude = Clip(latitude, MinLatitude, MaxLatitude);
        var longitude = Clip(longitude, MinLongitude, MaxLongitude);

        var x = (longitude + 180) / 360;
        var y = (90 - latitude) / 180;
        var mapSize = parseInt(this.MapSize(levelOfDetail));
        var pixelX = parseInt(Clip(x * mapSize + 0.5, 0, mapSize - 1));
        var pixelY = parseInt(Clip(y * (mapSize/2) + 0.5, 0, (mapSize/2) - 1));
        return new PixelPoint(pixelX, pixelY);
    }

    this.PixelXYToLatLong = function(pixelX, pixelY, levelOfDetail) {
        var mapSize = this.MapSize(levelOfDetail);
        var x = (Clip(pixelX, 0, mapSize - 1) / mapSize) - 0.5;
        var y = 0.5 - (Clip(pixelY, 0, mapSize - 1) / mapSize);

        var latitude = 90 - 180 * y;
        var longitude = 360 * x;
        return new CoordinatePoint(latitude, longitude);
    }

    this.PixelXYToTileXY = function(pixelX, pixelY) {
        var tileX = parseInt(pixelX / 256);
        var tileY = parseInt(pixelY / 256);
        return new TileXY(tileX, tileY);
    }

    this.TileXYToPixelXY = function(tileX, tileY) {
        var pixelX = tileX * 256;
        var pixelY = tileY * 512;
        return new PixelPoint(pixelX, pixelY);
    }
		/*
    this.TileXYToQuadKey = function(tileX, tileY, levelOfDetail) {
        var quadKey = "";
        var i = 0;
        for (i = levelOfDetail; i > 0; i--) {
            var digit = '0';
            var mask = 1 << (i - 1);
            if ((tileX & mask) != 0) {
                digit++;
            }
            if ((tileY & mask) != 0) {
                digit++;
                digit++;
            }
            quadKey += digit;
        }
        return quadKey;
    }
    */
}

var CoordinatePoint = function(lat, lon) {
    this.lat = lat;
    this.lon = lon;
}

var PixelPoint = function(pixelX, pixelY) {
    this.pixelX = pixelX;
    this.pixelY = pixelY;
}

var TileXY = function(tileX, tileY) {
    this.tileX = tileX;
    this.tileY = tileY;
}