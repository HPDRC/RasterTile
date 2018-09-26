using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using TileImagerySystem_CLR;

namespace TileImageryReader
{
    public class Param
    {
        private HttpRequest request;
        
        /// <summary>
        /// do not return tile, return debug information instead
        /// </summary>
        public bool debug;

        public Param(HttpRequest request)
        {
            this.request = request;
            debug = (request["debug"] != null && request["debug"].ToLower()=="true") ? true : false;
        }

        protected string GetStringParam(string paramName)
        {
            string paramValue = request[paramName];
            if (paramValue == null)
                throw new Exception("Param: " + paramName + " is missing");
            return paramValue;
        }

        protected int GetIntParam(string paramName)
        {
            string paramValue = GetStringParam(paramName);

            try
            {
                return Int32.Parse(paramValue);
            }
            catch (Exception)
            {
                throw new Exception("Param: " + paramName + " must be an integer");
            }
        }

        protected byte GetByteParam(string paramName)
        {
            string paramValue = GetStringParam(paramName);

            try
            {
                return byte.Parse(paramValue);
            }
            catch (Exception)
            {
                throw new Exception("Param: " + paramName + " must be an byte value");
            }
        }

        protected uint GetUIntParam(string paramName)
        {
            string paramValue = GetStringParam(paramName);

            try
            {
                return uint.Parse(paramValue);
            }
            catch (Exception)
            {
                throw new Exception("Param: " + paramName + " must be a non-negative integer");
            }
        }

        protected double GetDoubleParam(string paramName)
        {
            string paramValue = GetStringParam(paramName);

            try
            {
                return double.Parse(paramValue);
            }
            catch (Exception)
            {
                throw new Exception("Param: " + paramName + " must be a double number");
            }
        }
    }
}