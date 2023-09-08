import os
import numpy as np
import tensorflow as tf
from dataloader import DataLoader
from model import DUAL_LSTM
import matplotlib.pyplot as plt

# Create Dataloader
config_folder = "./configs"
config_fname = "config_dual_model.json"
config_fpath = os.path.join(config_folder, config_fname)
dataloader = DataLoader(config_fpath)
dataloader_ID_folder = dataloader.config["dataset_folder"]
model_folder = dataloader.config["model_folder"]

tf.keras.utils.set_random_seed(dataloader.config["seed"])
tf.config.experimental.enable_op_determinism()

# Load dataset
X1_train = np.load(os.path.join(dataloader_ID_folder, "X1_train.npy"))
X2_train = np.load(os.path.join(dataloader_ID_folder, "X2_train.npy"))
y_train = np.load(os.path.join(dataloader_ID_folder, "y_train.npy"))
X1_val = np.load(os.path.join(dataloader_ID_folder, "X1_val.npy"))
X2_val = np.load(os.path.join(dataloader_ID_folder, "X2_val.npy"))
y_val = np.load(os.path.join(dataloader_ID_folder, "y_val.npy"))

# Reshape X_train and X_val from (numbers, bins, times, bands) to (numbers, times, features)
X1_train = np.transpose(X1_train, (0, 2, 1, 3))
X1_train = np.reshape(X1_train, (X1_train.shape[0], X1_train.shape[1], -1))
X2_train = np.transpose(X2_train, (0, 2, 1, 3))
X2_train = np.reshape(X2_train, (X2_train.shape[0], X2_train.shape[1], -1))
X1_val = np.transpose(X1_val, (0, 2, 1, 3))
X1_val = np.reshape(X1_val, (X1_val.shape[0], X1_val.shape[1], -1))
X2_val = np.transpose(X2_val, (0, 2, 1, 3))
X2_val = np.reshape(X2_val, (X2_val.shape[0], X2_val.shape[1], -1))
X1_shape = X1_train.shape[1:]
X2_shape = X2_train.shape[1:]


# Create new model
model = DUAL_LSTM(
    input_shape1=X1_shape,
    input_shape2=X2_shape,
    activation="relu",
    loss="mse",
    learning_rate=1e-4,
)
history = model.fit(
    x={"input_1": X1_train, "input_2": X2_train},
    y=y_train,
    epochs=200,
    validation_data=({"input_1": X1_val, "input_2": X2_val}, y_val),
    shuffle=dataloader.config["shuffle"],
)
loss = history.history["loss"]
val_loss = history.history["val_loss"]

# Generate the loss plot
epochs = range(1, len(loss) + 1)
plt.plot(epochs, loss, "b", label="Training loss")
plt.plot(epochs, val_loss, "r", label="Validation loss")
plt.title("Training and validation loss")
plt.xlabel("Epochs")
plt.ylabel("Loss")
plt.legend()
plt.show()
plt.savefig("dual_loss.png")

# Save model
model.save(os.path.join(model_folder, "model_dual.h5"))
