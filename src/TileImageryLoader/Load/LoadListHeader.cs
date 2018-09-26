using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using TileImagerySystem_CLR;

namespace TileImageryLoader
{
    class LoadListHeader
    {
        public string outputPath = "";
        public MSSourceSetMetaInfo ssmi;

        public LoadListHeader(string headerString)
        {
            try
            {
                // parse string
                Regex reg = new Regex(@"(\S+)\s+(\d+)\D+(\d+)\D+(\d+)\s+(\d+)");
                Match match = reg.Match(headerString);
                if (!match.Success)
                    throw new Exception();
                outputPath = match.Groups[1].Value;
                ssmi.date.year = (ushort)Int32.Parse(match.Groups[2].Value);
                ssmi.date.month = (byte)Int32.Parse(match.Groups[3].Value);
                ssmi.date.day = (byte)Int32.Parse(match.Groups[4].Value);
                ssmi.originId = (ushort)Int32.Parse(match.Groups[5].Value);
            }
            catch (Exception)
            {
                throw new Exception("LoadListHeader: Can not parse LoadList Header from string: " + headerString);
            }
        }

        public LoadListHeader(string outputPath, string dateString, string sourceIdString)
        {
            this.outputPath = outputPath.Trim();
            if (this.outputPath.Contains(" ") || this.outputPath.Length == 0)
                throw new Exception("Output Dir: format error, should be not empty and contains no space");

            Regex reg = new Regex(@"(\d+)/(\d+)/(\d+)");
            Match m = reg.Match(dateString.Trim());
            if (!m.Success)
                throw new Exception("ImageSet Date: format error, should look like 2013/10/03");
            ssmi.date.year = ushort.Parse(m.Groups[1].Value);
            ssmi.date.month = byte.Parse(m.Groups[2].Value);
            ssmi.date.day = byte.Parse(m.Groups[3].Value);

            reg = new Regex(@"(\d+)");
            m = reg.Match(sourceIdString.Trim());
            if (!m.Success)
                throw new Exception("SourceId: format error, should be an integer");
            ssmi.originId = ushort.Parse(m.Groups[1].Value);
        }

        public override string ToString()
        {
            return outputPath + " " + ssmi.date + " " + ssmi.originId;
        }
    }
}
