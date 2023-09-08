import os
from osgeo import ogr
import numpy as np

"""
A script converting yield table data to npy files.
"""

#### CONFIGURATION ####
##############################################

raw_path = "./raw_data/county_yield"
npy_path = "./raw_data"

##############################################

#### Handle county_yield ####
##############################################

# Read yield table
driver = ogr.GetDriverByName("ESRI Shapefile")
arr = []
for year in range(2000, 2013):
    ds = driver.Open(
        os.path.join(raw_path, f"county_yield_{year}", f"county_yield_{year}.shp")
    )
    for f in ds.GetLayer():
        yield_value = f.GetField("maize_yiel")
        if yield_value is None:
            yield_value = 0
        arr.append(
            {
                "year": year,
                "dt_adcode": f.GetField("dt_adcode"),
                "yield": yield_value / float(1000),
            }
        )
# Save to npy file
np.save(os.path.join(npy_path, "county_yield.npy"), arr)

#### Handle
