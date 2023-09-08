import os
import json
from osgeo import gdal

#### USER INPUT ####
##############################################

config_folder = "./configs"
config_fname = "config.json"

# Load config
config_fpath = os.path.join(config_folder, config_fname)
with open(config_fpath, "r") as f:
    config = json.load(f)
modis = os.path.join(config["raw_folder"], "modis")

# Get min and max
for m in os.listdir(modis):
    if not os.path.isdir(os.path.join(modis, m)):
        continue
    bands = next((x["bands"] for x in config["modis"] if x["name"] == m))
    min_value = [1e6 for _ in range(len(bands))]
    max_value = [-1e6 for _ in range(len(bands))]
    if not os.path.isdir(os.path.join(modis, m)):
        continue
    for d in os.listdir(os.path.join(modis, m)):
        if not os.path.isdir(os.path.join(modis, m, d)):
            continue
        for f in os.listdir(os.path.join(modis, m, d)):
            if not f.endswith(".tif") or f.startswith("."):
                continue
            raster_fpath = os.path.join(modis, m, d, f)
            ds = gdal.Open(raster_fpath, gdal.GA_ReadOnly)
            n_band = ds.RasterCount
            for i in range(n_band):
                band = ds.GetRasterBand(i + 1)
                if band.GetDescription() in bands:
                    stats = band.GetStatistics(True, True)
                    band_index = bands.index(band.GetDescription())
                    min_value[band_index] = min(min_value[band_index], stats[0])
                    max_value[band_index] = max(max_value[band_index], stats[1])
    for i in range(len(bands)):
        print(f"{m} {bands[i]}: min={min_value[i]}, max={max_value[i]}")
