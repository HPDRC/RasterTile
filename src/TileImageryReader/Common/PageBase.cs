using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace TileImageryReader
{
    public class PageBase : System.Web.UI.Page
    {
        protected delegate void DelegateVoid();

        protected void SafeRunPrintError(DelegateVoid func)
        {
            try
            {
                func();
            }
            catch (Exception ex)
            {
                Log("SafeRunPrintError", ex);
                Response.ContentType = "text/html";
                Response.Output.Write("Exception: " + ex.Message);
            }
        }

        protected void Log(string msg)
        {
            Helper.Log(msg + " Request: " + Request.Url.Query);
        }

        protected void Log(string header, Exception ex)
        {
            Log("Exception: " + header + " Detail: " + ex.Message);
        }

        protected void OutputText(string text)
        {
            Response.ContentType = "text/html";
            Response.Output.Write(text);
        }

        protected void OutputXml(string text)
        {
            Response.ContentType = "text/xml";
            Response.Output.Write(text);
        }

        protected void OutputJson(string text)
        {
            Response.ContentType = "application/json";
            Response.Output.Write(text);
        }
        
        protected void OutputBlackTile(int tileSize)
        {
            Response.Headers.Add("IsTileEmpty", "1");
            Response.ContentType = "image/jpeg";
            Response.Cache.SetExpires(DateTime.Now + new TimeSpan(0, Config.CLIENT_CACHE_BLACK_TILE_MINUTES, 0));
            Response.BinaryWrite(Helper.GetBlackTile(tileSize));
        }

        protected void OutputImage(byte[] buffer, int blackTileSize)
        {
            if (buffer.Length == 0)
                OutputBlackTile(blackTileSize);
            else
            {
                Response.Headers.Add("IsTileEmpty", "0");
                Response.ContentType = "image/jpeg";
                Response.Cache.SetExpires(DateTime.Now + new TimeSpan(0, Config.CLIENT_CACHE_MINUTES, 0));
                Response.BinaryWrite(buffer);
            }
        }
    }
}