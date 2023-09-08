import os
from osgeo import gdal

"""
This script is used to reproject ChinaCropPhen1km to WGS84 with the same extents as MODIS image.
"""

# ChinaCropPhen1km path
mask_path = "./raw_data/distribution"
# Reprojected ChinaCropPhen1km path
reproj_path = "./raw_data/reproj"
# MODIS image parent path
modis_path = "./raw_data/modis"
MOD15A2H = os.path.join(modis_path, "MOD15A2H")

##############################################


# Reproject ChinaCropPhen1km to WGS84 with the same extents as MODIS image
if __name__ == "__main__":
    for d in os.listdir(mask_path):
        if not os.path.isdir(os.path.join(mask_path, d)):
            continue
        os.mkdir(os.path.join(reproj_path, d))
        county = gdal.Open(
            os.path.join(MOD15A2H, "2001_04_07", f"MOD15A2H_2001_04_07_{d}.tif"),
            gdal.GA_ReadOnly,
        )
        gt = county.GetGeoTransform()
        width = county.RasterXSize
        height = county.RasterYSize
        for f in os.listdir(os.path.join(mask_path, d)):
            if f.startswith(".") or not f.endswith(".tif"):
                continue
            extent = [
                gt[0],
                gt[3] + gt[5] * county.RasterYSize,
                gt[0] + gt[1] * county.RasterXSize,
                gt[3],
            ]
            gdal.Warp(
                destNameOrDestDS=os.path.join(reproj_path, d, f),
                srcDSOrSrcDSTab=os.path.join(mask_path, d, f),
                dstSRS="EPSG:4326",
                resampleAlg="near",
                width=width,
                height=height,
                outputBounds=extent,
            )
    print("🗺️Reproject done.")
