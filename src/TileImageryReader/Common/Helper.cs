using System;
using System.Collections.Generic;
using System.Web;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;
using System.Net;
using System.Text.RegularExpressions;

namespace TileImageryReader
{
    public static class Helper
    {
        public static byte[] GetBlackTile(int tileSize)
        {
            byte[] blackTileBuffer = (byte[])HttpRuntime.Cache["memorycache_blacktile_" + tileSize];
            if (blackTileBuffer == null)
                throw new Exception("Black Tile Buffer not found for size: " + tileSize);
            return blackTileBuffer;            
        }

        public static void Log(string msg)
        {
            try
            {
                File.AppendAllText(Config.LOG_PATH_CSHARP, DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss ") + msg + "\r\n");
            }
            catch (Exception) { }
        }

        public static byte[] HttpGetBytes(string url)
        {
            WebClient wc = new WebClient();
            return wc.DownloadData(url);
        }

        public static byte[] OverlayImages(byte[] imgbufferLower, byte[] imgbufferUpper, int jpegQuality)
        {
            Image imgLower = Image.FromStream(new MemoryStream(imgbufferLower));
            Image imgUpper = Image.FromStream(new MemoryStream(imgbufferUpper));
            Graphics g = Graphics.FromImage(imgLower);
            g.DrawImage(imgUpper, new Point(0, 0));
            MemoryStream finalStream = new MemoryStream();

            // prepare encoder
            Encoder myEncoder = Encoder.Quality;
            EncoderParameters myEncoderParameters = new EncoderParameters(1);
            EncoderParameter myEncoderParameter = new EncoderParameter(myEncoder, jpegQuality);
            myEncoderParameters.Param[0] = myEncoderParameter;


            imgLower.Save(finalStream, GetEncoder(ImageFormat.Jpeg), myEncoderParameters);
            return finalStream.ToArray();
        }

        private static ImageCodecInfo GetEncoder(ImageFormat format)
        {

            ImageCodecInfo[] codecs = ImageCodecInfo.GetImageDecoders();

            foreach (ImageCodecInfo codec in codecs)
            {
                if (codec.FormatID == format.Guid)
                {
                    return codec;
                }
            }
            return null;
        }

        public static double DegreeToRadian(double degree)
        {
            return degree * Math.PI / 180;
        }
    }
}