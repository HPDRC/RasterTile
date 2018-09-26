using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace TileImageryLoader
{
    class OldIndexRecord
    {
        public double x;
        public double y;
        public int width = 0;
        public int height = 0;
        public string path = "";

        public OldIndexRecord(string recordString)
        {
            Regex reg = new Regex(@"(\\\\[^\\]+\\\S+)\s+([0-9.]+)\s+([0-9.]+)\s+[0-9.]+\s+[0-9.]+\s+[0-9.]+\s+([0-9.]+)\s+([0-9.]+)");
            Match m = reg.Match(recordString);
            if (!m.Success)
                throw new Exception("OldIndexRecord: parse failed");

            path = m.Groups[1].Value.ToLower();
            x = double.Parse(m.Groups[2].Value);
            y = double.Parse(m.Groups[3].Value);
            width = int.Parse(m.Groups[4].Value);
            height = int.Parse(m.Groups[5].Value);
        }
    }
}
