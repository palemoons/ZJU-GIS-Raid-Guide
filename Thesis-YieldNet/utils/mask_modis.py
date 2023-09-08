import os
import subprocess
import shutil
import glob
from osgeo import gdal
import numpy as np
import multiprocessing
from multiprocessing import Pool
from datetime import datetime

"""
This script is used to mask MODIS image using ChinaCropPhen1km.
"""

#### USER INPUT ####
##############################################

# ChinaCropPhen1km path
reproj_path = "./raw_data/reproj"
# MODIS image parent path
modis_path = "./raw_data/modis"
output_path = "./raw_data/masked"
MOD09A1 = os.path.join(modis_path, "MOD09A1")
MOD15A2H = os.path.join(modis_path, "MOD15A2H")
MOD16A2 = os.path.join(modis_path, "MOD16A2")
MOD17A2H = os.path.join(modis_path, "MOD17A2H")
modis = [MOD09A1, MOD15A2H, MOD16A2, MOD17A2H]

##############################################

# Mask MODIS image
def mask():
    # Create output dir
    os.mkdir(output_path)
    # Create output dir
    for m in modis:
        m = os.path.basename(m)
        os.mkdir(os.path.join(output_path, m))
        for d in os.listdir(os.path.join(modis_path, m)):
            if not os.path.isdir(os.path.join(modis_path, m, d)):
                continue
            os.mkdir(os.path.join(output_path, m, d))

    for m in modis:
        # Create tmp dir
        os.mkdir(os.path.join(output_path, "tmp"))
        # MODIS Product
        m = os.path.basename(m)
        for d in os.listdir(os.path.join(modis_path, m)):
            process_arr = []
            # MODIS Date
            if not os.path.isdir(os.path.join(modis_path, m, d)):
                continue
            for f in os.listdir(os.path.join(modis_path, m, d)):
                # MODIS Image
                if f.startswith(".") or not f.endswith(".tif"):
                    continue
                dt_adcode = f[-10:-4]
                year = f[-21:-17]
                raster_fpath = os.path.join(modis_path, m, d, f)
                mask_fpath = os.path.join(
                    reproj_path, dt_adcode, f"CHN_Maize_HE_{year}_{dt_adcode}.tif"
                )
                tmp_folder = os.path.join(output_path, "tmp")
                out_fpath = os.path.join(output_path, m, d, f)
                resized_path = os.path.join(tmp_folder, f)
                resize(raster_fpath, mask_fpath, resized_path)
                process_arr.append((resized_path, mask_fpath, tmp_folder, out_fpath))
            print(
                f"🚀Start {m} {d} multiprocessing at {datetime.now()}",
                end="\r",
                flush=True,
            )
            with Pool(multiprocessing.cpu_count()) as p:
                p.map(mask_calc, process_arr)
        # Remove tmp dir
        shutil.rmtree(os.path.join(output_path, "tmp"))
        print("🚮Tmp dir removed.", end="\r", flush=True)


#### Functions ####
######################################


def mask_calc(args):
    """
    Param:
        raster_fpath: str, path of raster file
        mask_fpath: str, path of mask file
        tmp_path: str, parent path of tmp file
        out_fpath: str, output file path
    """
    raster_fpath, mask_fpath, tmp_path, out_fpath = args
    ras = gdal.Open(raster_fpath)
    bands_count = ras.RasterCount
    bands = []
    raster_fname = os.path.basename(raster_fpath)
    images = []
    for i in range(bands_count):
        band = ras.GetRasterBand(i + 1)
        bands.append(band.GetDescription())
        tmp_fpath = os.path.join(tmp_path, f"{raster_fname[:-4]}_{i+1}.tif")
        images.append(tmp_fpath)
        command = f'gdal_calc.py -A {raster_fpath} --A_band={i+1} -B {mask_fpath} --calc="A*(B!=255)" --outfile={tmp_fpath} --NoDataValue=-999 --quiet'
        os.system(command)
    ras = None
    raster_fname = os.path.basename(out_fpath)
    command = [
        "gdal_merge.py",
        "-o",
        out_fpath,
        "-of",
        "GTiff",
        "-a_nodata",
        "-999",
        "-ot",
        "Float32",
        "-separate",
    ]
    command.extend(images)
    subprocess.run(command, stdout=subprocess.DEVNULL)
    # Rename bands' name
    names = []
    for index, band in enumerate(bands):
        names.append((index + 1, band))
    set_band_descriptions(out_fpath, names)


