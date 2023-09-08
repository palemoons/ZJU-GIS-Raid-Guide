import os
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.cm as cm
from matplotlib_scalebar.scalebar import ScaleBar
from shapely.geometry.point import Point
import geopandas as gpd
import pandas as pd

csv_folder = "./raw_data"

county_yield = pd.read_csv(os.path.join(csv_folder, f"city_yield.csv"))
county_yield = county_yield.groupby("ct_adcode").mean()
# draw map
counties = gpd.read_file(
    "./raw_data/northeast_city/northeast_city.shp", layer="northeast_city"
)
counties["ct_adcode"] = counties["ct_adcode"].astype(int)
counties = counties.merge(county_yield, on="ct_adcode", how="left")

yield_max = counties["yield"].max()
fig, ax = plt.subplots(1, 1, figsize=(5, 5), dpi=600)
zh_font = matplotlib.font_manager.FontProperties(fname="STHeitiBold.ttf")
counties.plot(
    column="yield",
    ax=ax,
    cmap="OrRd",
    edgecolor="k",
    linewidth=0.1,
    missing_kwds={
        "color": "lightgrey",
        "edgecolor": "k",
    },
)
mappable = cm.ScalarMappable(cmap="OrRd", norm=plt.Normalize(vmin=0, vmax=yield_max))
cb_ax = fig.add_axes([0.82, 0.11, 0.03, 0.77])
cb_ax.set_title(
    "总产量（万吨）",
    fontproperties=matplotlib.font_manager.FontProperties(
        fname="STHeitiBold.ttf", size=8
    ),
    pad=2,
)
fig.colorbar(mappable, cax=cb_ax, orientation="vertical")
ax.set_ylabel("经度", labelpad=2, fontproperties=zh_font)
ax.set_xlabel("纬度", labelpad=2, fontproperties=zh_font)
ax.set_title(
    "地级市尺度玉米总产量",
    fontproperties=matplotlib.font_manager.FontProperties(
        fname="STHeitiBold.ttf", size=12
    ),
    pad=2,
)

points = gpd.GeoSeries(
    [Point(120, 40), Point(121, 40)], crs=4326
)  # Geographic WGS 84 - degrees
points = points.to_crs(32619)  # Projected WGS 84 - meters
distance_meters = points[0].distance(points[1])
ax.add_artist(ScaleBar(dx=distance_meters, location="lower left"))

x, y, arrow_length = 0.9, 0.95, 0.1
ax.annotate('N', xy=(x, y), xytext=(x, y-arrow_length),
            arrowprops=dict(facecolor='black', width=1, headwidth=6),
            ha='center', va='center', fontsize=12,
            xycoords=ax.transAxes)
fig.savefig("yield_city.png")
