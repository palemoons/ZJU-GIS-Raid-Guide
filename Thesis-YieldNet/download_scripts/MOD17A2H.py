import ee
from geetools import *

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
# MOD17A2H handle by year
for y in year:
    MOD17A2H = (
        ee.ImageCollection("MODIS/006/MOD17A2H")
        .filterDate(f"{y}-{start_date}", f"{y}-{end_date}")
        .filterBounds(northeast.geometry())
    )
    # handle by each image
    size = MOD17A2H.size().getInfo()
    toList = MOD17A2H.toList(size)
    for i in range(size):
        image = ee.Image(toList.get(i))
        date = image.get("system:index").getInfo()
        scale = 1000
        name_pattern = "MOD17A2H_{system_date}_{dt_adcode}"
        date_pattern = "yyyy_MM_dd"
        folder = f"MOD17A2H_{str(y)}"
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
