using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using TileImagerySystem_CLR;

namespace TileImageryLoader
{
    /// <summary>
    /// contains full information (file type, path, location) of a source image file
    /// </summary>
    class LoadListRecord
    {
        public MSSourceMetaInfo metaInfo = new MSSourceMetaInfo();

        public string guessedDate = "";

        /// <summary>
        /// is always all lower cases
        /// </summary>
        public string filePath = "";

        public void InitByRecordString(string recordString)
        {
            // fileType path width height zone x y xScale yScale 
            Regex reg = new Regex(@"^(.*)\s+(.*)\s+(.*)\s+(.*)\s+(.*)\s+(.*)\s+(.*)\s+(.*)\s+(.*)\s*$");
            Match m = reg.Match(recordString);
            if (!m.Success)
                throw new Exception("LoadRecord: can not parse load record from string " + recordString);
            metaInfo.fileType = (MESourceFileType)Enum.Parse(typeof(MESourceFileType), m.Groups[1].Value);
            filePath = m.Groups[2].Value.ToLower();
            metaInfo.width = Int32.Parse(m.Groups[3].Value);
            metaInfo.height = Int32.Parse(m.Groups[4].Value);
            metaInfo.zone = Int32.Parse(m.Groups[5].Value);
            metaInfo.x = double.Parse(m.Groups[6].Value);
            metaInfo.y = double.Parse(m.Groups[7].Value);
            metaInfo.xScale = double.Parse(m.Groups[8].Value);
            metaInfo.yScale = double.Parse(m.Groups[9].Value);
        }

        /// <summary>
        /// get utm info from source file and return a guessed date of the image file
        /// </summary>
        /// <param name="filePath"></param>
        /// <param name="fileType"></param>
        /// <returns></returns>
        public void InitByUtmSourceFile(string filePath, MESourceFileType fileType, string srcProjection, string dstProjection)
        {
            this.filePath = filePath.ToLower();
            switch (fileType)
            {
                case MESourceFileType.doqq:
                    MUtilities.ReadUtmMetaFromDoqq(filePath, ref metaInfo, ref guessedDate);
                    metaInfo.fileType = MESourceFileType.doqq;
                    break;
                case MESourceFileType.doqqc:
                    MUtilities.ReadUtmMetaFromDoqq(filePath, ref metaInfo, ref guessedDate);
                    metaInfo.fileType = MESourceFileType.doqqc;
                    break;
                case MESourceFileType.tif:
                    metaInfo = MUtilities.ReadUtmMetaFromTiff(filePath);
                    metaInfo.fileType = MESourceFileType.tif;
                    break;
                default:
                    throw new Exception("Can only get utm info from doqq, doqqc and tif files");
            }

            // convert projection
            if (srcProjection != "")
            {
                MUtilities.TransformProjection(srcProjection, dstProjection, ref metaInfo.x, ref metaInfo.y);
            }

            guessedDate = guessedDate.Replace(" ", "");
            guessedDate = guessedDate.Substring(0, Math.Min(guessedDate.Length, 12));
        }

        public override string ToString()
        {
            return metaInfo.fileType + " " + filePath + " " + metaInfo.width + " " + metaInfo.height + " " + metaInfo.zone
                + " " + metaInfo.x + " " + metaInfo.y + " " + metaInfo.xScale + " " + metaInfo.yScale;
        }
    }
}
