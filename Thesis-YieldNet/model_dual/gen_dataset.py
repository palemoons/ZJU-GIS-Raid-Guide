import os
import numpy as np
from dataloader import DataLoader

"""
This script is used to generate the dataset for training and validation with a given config.
"""

#### USER INPUT ####
##############################################

config_folder = "./configs"
config_fname = "config_dual_model.json"

# Load config
print(f"Loading config from {config_fname}...")
config_fpath = os.path.join(config_folder, config_fname)
dataloader = DataLoader(config_fpath)
dataloader_ID_folder = dataloader.config["dataset_folder"]
print("\nDone.\n")

# Create output folder
if not os.path.exists(dataloader_ID_folder):
    os.makedirs(dataloader_ID_folder)

#### Build up numpy dataset
###############################################################################
X1_train = []
X2_train = []
y_train = []
valid_number = 0
for i, sample in enumerate(dataloader.train_sample_IDs):
    print(
        f"Building up the training dataset ({i+1}/{len(dataloader.train_sample_IDs)})... ",
        end="\r",
        flush=True,
    )
    X1, X2, y = dataloader.data_generation(sample["year"], sample["ct_adcode"])
    if y is None:
        continue
    else:
        X1_train.append(X1)
        X2_train.append(X2)
        y_train.append(y)
        valid_number += 1
X1_train = np.stack(X1_train, axis=0)
X2_train = np.stack(X2_train, axis=0)
print(f"\nValid samples: {valid_number}")
print(f"Saving X1_train with {X1_train.nbytes / 1e6:.4f}MB...")
np.save(os.path.join(dataloader_ID_folder, "X1_train.npy"), X1_train)
print(f"Saving X2_train with {X2_train.nbytes / 1e6:.4f}MB...")
np.save(os.path.join(dataloader_ID_folder, "X2_train.npy"), X2_train)
y_train = np.asarray(y_train)
print(f"Saving y_train with {y_train.nbytes / 1e6:.4f}MB...")
np.save(os.path.join(dataloader_ID_folder, "y_train.npy"), y_train)

X1_val = []
X2_val = []
y_val = []
valid_number = 0
for i, sample in enumerate(dataloader.val_sample_IDs):
    print(
        f"Building up the validation dataset ({i+1}/{len(dataloader.val_sample_IDs)})... ",
        end="\r",
        flush=True,
    )
    X1, X2, y = dataloader.data_generation(sample["year"], sample["ct_adcode"])
    if y is None:
        continue
    else:
        X1_val.append(X1)
        X2_val.append(X2)
        y_val.append(y)
        valid_number += 1
X1_val = np.stack(X1_val, axis=0)
X2_val = np.stack(X2_val, axis=0)
print(f"\nValid samples: {valid_number}")
print(f"Saving X1_val with {X1_val.nbytes / 1e6:.4f}MB...")
np.save(os.path.join(dataloader_ID_folder, "X1_val.npy"), X1_val)
print(f"Saving X2_val with {X2_val.nbytes / 1e6:.4f}MB...")
np.save(os.path.join(dataloader_ID_folder, "X2_val.npy"), X2_val)
y_val = np.asarray(y_val)
print(f"Saving y_val with {y_val.nbytes / 1e6:.4f}MB...")
np.save(os.path.join(dataloader_ID_folder, "y_val.npy"), y_val)

X1_test = []
X2_test = []
y_test = []
valid_number = 0
for i, sample in enumerate(dataloader.test_sample_IDs):
    print(
        f"Building up the test dataset ({i+1}/{len(dataloader.test_sample_IDs)})... ",
        end="\r",
        flush=True,
    )
    X1, X2, y = dataloader.data_generation(sample["year"], sample["ct_adcode"])
    if y is None:
        continue
    else:
        X1_test.append(X1)
        X2_test.append(X2)
        y_test.append(y)
        valid_number += 1
X1_test = np.stack(X1_test, axis=0)
X2_test = np.stack(X2_test, axis=0)
print(f"\nValid samples: {valid_number}")
print(f"Saving X1_test with {X1_test.nbytes / 1e6:.4f}MB...")
np.save(os.path.join(dataloader_ID_folder, "X1_test.npy"), X1_test)
print(f"Saving X2_test with {X2_test.nbytes / 1e6:.4f}MB...")
np.save(os.path.join(dataloader_ID_folder, "X2_test.npy"), X2_test)
y_test = np.asarray(y_test)
print(f"Saving y_test with {y_test.nbytes / 1e6:.4f}MB...")
np.save(os.path.join(dataloader_ID_folder, "y_test.npy"), y_test)

print("Done.")
