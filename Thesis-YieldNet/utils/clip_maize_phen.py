from osgeo import gdal, ogr
import os

"""
This script is used to clip ChinaCropPhen1km dataset with northeast shp.
"""

#### USER INPUT ####
##############################################

input_shape = "./northeast_city/northeast_city.shp"
raster_dir = "./全国玉米物候分布图2000-2019"
output_dir = "./clip"

##############################################

# create output directory if not exists
if not os.path.exists(output_dir):
    os.makedirs(output_dir)
    os.makedirs(os.path.join(output_dir, "HE"))
    os.makedirs(os.path.join(output_dir, "MA"))
    os.makedirs(os.path.join(output_dir, "V3"))
else:
    raise Exception("Output directory already exists.")


def clip_raster(raster_fname, vector_fname, code, out_path):
    # New filename.
    fname_out = out_path.replace(".tif", f"_{int(code):05}.tif")
    # Do the actual clipping
    gdal.Warp(
        fname_out,
        raster_fname,
        format="GTiff",
        cutlineDSName=vector_fname,
        cutlineWhere=f"ct_adcode='{code}'",
        cropToCutline=True,
    )
    # Return the fname
    return fname_out


# Open raster file, select first and unique layer
gg = ogr.Open(input_shape)
layer = gg.GetLayerByIndex(0)

# Loop over all tiff file in dir
for r in os.listdir(raster_dir):
    if r.endswith(".tif") and not r.startswith("."):
        raster_fname = os.path.join(raster_dir, r)
        # classify by 'HE', 'MA', 'V3'
        if "HE" in r:
            out_path = os.path.join(output_dir, "HE")
        elif "MA" in r:
            out_path = os.path.join(output_dir, "MA")
        elif "V3" in r:
            out_path = os.path.join(output_dir, "V3")
        # Loop over all features
        for ifeat in layer:
            code = ifeat.GetFieldAsString("ct_adcode")
            if not os.path.exists(os.path.join(out_path, code)):
                os.makedirs(os.path.join(out_path, code))
            fname_out = clip_raster(
                raster_fname, input_shape, code, os.path.join(out_path, code, r)
            )
        print(f"📃 Finished {r}.", flush=True)
print("🎉 Finished all.")
