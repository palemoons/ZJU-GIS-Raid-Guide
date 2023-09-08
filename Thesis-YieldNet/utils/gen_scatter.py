import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.colors import Normalize
import matplotlib.cm as cm
from scipy.stats import gaussian_kde

models=['dual', 'dual_no_att', 'hist', 'hist_no_att', 'cnn', 'rf']
fig, axes = plt.subplots(2, 3, sharey=True, sharex=True, figsize=(10, 8), dpi=100)
_min=1
_max=0
for index, model in enumerate(models):
    y_test_pred, y_test_true = np.loadtxt(f'{model}.txt', delimiter=',')
    xy = np.vstack([y_test_true, y_test_pred])
    density = gaussian_kde(xy)(xy)
    _min = min(_min, density.min())
    _max = max(_max, density.max())
    idx = density.argsort()
    y_test_true, y_test_pred, density = y_test_true[idx], y_test_pred[idx], density[idx]
    coef = np.polyfit(y_test_pred, y_test_true, 1)
    poly1d_fn = np.poly1d(coef)

    axes[index//3, index%3].plot([min(y_test_pred), max(y_test_pred)], [min(poly1d_fn(y_test_pred)), max(poly1d_fn(y_test_pred))], linestyle='--', color='black')
    axes[index//3, index%3].set_title(['DUAL_ATT', 'DUAL', 'LSTM_ATT', 'LSTM', 'CNN', 'RF'][index])
    axes[index//3, index%3].plot([0, 1000], [0, 1000], color="black")

norm = Normalize(vmin=_min,vmax=_max)

for index, model in enumerate(models):
    y_test_pred, y_test_true = np.loadtxt(f'{model}.txt', delimiter=',')
    xy = np.vstack([y_test_true, y_test_pred])
    density = gaussian_kde(xy)(xy)
    idx = density.argsort()
    y_test_true, y_test_pred, density = y_test_true[idx], y_test_pred[idx], density[idx]
    axes[index//3, index%3].scatter(y_test_pred, y_test_true, s=5, c=density, norm=norm, cmap='coolwarm')

zh_font = matplotlib.font_manager.FontProperties(fname="STHeitiBold.ttf")
fig.supxlabel('预测产量', y=0.06, fontproperties=zh_font)
fig.supylabel('真实产量', x=0.06,fontproperties=zh_font) 
mappable = cm.ScalarMappable(norm=norm, cmap='coolwarm')
cax = plt.axes([0.92, 0.1, 0.03, 0.8])
plt.colorbar(mappable, cax=cax)
fig.savefig(f'scatter_city.png')