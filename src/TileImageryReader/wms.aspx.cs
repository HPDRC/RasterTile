using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.IO;

namespace TileImageryReader
{
    public partial class wms : PageBase
    {
        const int tileSize = 256;

        protected void Page_Load(object sender, EventArgs e)
        {
            SafeRunPrintError(delegate
            {
                string service = "getcapabilities";
                if (Request["bbox"] != null)
                    service = "getmap";
                if (Request["request"] != null)
                    service = Request["request"].ToLower();
                if (service == "getcapabilities")
                {
                    OutputXml(capabilitiesString);
                }
                else if (service == "getmap")
                {
                    ParamWmsGetMap param = new ParamWmsGetMap(Request);
                    try
                    {
                        Reader reader = new Reader(param.debug);
                        reader.GetWmsMosaic(param);
                        if (!param.debug)
                            OutputImage(reader.resultAsBytes, tileSize);
                        else
                            OutputText(reader.logString);
                    }
                    catch (Exception ex)
                    {
                        if (!param.debug)
                            OutputBlackTile(tileSize);
                        else
                            OutputText(ex.Message);
                        Log("wms_get_map", ex);
                    }
                }
                else
                    throw new Exception("Parameter request is not recognized");
            });
        }

        private string capabilitiesString = @"<?xml version=""1.0"" encoding=""UTF-8""?>
<WMT_MS_Capabilities version=""1.3.0"">
    <Service>
        <Name>OGC:WMS</Name>
        <Title>TerraflyWMS</Title>
        <Abstract></Abstract>
        <KeywordList><Keyword>WMS</Keyword></KeywordList>
        <Insert_OnlineResource/>
        <ContactInformation>
            <ContactPersonPrimary>
                <ContactPerson></ContactPerson>
                <ContactOrganization></ContactOrganization>
            </ContactPersonPrimary>
            <ContactPosition/>
            <ContactAddress>
                <AddressType/>
                <Address/>
                <City/>
                <StateOrProvince/>
                <PostCode/>
                <Country/>
            </ContactAddress>
            <ContactVoiceTelephone/>
            <ContactFacsimileTelephone/>
            <ContactElectronicMailAddress/>
        </ContactInformation>
        <!-- Fees or access constraints imposed. -->
        <Fees>NONE</Fees>
        <AccessConstraints>NONE</AccessConstraints>
        <LayerLimit>1</LayerLimit>
        <MaxWidth>2048</MaxWidth>
        <MaxHeight>2048</MaxHeight>
    </Service>

    <Capability>
        <Request>
            <GetCapabilities>
                <Format>text/xml</Format>
                <DCPType>
                    <HTTP>
                        <Get><OnlineResource xmlns:xlink=""http://www.w3.org/1999/xlink"" xlink:type=""simple"" xlink:href=""http://mountain.cs.fiu.edu/RasterService/wms.aspx?""/></Get>
                    </HTTP>
                </DCPType>
            </GetCapabilities>

            <GetMap>
                <Format>image/jpeg</Format>
                <DCPType>
                    <HTTP>
                        <Get><OnlineResource xmlns:xlink=""http://www.w3.org/1999/xlink"" xlink:type=""simple"" xlink:href=""http://mountain.cs.fiu.edu/RasterService/wms.aspx?""/></Get>
                    </HTTP>
                </DCPType>
            </GetMap>
        </Request>

        <Layer>
            <Title>TerraflyWMS</Title>
            <Layer queryable=""false"">
                <Name>TerraflyWMS</Name>
                <Title>TerraflyWMS</Title>
                <CRS>EPSG:3857</CRS>
                <CRS>EPSG:4326</CRS>
                <EX_GeographicBoundingBox>
                    <westBoundLongitude>-180</westBoundLongitude>
                    <eastBoundLongitude>180</eastBoundLongitude>
                    <southBoundLatitude>-85.0511</southBoundLatitude>
                    <northBoundLatitude>85.0511</northBoundLatitude>
                </EX_GeographicBoundingBox>
                <BoundingBox CRS=""EPSG:3857"" minx=""-2.00375e+07"" miny=""-2.00375e+07"" maxx=""2.00375e+07"" maxy=""2.00375e+07""/>
                <BoundingBox CRS=""EPSG:4326"" minx=""-85.0511"" miny=""-180"" maxx=""85.0511"" maxy=""180""/>
            </Layer>
        </Layer>
    </Capability>
</WMT_MS_Capabilities>";
    }
}