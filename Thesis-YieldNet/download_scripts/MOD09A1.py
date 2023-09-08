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
    # "2001",
    # "2002",
    # "2003",
    # "2004",
    # "2005",
    "2006",
    # "2007",
    # "2008",
    # "2009",
    # "2010",
    # "2011",
    # "2012",
    # "2013",
    # "2014",
    # "2015",
    # "2016",
    # "2017",
    # "2018",
    # "2019",
    # "2020",
]
start_date = "06-09"
end_date = "06-11"

northeast = ee.FeatureCollection("projects/ee-palemoons14/assets/northeast_city")
# MOD09A1 handle by year
for y in year:
    MOD09A1 = (
        ee.ImageCollection("MODIS/061/MOD09A1")
        .filterDate(f"{y}-{start_date}", f"{y}-{end_date}")
        .filterBounds(northeast.geometry())
    )
    # handle by each image
    size = MOD09A1.size().getInfo()
    toList = MOD09A1.toList(size)
    for i in range(size):
        image = ee.Image(toList.get(i))
        date = image.get("system:index").getInfo()
        bands = [
            "sur_refl_b01",
            "sur_refl_b02",
            "sur_refl_b03",
            "sur_refl_b04",
            "sur_refl_b05",
            "sur_refl_b06",
            "sur_refl_b07",
            "QA",
        ]
        scale = 1000
        name_pattern = "MOD09A1_{system_date}_{ct_adcode}"
        date_pattern = "yyyy_MM_dd"
        folder = f"MOD09A1_{str(y)}"
        data_type = "double"
        max_pixels = 1e9
        print("Start task.")
        task = batch.Export.image.toDriveByFeature(
            image.select(bands),
            collection=northeast,
            folder=f"{date}_9",
            namePattern=name_pattern,
            scale=scale,
            dataType=data_type,
            datePattern=date_pattern,
            maxPixels=max_pixels,
            crs="EPSG: 4326",
            verbose=True,
        )
print("Finish.")
