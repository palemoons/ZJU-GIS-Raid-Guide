import pandas as pd
import geopandas as gpd
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import matplotlib
from matplotlib_scalebar.scalebar import ScaleBar
from shapely.geometry.point import Point
from matplotlib.colors import Normalize

def draw_error_map(year):
    cities = gpd.read_file('./raw_data/northeast_city', layer='northeast_city')
    cities["ct_adcode"] = cities["ct_adcode"].astype(int)
    models=['dual', 'dual_no_att', 'hist', 'hist_no_att', 'cnn', 'rf']
    tables = []
    for model in models:
        table = pd.read_csv(f'./model_{model}/error_table.txt')
        table["delta"] = table["prediction"] - table["truth"]
        et = cities.merge(table[table["year"] == year], on='ct_adcode')[["geometry", "delta"]]
        tables.append(et)

    fig, axs = plt.subplots(2, 3, sharey=True, sharex=True, figsize=(10, 8), dpi=600)
    vmax_abs = max(abs(min([table["delta"].min() for table in tables])), abs(max([table["delta"].max() for table in tables])))
    norm = Normalize(vmin=-vmax_abs,vmax=vmax_abs)
    mappable = cm.ScalarMappable(norm=norm, cmap='coolwarm')

    for index, table in enumerate(tables):
        table.plot(column='delta',
          ax = axs[index//3, index%3],
          cmap='coolwarm',
          norm = norm,
          linewidth=0.1,
          edgecolor="k",
          missing_kwds={
            "color": "white",
            "linewidth": 0.1,
            "edgecolor": "k",
          })
        axs[index//3, index%3].set_title(['DUAL_ATT', 'DUAL', 'LSTM_ATT', 'LSTM', 'CNN', 'RF'][index])
        points = gpd.GeoSeries(
            [Point(120, 40), Point(121, 40)], crs=4326
        )  # Geographic WGS 84 - degrees
        points = points.to_crs(32619)  # Projected WGS 84 - meters
        distance_meters = points[0].distance(points[1])
        axs[index//3, index%3].add_artist(ScaleBar(dx=distance_meters, location="lower left"))

        x, y, arrow_length = 0.9, 0.95, 0.1
        axs[index//3, index%3].annotate('N', xy=(x, y), xytext=(x, y-arrow_length),
                    arrowprops=dict(facecolor='black', width=1, headwidth=6),
                    ha='center', va='center', fontsize=12,
                    xycoords=axs[index//3, index%3].transAxes)
    
    cb_ax = fig.add_axes([0.1, 0.06, 0.8, 0.02])
    zh_font = matplotlib.font_manager.FontProperties(fname="STHeitiBold.ttf") 
    fig.colorbar(mappable, cax=cb_ax, orientation='horizontal', shrink=0.6)
    fig.suptitle("预测误差（万吨）", y=0.035, fontsize=14, fontproperties=zh_font)
    fig.savefig(f'error_{year}.png')

draw_error_map(2017)
draw_error_map(2018)
draw_error_map(2019)