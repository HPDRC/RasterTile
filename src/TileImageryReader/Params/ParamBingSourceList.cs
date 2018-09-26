using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace TileImageryReader
{
    public class ParamBingSourceList : Param
    {
        public int z;
        public double top, right, bottom, left;

        public ParamBingSourceList(HttpRequest request)
            : base(request)
        {
            z = (int)GetUIntParam("z");
            string[] bbox = GetStringParam("bbox").Split(new char[]{','}, StringSplitOptions.RemoveEmptyEntries);
            if (bbox.Length != 4)
                throw new Exception("Param bbox has a wrong format. Correct format: top,right,bottom,left");
            try
            {
                top = Helper.DegreeToRadian(double.Parse(bbox[0].Trim()));
                right = Helper.DegreeToRadian(double.Parse(bbox[1].Trim()));
                bottom = Helper.DegreeToRadian(double.Parse(bbox[2].Trim()));
                left = Helper.DegreeToRadian(double.Parse(bbox[3].Trim()));
            }
            catch (Exception)
            {
                throw new Exception("Param bbox has a wrong format. All parameters should be double type");
            }
        }
    }
}