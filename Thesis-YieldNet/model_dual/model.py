from keras.layers import (
    Input,
    LSTM,
    Dense,
    concatenate,
    Activation,
    Multiply,
    Lambda,
    BatchNormalization,
    Dropout
)
from keras.models import Model
from keras.optimizers import Adam
from keras import backend as K


def DUAL_LSTM(
    input_shape1, input_shape2, activation="relu", loss="mse", learning_rate=1e-4
):
    """
    Build up the DUAL_LSTM model.

    Parameters:
    input_shape1 (tuple): shape of the X1 tensor.
    input_shape2 (tuple): shape of the X2 tensor.
    activation (str): activation function for the model.
    loss (str): loss function for the model.
    learning_rate (float): learning rate for the model.

    Returns:
    model (Model): the built up model.
    """

    input1 = Input(shape=input_shape1, name="input_1")
    lstm1 = LSTM(units=128, return_sequences=True, activation="relu", dropout=0.2)(input1)
    
    input2 = Input(shape=input_shape2, name="input_2")
    lstm2 = LSTM(units=192, return_sequences=True, activation="relu", dropout=0.2)(input2)

    merged = concatenate([lstm1, lstm2], axis=-1)

    # attention layer
    attention_dense = Dense(1, activation="tanh")(merged)
    attention_vector = Activation("softmax")(attention_dense)
    attention_output = Multiply()([merged, attention_vector])
    attention_output = Lambda(lambda x: K.sum(x, axis=1))(attention_output)

    # dense layers
    output = Dense(units=512, activation=activation)(attention_output)
    bn1 = BatchNormalization(axis=-1)(output)
    act1 = Activation(activation)(bn1)
    drop1 = Dropout(0.4)(act1)
    output = Dense(units=256, activation=activation)(drop1)
    bn2 = BatchNormalization(axis=-1)(output)
    act2 = Activation(activation)(bn2)
    drop2 = Dropout(0.2)(act2)
    output = Dense(units=128, activation=activation)(drop2)
    output = Dense(units=1)(output)

    model = Model(inputs=[input1, input2], outputs=output)
    model.compile(Adam(learning_rate=learning_rate), loss=loss)

    return model
