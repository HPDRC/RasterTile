using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace TileImageryLoader
{
    class PixelValidRange
    {
        private int[] range = new int[6];

        public PixelValidRange(string rangeString)
        {
            try
            {
                string[] segments = rangeString.Split(new char[]{','}, StringSplitOptions.RemoveEmptyEntries);
                for (int i = 0; i < 6; i++)
                    range[i] = Int32.Parse(segments[i]);
            }
            catch (Exception)
            {
                throw new Exception("Cannot parse range string: " + rangeString);
            }
        }

        public void DrawValidGraph(Bitmap bm)
        {
            Color invalidColor = Color.Red;
            for (int i = 0; i < bm.Width; i++)
            {
                for (int j = 0; j < bm.Height; j++)
                {
                    Color pixel = bm.GetPixel(i, j);
                    if (!IsPixelValid(pixel))
                        bm.SetPixel(i, j, invalidColor);
                }
            }
        }

        public bool IsPixelValid(Color pixel)
        {
            if (pixel.A == 0)
                return true;
            else
                return ((pixel.R < range[0] && pixel.G < range[1] && pixel.B < range[2]) || (pixel.R > range[3] && pixel.G > range[4] && pixel.B > range[5])) ? false : true;
        }
    }
}
