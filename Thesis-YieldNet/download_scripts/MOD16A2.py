import ee
from geetools import *
from datetime import datetime

# ee.Authenticate()
print("Start Initializing.")
ee.Initialize()
print("Finish Initializing.")

# external variables
year = [
    # '2000',
    #     '2001',
    "2002",
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
# MOD16A2 handle by year
for y in year:
    MOD16A2 = (
        ee.ImageCollection("MODIS/NTSG/MOD16A2/105")
        .filterDate(f"{y}-{start_date}", f"{y}-{end_date}")
        .filterBounds(northeast.geometry())
    )
    # handle by each image
    size = MOD16A2.size().getInfo()
    toList = MOD16A2.toList(size)
    for i in range(size):
        image = ee.Image(toList.get(i))
        date = datetime.fromtimestamp(
            ee.Image(toList.get(i)).get("system:time_start").getInfo() / 1000
        ).strftime("%Y_%m_%d")
        bands = ["ET", "ET_QC"]
        scale = 1000
        name_pattern = "MOD16A2_{system_date}_{dt_adcode}"
        date_pattern = "yyyy_MM_dd"
        folder = f"MOD16A2_{str(y)}"
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
