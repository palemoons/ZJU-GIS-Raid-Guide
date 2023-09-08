import ee
from geetools import *
import os

os.environ["HTTP_PROXY"] = "http://127.0.0.1:8889"
os.environ["HTTPS_PROXY"] = "http://127.0.0.1:8889"
# ee.Authenticate()
print("Start Initializing.")
ee.Initialize()
print("Finish Initializing.")

# external variables
year = [
    "2000",
    #     '2001',
    #     '2002',
    #     '2003',
    #     '2004',
    #     '2005',
    #     '2006',
    #     '2007',
    #     '2008',
    #     '2009',
    #     '2010',
    #     '2011',
    #     '2012',
]
start_date = "04-01"
end_date = "10-31"

northeast = ee.FeatureCollection("projects/ee-palemoons14/assets/northeast")
# MOD15A2H handle by year
for y in year:
    MOD15A2H = (
        ee.ImageCollection("MODIS/061/MOD15A2H")
        .filterDate(f"{y}-{start_date}", f"{y}-{end_date}")
        .filterBounds(northeast.geometry())
    )
    # handle by each image
    size = MOD15A2H.size().getInfo()
    toList = MOD15A2H.toList(size)
    for i in range(size):
        image = ee.Image(toList.get(i))
        date = image.get("system:index").getInfo()
        bands = ["Lai_500m", "FparLai_QC", "LaiStdDev_500m"]
        scale = 1000
        name_pattern = "MOD15A2H_{system_date}_{dt_adcode}"
        date_pattern = "yyyy_MM_dd"
        folder = f"MOD15A2H_{str(y)}"
        data_type = "double"
        max_pixels = 1e9
        print("Start task.")
        task = batch.Export.image.toDriveByFeature(
            image,
            collection=northeast,
            folder=f"{date}",
            namePattern=name_pattern,
            scale=scale,
            dataType=data_type,
            datePattern=date_pattern,
            maxPixels=max_pixels,
            crs="EPSG: 4326",
            verbose=True,
        )
print("Finish.")
