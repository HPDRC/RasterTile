using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using TileImagerySystem_CLR;

namespace TileImageryReader
{
    public class ProjectionUtm
    {
        private const int TILE_X = 400;
        private const int TILE_Y = 400;
        private const int UTM_MAX_NORTHING = 13107200;

        private const int maxTileLevel = 31;

        private static double[] resolutionTable = new double[maxTileLevel]
        {
            1<<20, // level 0
            1<<19, 1<<18, 1<<17, 1<<16, 1<<15, 1<<14, 1<<13, 1<<12, 1<<11, 1<<10,   // level 1 to 10
            1<<9, 1<<8, 1<<7, 1<<6, 1<<5, 1<<4, 1<<3, 1<<2, 1<<1, 1<<0,             // level 11 to 20
            1.0/(1<<1), 1.0/(1<<2), 1.0/(1<<3), 1.0/(1<<4), 1.0/(1<<5), 1.0/(1<<6), 1.0/(1<<7), 1.0/(1<<8), 1.0/(1<<9), 1.0/(1<<10), // level 21 to 30
        };

        public static byte ResToLevel(double res)
        {
            for (int i = 0; i < maxTileLevel; i++)
                if (resolutionTable[i] == res)
                    return (byte)i;
            throw new Exception("Resolution: resolution " + res + " can't be converted to level");
        }

        public static uint Transform_YnOld_YnNew(uint oldYn, double currentRes)
        {
            // UTM_MAX_NORTHING - newYn*TILE_Y*res = oldYn*TILE_Y*res
            // so, newYn=(UTM_MAX_NORTHING/TILE_Y/res) - oldYn
            return (uint)(UTM_MAX_NORTHING / TILE_Y / currentRes) - oldYn;
        }
    }
}