def merge_bands(band_fpath, out_fpath, bands):
    """
    Param:
        band_fpath: str, path of input bands
        out_fpath: str, output path of merged bands
        bands: list, band names
    """
    # Merge bands
    raster_fname = os.path.basename(out_fpath)
    images = glob.glob(os.path.join(band_fpath, f"{raster_fname[:-4]}_*.tif"))
    subprocess.run(
        [
            "gdal_merge.py",
            "-o",
            out_fpath,
            "-of",
            "GTiff",
            "-a_nodata",
            "0.0",
            "-ot",
            "Float32",
            "-separate",
        ]
        + images,
        stdout=subprocess.DEVNULL,
    )
    # Rename bands' name
    names = []
    for index, band in enumerate(bands):
        names.append((index + 1, band))
    set_band_descriptions(out_fpath, names)


def set_band_descriptions(filepath, bands):
    """
    filepath: path/virtual path/uri to raster
    bands:    ((band, description), (band, description),...)
    """
    ds = gdal.Open(filepath, gdal.GA_Update)
    for band, desc in bands:
        rb = ds.GetRasterBand(band)
        rb.SetDescription(desc)
    del ds


def resize(raster_fpath, mask_fpath, output_fpath):
    """
    Resize the raster_file to mask_file's shape.

    Parameters:
    raster_fpath: input raster image.
    mask_fpath: input mask image.
    output_fpath: output raster image.
    """
    mask_file = gdal.Open(mask_fpath, gdal.GA_ReadOnly)
    raster_file = gdal.Open(raster_fpath, gdal.GA_ReadOnly)
    gt = mask_file.GetGeoTransform()
    minx = gt[0]
    maxy = gt[3]
    maxx = minx + gt[1] * mask_file.RasterXSize
    miny = maxy + gt[5] * mask_file.RasterYSize
    gdal.Translate(
        output_fpath, raster_file, format="GTiff", projWin=[minx, maxy, maxx, miny]
    )
    mask_file = None
    raster_file = None


def clip2minimum(raster_fpath, output_fpath):
    """
    Clip the raster_file using minimum bounding rectangle of the non-zero values. If file do not have valid values, clip to 8x8.
    """
    ds = gdal.Open(raster_fpath, gdal.GA_ReadOnly)
    band = ds.GetRasterBand(1)
    no_data = band.GetNoDataValue()
    gt = ds.GetGeoTransform()
    width = ds.RasterXSize
    height = ds.RasterYSize
    data = band.ReadAsArray(0, 0, width, height)
    rows = np.any(data != no_data, axis=1)
    cols = np.any(data != no_data, axis=0)
    if np.all(data == no_data):
        # clip to 8x8 in the center of image
        minx, miny, maxx, maxy = (
            gt[0] + gt[1] * (width / 2 - 4),
            gt[3] + gt[5] * (height / 2 + 4),
            gt[0] + gt[1] * (width / 2 + 4),
            gt[3] + gt[5] * (height / 2 - 4),
        )
        gdal.Translate(
            output_fpath,
            raster_fpath,
            format="GTiff",
            projWin=[minx, maxy, maxx, miny],
            xRes=gt[1],
            yRes=gt[5],
        )
    else:
        minx = gt[0] + np.where(cols)[0][0] * gt[1]
        maxx = gt[0] + (np.where(cols)[0][-1] + 1) * gt[1]
        miny = gt[3] + (np.where(rows)[0][-1] + 1) * gt[5]
        maxy = gt[3] + np.where(rows)[0][0] * gt[5]
        gdal.Translate(
            output_fpath,
            raster_fpath,
            format="GTiff",
            projWin=[minx, maxy, maxx, miny],
            xRes=gt[1],
            yRes=gt[5],
        )


######################################

if __name__ == "__main__":
    start = datetime.now()
    print(f"🕒Start Time: {start}")
    mask()
    end = datetime.now()
    print(f"\n🕒End Time: {end}")
    print(f"🕒Processing Time: {end - start}")
