import os
from osgeo import gdal
import matplotlib
import matplotlib.pyplot as plt

# count valid pixels
# reproj_path = "./raw_data/reproj"
reproj_path_county = './raw_data/reproj_county'

def count_valid_pixels(filepath):
  ds = gdal.Open(filepath, gdal.GA_ReadOnly)
  band = ds.GetRasterBand(1)
  data = band.ReadAsArray().flatten()
  no_data_value = band.GetNoDataValue()
  valid_data = [x for x in data if x != no_data_value]
  
  return len(valid_data)

# valid_pixels_list = []
# folder_list = os.listdir(reproj_path)
# for folder in folder_list:
#   file_list = os.listdir(os.path.join(reproj_path, folder))
#   for file in file_list:
#     if not file.endswith(".tif"):
#       continue
#     valid_pixels_list.append(count_valid_pixels(os.path.join(reproj_path, folder, file)))

valid_pixels_list_county = []
folder_list = os.listdir(reproj_path_county)
for folder in folder_list:
  file_list = os.listdir(os.path.join(reproj_path_county, folder))
  for file in file_list:
    if not file.endswith(".tif"):
      continue
    valid_pixels_list_county.append(count_valid_pixels(os.path.join(reproj_path_county, folder, file)))

# plt.violinplot(valid_pixels_list, showmedians=True)
plt.violinplot(valid_pixels_list_county, showmedians=True)
zh_font = matplotlib.font_manager.FontProperties(fname="STHeitiBold.ttf") 
plt.xlabel("频率", fontproperties=zh_font)
plt.ylabel("有效像素数", fontproperties=zh_font)
plt.title("有效像素数分布图", fontproperties=zh_font)
plt.savefig('violin_county.png')