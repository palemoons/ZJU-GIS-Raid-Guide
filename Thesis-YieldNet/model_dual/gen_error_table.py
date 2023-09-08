from keras.models import load_model
from keras import backend as K
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

# Build up test dataset
X1_test = []
X2_test = []
y_test_true =[]
error_map = []
for i, sample in enumerate(dataloader.test_sample_IDs):
    print(
        f"Building up the training dataset ({i+1}/{len(dataloader.test_sample_IDs)})... ",
        end="\r",
        flush=True,
    )
    X1, X2, y = dataloader.data_generation(sample["year"], sample["ct_adcode"])
    if y is None:
        continue
    else:
        X1_test.append(X1)
        X2_test.append(X2)
        y_test_true.append(y)
        error_map.append([sample["year"], sample["ct_adcode"], '', y])


# Load model
print("Predicting...")
model = load_model(os.path.join(config["model_folder"], "model_dual.h5"), custom_objects={"K": K})
X1_test = np.transpose(X1_test, (0, 2, 1, 3))
X1_test = np.reshape(X1_test, (X1_test.shape[0], X1_test.shape[1], -1))
X2_test = np.transpose(X2_test, (0, 2, 1, 3))
X2_test = np.reshape(X2_test, (X2_test.shape[0], X2_test.shape[1], -1))
y_test_pred = np.squeeze(model.predict({"input_1": X1_test, "input_2": X2_test}))


# create error map table.
for i, pred in enumerate(y_test_pred):
    error_map[i][2] = pred

error_map = np.array(error_map)
np.savetxt('error_table.txt', error_map, delimiter=',', fmt="%s", header= 'year,ct_adcode,prediction,truth', comments='')