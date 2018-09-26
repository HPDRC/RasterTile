using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Text.RegularExpressions;
using System.Threading;
using TileImagerySystem_CLR;

namespace TileImageryLoader
{
    /// <summary>
    /// A SourceSet is a set of sources, which contains lots of SourceInfo objects and a SourceSetInfo object
    /// It contains complete information to perform a load
    /// </summary>
    class LoadList
    {
        #region fields

        /// <summary>
        /// header contains common attributes shared by all records in this list
        /// </summary>
        public LoadListHeader header;

        /// <summary>
        /// each record contains full information (file type, path, location) of a source image file
        /// </summary>
        public List<LoadListRecord> records = new List<LoadListRecord>();

        /// <summary>
        /// this loadlist may have special config settings
        /// </summary>
        private string configString = "";

        #endregion

        #region initialize

        /// <summary>
        /// initialize this list from file
        /// </summary>
        /// <param name="filePath">path of the file</param>
        public LoadList(string filePath)
        {
            // clear old records
            records.Clear();

            // read file
            string[] lines = File.ReadAllLines(filePath);

            // first line is header
            header = new LoadListHeader(lines[0]);

            // the rest are loadRecords or configs
            for (int i = 1; i < lines.Length; i++)
            {
                string line = lines[i].Trim();
                if (line.Length == 0) continue;
                if (line[0] == '#')
                    configString += line.Substring(1).Trim() + "\r\n";
                else
                {
                    LoadListRecord rec = new LoadListRecord();
                    rec.InitByRecordString(line);
                    records.Add(rec);
                }
            }
        }

        #endregion

        #region save to file

        /// <summary>
        /// save this list to file
        /// </summary>
        /// <param name="filePath"></param>
        public void Save(string filePath)
        {
            string result = header.ToString() + "\r\n";
            foreach (LoadListRecord record in records)
                result += record + "\r\n";
            File.WriteAllText(filePath, result);
        }

        #endregion

        #region do load
        
        MLoader loader = null;

        /// <summary>
        /// start loading this list
        /// </summary>
        /// <param name="isUtmDest">if dest projection is UTM, zones are loaded seperately into different outputDirs</param>
        public void LoadStart(bool isUtmDest)
        {
            // load config
            if (configString.Length != 0)
                MUtilities.InitConfig(configString);
            if (isUtmDest)
            {
                // get a list of zones
                List<int> zones = new List<int>();
                foreach (LoadListRecord rec in records)
                {
                    if (!zones.Contains(rec.metaInfo.zone))
                        zones.Add(rec.metaInfo.zone);
                }
                FormMain.ShowMessage("Load: number of zones: " + zones.Count);

                // load each zone
                foreach (int zone in zones)
                {
                    FormMain.ShowMessage("Load: start to load zone " + zone);
                    string path = Path.GetFullPath(header.outputPath) + "\\zone " + zone;
                    header.ssmi.outputZone = (byte)zone;
                    List<LoadListRecord> recordsToLoad = new List<LoadListRecord>();
                    foreach (LoadListRecord rec in records)
                    {
                        if (rec.metaInfo.zone == zone)
                            recordsToLoad.Add(rec);
                    }
                    LoadStart(path, recordsToLoad);
                }
            }
            else
            {
                FormMain.ShowMessage("Load: start to load");
                header.ssmi.outputZone = 0; // the zone is ignored in this loading mode
                LoadStart(Path.GetFullPath(header.outputPath), records);
            }
        }

        private void LoadStart(string outputDir, List<LoadListRecord> records)
        {
            try
            {
                // create loader
                loader = new MLoader(outputDir, header.ssmi);
                foreach (LoadListRecord rec in records)
                    loader.AddSource(rec.metaInfo, rec.filePath);

                // create run thread
                Thread thRun = Helper.AsyncRun(delegate
                {
                    try
                    {
                        loader.Load();
                        FormMain.ShowMessage("Load: success!");
                    }
                    catch (Exception ex) { FormMain.ShowMessage(ex.Message); }
                });

                // create monitor thread
                Thread thMonitor = Helper.AsyncRun(delegate
                {
                    MSLoaderStat stat;
                    DateTime startTime = DateTime.Now;
                    while (true)
                    {
                        Thread.Sleep(500);

                        // get loader stat
                        stat = loader.GetStat();

                        // estimate time
                        double realSeondsElapsed = (DateTime.Now - startTime).TotalSeconds;
                        int secondsElapsed = (int)realSeondsElapsed;
                        int secondsLeft = (int)(realSeondsElapsed * (stat.tileTotalCnt - stat.tileOutputedCnt) / Math.Max(stat.tileOutputedCnt, 1));
                        int secondsTotal = Math.Max(1, secondsElapsed + secondsLeft);
                        FormMain.ShowLoaderStatus("Time Elapsed " + Helper.SecondsToString(secondsElapsed)
                            + " Time Left " + Helper.SecondsToString(secondsLeft)
                            + " Time Total " + Helper.SecondsToString(secondsTotal)
                            + "\r\n" + stat.ToString()
                            + "\r\nSpeed: " + (int)(stat.tileTotalCnt / secondsTotal) + " tiles/sec");

                        if (!thRun.IsAlive)
                            return;
                    }
                });

                // wait for threads to end
                thRun.Join();
                thMonitor.Join();

                // close loader
                loader.Close();
                loader = null;
                FormMain.ShowMessage("Load: close success!");
            }
            catch (Exception ex)
            {
                FormMain.ShowMessage(ex);
            }
        }

        /// <summary>
        /// abort the loading procedure
        /// </summary>
        public void LoadAbort()
        {
            if (loader == null)
                return;
            else
                loader.Abort();
        }

        #endregion

        #region correct the list using old index

        public void CorrectByOldIndex(Dictionary<string, OldIndexRecord> oldRecords)
        {
            int totalCnt = 0;
            int processedCnt = 0;
            int correctedCnt = 0;
            foreach (LoadListRecord newRecord in records)
            {
                totalCnt++;
                if (oldRecords.ContainsKey(newRecord.filePath))
                {
                    processedCnt++;
                    OldIndexRecord oldRecord = oldRecords[newRecord.filePath];
                    if (newRecord.metaInfo.x != oldRecord.x || newRecord.metaInfo.y != oldRecord.y
                        || newRecord.metaInfo.width != oldRecord.width || newRecord.metaInfo.height != oldRecord.height)
                    {
                        correctedCnt++;
                        newRecord.metaInfo.x = oldRecord.x;
                        newRecord.metaInfo.y = oldRecord.y;
                        newRecord.metaInfo.width = oldRecord.width;
                        newRecord.metaInfo.height = oldRecord.height;
                    }
                }
            }
            FormMain.ShowMessage("Corrected count: " + correctedCnt + "/" + totalCnt + (correctedCnt == 0 ? "" : " <-- CHECK"));
            FormMain.ShowMessage("Processed count: " + processedCnt + "/" + totalCnt);
        }

        #endregion
    }
}
