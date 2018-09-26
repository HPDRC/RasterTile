using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace TileImageryLoader
{
    class LoadDraft
    {
        public List<LoadDraftRecord> records = new List<LoadDraftRecord>();

        /// <summary>
        /// init load draft by scanning a directory and judge every file's type
        /// </summary>
        /// <param name="dirToScan"></param>
        /// <param name="pathEndsWith">file path should end with this string, empty string means ignore this filter</param>
        /// <param name="only1SubDir">only scan *.1 sub directory under the main directory</param>
        /// <returns></returns>
        public LoadDraft(string dirToScan, string pathEndsWith, bool only1SubDir)
        {
            // scan dir
            List<string> paths = new List<string>();
            if (!only1SubDir)
                Helper.GetAllFilesEndsWith(dirToScan, paths, pathEndsWith.Trim());
            else
            {
                DirectoryInfo di = new DirectoryInfo(dirToScan);
                foreach (DirectoryInfo subDir in di.GetDirectories())
                {
                    if (subDir.Name.EndsWith(".1"))
                        Helper.GetAllFilesEndsWith(subDir.FullName, paths, pathEndsWith.Trim());
                }
            }

            // judge type by extension name
            foreach (string path in paths)
            {
                LoadDraftRecord rec = new LoadDraftRecord();
                rec.InitByPath(path);
                records.Add(rec);
            }

            records.Sort();
        }

        public LoadDraft(string filePath)
        {
            foreach (string line in File.ReadAllLines(filePath))
            {
                if (line.Trim().Length == 0)
                    continue;
                LoadDraftRecord rec = new LoadDraftRecord();
                rec.InitByRecordString(line);
                records.Add(rec);
            }
        }

        /// <summary>
        /// save load draft to file
        /// </summary>
        public void Save(string savePath)
        {
            List<string> lines = new List<string>();
            foreach (LoadDraftRecord rec in records)
                lines.Add(rec.ToString());
            File.WriteAllLines(savePath, lines.ToArray());
        }
    }
}
