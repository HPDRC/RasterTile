using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using TileImagerySystem_CLR;
using System.IO;

namespace TileImageryLoader
{
    class LoadDraftRecord : IComparable<LoadDraftRecord>
    {
        public MESourceFileType fileType;
        public string filePath;

        public void InitByRecordString(string recordString)
        {
            Regex reg = new Regex(@"^(.*)\s+(.*)");
            Match m = reg.Match(recordString);
            if (!m.Success)
                throw new Exception("LoadDraftRecord: can not parse load record from string");
            fileType = (MESourceFileType)Enum.Parse(typeof(MESourceFileType), m.Groups[1].Value);
            filePath = m.Groups[2].Value;
        }

        public void InitByPath(string imageFilePath)
        {
            filePath = imageFilePath.ToLower();
            if (filePath.EndsWith(".doqq") || filePath.EndsWith(".doq"))
                fileType = MESourceFileType.doqq;
            else if (filePath.EndsWith("~j"))
                fileType = MESourceFileType.doqqc;
            else if (filePath.EndsWith(".tif") || filePath.EndsWith(".tiff"))
                fileType = MESourceFileType.tif;
            else if (filePath.EndsWith(".jpg") || filePath.EndsWith(".jpeg"))
                fileType = MESourceFileType.jpg;
            else if (filePath.EndsWith(".png"))
                fileType = MESourceFileType.png;
            else
                fileType = MESourceFileType.unknown;
        }

        public void FixDoqqHeader(int headerLength, int lineLength)
        {

            if (fileType == MESourceFileType.doqq || fileType == MESourceFileType.doqqc)
            {
                // construct last line
                string endOfHeader = "\nEND_USGS_DOQ_HEADER";
                if (lineLength < endOfHeader.Length + 1)
                    throw new Exception("FixDoqqHeader: line length can not hold end string");
                int paddingLength = lineLength - endOfHeader.Length - 1;
                for (int i=0; i<paddingLength; i++)
                    endOfHeader += " ";
                endOfHeader += "\n";

                try
                {
                    FileStream fs = File.OpenWrite(filePath);
                    fs.Seek(headerLength - endOfHeader.Length, SeekOrigin.Begin);
                    fs.Write(ASCIIEncoding.ASCII.GetBytes(endOfHeader), 0, endOfHeader.Length);
                    fs.Close();

                }
                catch (Exception ex)
                {
                    throw new Exception("FixDoqqHeader: exception when fixing " + filePath + " : " + ex.Message);
                }
            }
        }

        public override string ToString()
        {
            return fileType + " " + filePath;
        }

        public int CompareTo(LoadDraftRecord other)
        {
            return this.filePath.CompareTo(other.filePath);
        }
    }
}
