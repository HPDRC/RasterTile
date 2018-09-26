using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.IO;
using System.Threading;
using System.Windows.Forms;
using TileImagerySystem_CLR;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;

namespace TileImageryLoader
{
    static class Helper
    {
        public static string[] GetLines(string str)
        {
            return str.Split(new char[] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);
        }

        public static string SecondsToString(int seconds)
        {
            return (seconds / 3600).ToString("D2") + ":" + ((seconds / 60) % 60).ToString("D2") + ":" + (seconds % 60).ToString("D2");
        }

        /// <summary>
        /// get all files with specified extension under a directory, when endWith==null || endWith=="", ignore this condition
        /// </summary>
        public static void GetAllFilesEndsWith(string dirPath, List<string> result, string endWith)
        {
            endWith = endWith.ToLower().Trim();
            DirectoryInfo di = new DirectoryInfo(dirPath);
            foreach (FileInfo fi in di.GetFiles())
            {
                if (endWith == null || endWith.Length == 0 || fi.FullName.ToLower().EndsWith(endWith))
                    result.Add(fi.FullName);
            }
            foreach (DirectoryInfo subdi in di.GetDirectories())
            {
                GetAllFilesEndsWith(subdi.FullName, result, endWith);
            }
        }

        public static string GetCurrentDir()
        {
            string path = Application.ExecutablePath;
            return path.Substring(0, path.LastIndexOf("\\") + 1);
        }

        public static Thread AsyncRun(MethodInvoker mi)
        {
            Thread th = new Thread(new ThreadStart(delegate
            {
                try
                {
                    mi.Invoke();
                }
                catch (Exception ex) { FormMain.ShowMessage(ex); }
            }));
            th.IsBackground = true;
            th.Start();
            return th;
        }

        public static void SaveImageMultiColor(Image image, string path)
        {
            ImageAttributes imageAttributes = new ImageAttributes();
            int width = image.Width;
            int height = image.Height;
            Bitmap bm = new Bitmap(width * 2, height * 3);
            Graphics g = Graphics.FromImage(bm);

            // rgb
            g.DrawImageUnscaled(image, 0, 0);

            // rbg
            float[][] rbg = {new float[] {1, 0, 0, 0, 0},
                             new float[] {0, 0, 1, 0, 0},
                             new float[] {0, 1, 0, 0, 0},
                             new float[] {0, 0, 0, 1, 0},
                             new float[] {0, 0, 0, 0, 1} };
            imageAttributes.SetColorMatrix(new ColorMatrix(rbg), ColorMatrixFlag.Default, ColorAdjustType.Bitmap);
            g.DrawImage(image, new Rectangle(width, 0, width, height), 0, 0, width, height, GraphicsUnit.Pixel, imageAttributes);

            // grb
            float[][] grb = {new float[] {0, 1, 0, 0, 0},
                             new float[] {1, 0, 0, 0, 0},
                             new float[] {0, 0, 1, 0, 0},
                             new float[] {0, 0, 0, 1, 0},
                             new float[] {0, 0, 0, 0, 1} };
            imageAttributes.SetColorMatrix(new ColorMatrix(grb), ColorMatrixFlag.Default, ColorAdjustType.Bitmap);
            g.DrawImage(image, new Rectangle(0, height, width, height), 0, 0, width, height, GraphicsUnit.Pixel, imageAttributes);

            // gbr
            float[][] gbr = {new float[] {0, 0, 1, 0, 0},
                             new float[] {1, 0, 0, 0, 0},
                             new float[] {0, 1, 0, 0, 0},
                             new float[] {0, 0, 0, 1, 0},
                             new float[] {0, 0, 0, 0, 1} };
            imageAttributes.SetColorMatrix(new ColorMatrix(gbr), ColorMatrixFlag.Default, ColorAdjustType.Bitmap);
            g.DrawImage(image, new Rectangle(width, height, width, height), 0, 0, width, height, GraphicsUnit.Pixel, imageAttributes);

            // brg
            float[][] brg = {new float[] {0, 1, 0, 0, 0},
                             new float[] {0, 0, 1, 0, 0},
                             new float[] {1, 0, 0, 0, 0},
                             new float[] {0, 0, 0, 1, 0},
                             new float[] {0, 0, 0, 0, 1} };
            imageAttributes.SetColorMatrix(new ColorMatrix(brg), ColorMatrixFlag.Default, ColorAdjustType.Bitmap);
            g.DrawImage(image, new Rectangle(0, height*2, width, height), 0, 0, width, height, GraphicsUnit.Pixel, imageAttributes);

            // bgr
            float[][] bgr = {new float[] {0, 0, 1, 0, 0},
                             new float[] {0, 1, 0, 0, 0},
                             new float[] {1, 0, 0, 0, 0},
                             new float[] {0, 0, 0, 1, 0},
                             new float[] {0, 0, 0, 0, 1} };
            imageAttributes.SetColorMatrix(new ColorMatrix(bgr), ColorMatrixFlag.Default, ColorAdjustType.Bitmap);
            g.DrawImage(image, new Rectangle(width, height*2, width, height), 0, 0, width, height, GraphicsUnit.Pixel, imageAttributes);

            // save
            bm.Save(path, ImageFormat.Png);
        }

        public static int LongitudeToUtmZone(double longitude)
        {
            return ((int)(Math.Floor((longitude + 180) / 6))) % 60 + 1;
        }
    }
}
