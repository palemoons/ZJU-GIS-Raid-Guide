from keras.models import load_model
from keras import backend as K
from sklearn.metrics import r2_score, mean_squared_error
from scipy.stats import gaussian_kde
import matplotlib.pyplot as plt
import os
import json
import numpy as np
from dataloader import DataLoader

# Load config
config_folder = "./configs"
config_fname = "config_dual_model.json"
config_fpath = os.path.join(config_folder, config_fname)
with open(config_fpath, "r") as f:
    config = json.load(f)
dataloader = DataLoader(config_fpath)

# Load test dataset
X1_test = np.load(os.path.join(config["dataset_folder"], "X1_test.npy"))
X1_test = np.transpose(X1_test, (0, 2, 1, 3))
X1_test = np.reshape(X1_test, (X1_test.shape[0], X1_test.shape[1], -1))
X2_test = np.load(os.path.join(config["dataset_folder"], "X2_test.npy"))
X2_test = np.transpose(X2_test, (0, 2, 1, 3))
X2_test = np.reshape(X2_test, (X2_test.shape[0], X2_test.shape[1], -1))
y_train = np.load(os.path.join(config["dataset_folder"], "y_train.npy"))
y_test_true = np.load(os.path.join(config["dataset_folder"], "y_test.npy"))

# Load model
print("Predicting...")
model = load_model(os.path.join(config["model_folder"], "model_dual.h5"), custom_objects={"K": K})
y_test_pred = np.squeeze(model.predict({"input_1": X1_test, "input_2": X2_test}))

# Calculate R2 score
print("\nCalculating...")
r2 = r2_score(y_test_true, y_test_pred)
print("R2 score: ", r2)

# Calculate RMSE
rmse = np.sqrt(mean_squared_error(y_test_true, y_test_pred))
print("RMSE: ", rmse)

# Calculate pearsonr
pearsonr = np.corrcoef(y_test_true, y_test_pred)[0, 1]
print("Pearson's R: ", pearsonr)

# Draw scatter plot
plt.figure(figsize=(6, 5), dpi=600)
xy = np.vstack([y_test_true, y_test_pred])
density = gaussian_kde(xy)(xy)
idx = density.argsort()
y_test_true, y_test_pred, density = y_test_true[idx], y_test_pred[idx], density[idx]
plt.scatter(y_test_pred, y_test_true, s=5, c=density, cmap='coolwarm')
plt.colorbar()
plt.plot([0, 1000], [0, 1000], color="black")

# Regression Line
coef = np.polyfit(y_test_pred, y_test_true, 1)
poly1d_fn = np.poly1d(coef)
plt.plot([min(y_test_pred), max(y_test_pred)], [min(poly1d_fn(y_test_pred)), max(poly1d_fn(y_test_pred))], linestyle='--', color='black')
plt.rc('font',family='monospace', size = 10)
plt.text(600, 150, f'$R^2$={r2:.4f}')
plt.text(600, 100, f'RMSE={rmse:.4f}')
plt.text(600, 50, f'Pearson\'s R={pearsonr:.4f}' )
# add title and legend
plt.xlabel('DUAL_LSTM_ATT', fontweight='bold', size=15)

# show plot
plt.show()
plt.savefig('dual_test.png')

np.savetxt('dual.txt', [y_test_pred, y_test_true], fmt='%.4f', delimiter=',')