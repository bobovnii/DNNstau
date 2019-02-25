"""
"""
from keras.models import Sequential
from keras.models import model_from_json
from keras.optimizers import Adam
import matplotlib.pyplot as plt
from keras.callbacks import Callback
from logger import *
import numpy as np
import pandas as pd
np.random.seed(7)
from functools import partial
from sklearn.model_selection import StratifiedKFold
from keras.layers import Input, Dense, LSTM, GRU, Embedding, Lambda
from keras.models import Model, load_model
import keras.backend as K
from keras.layers.core import Dropout, Activation
from hyperopt import Trials, STATUS_OK, tpe
from hyperas import optim
from hyperas.distributions import choice, uniform



class Training():
    """
    Training class implementation
    """
    def __init__(self, config):
        """

        :return:
        """
        self.config = config
        self.dir  = config.get("model", "dir")
        self.model_name = config.get("model", "model_name")


        ##Model Properties
        try:
            self.batch_size = int(config.get("model", "batch_size"))
        except Exception:
            self.batch_size = 4000

        try:
            self.epochs = int(config.get("model", "epochs"))
        except Exception:
            self.epochs = 2


        ##Training Monitoring:
        try:
            self._plot_history = int(config.get("model", "plot_history"))
        except Exception:
            self._plot_history = False

        try:
            self._store_history = int(config.get("model", "store_history"))
        except Exception:
            self._store_history = False

        #Training store
        try:
            self.dir = config.get("model", "dir")
            print("DIR", self.dir)
        except Exception:
            if self._store_history==False and self._plot_history==False:
                raise Exception

        #Training Name
        #try:
        self.model_name = config.get("model", "model_name")

        #if not os.path.exists(self.dir):
        if os.path.exists(self.dir +self.model_name):
            pass
        else:
            os.makedirs(self.dir +self.model_name)


        return

    def get_deepset_model(self, max_length):

        input_txt = Input(shape=(max_length,))
        x = Embedding(2, 100, mask_zero=True)(input_txt)
        x = Dense(30, activation='tanh')(x)
        Adder = Lambda(lambda x: K.sum(x, axis=1), output_shape=(lambda shape: (shape[0], shape[2])))
        x = Adder(x)
        encoded = Dense(1)(x)
        summer = Model(input_txt, encoded)
        adam = Adam(lr=1e-4, epsilon=1e-3)
        summer.compile(optimizer=adam, loss='mae')
        self.__model = summer
        return summer

    def get_tune_model(self, loss="binary_crossentropy", input_dim = 13, config=None):
        """

        :param loss:
        :param input_dim:
        :param config:
        :return:
        """
        model = Sequential()
        model.add(Dense(256, input_shape=(13,)))
        model.add(Activation({{choice(['relu', 'sigmoid'])}}))
        model.add(Dropout({{uniform(0, 1)}}))
        model.add(Dense({{choice([256, 512, 1024])}}))
        model.add(Activation({{choice(['relu', 'sigmoid'])}}))
        model.add(Dropout({{uniform(0, 1)}}))

        # If we choose 'four', add an additional fourth layer
        if {{choice(['three', 'four'])}} == 'four':
            model.add(Dense(100))

            # We can also choose between complete sets of layers

            model.add({{choice([Dropout(0.5), Activation('linear')])}})
            model.add(Activation('relu'))

        model.add(Dense(1))
        model.add(Activation('sigmoid'))

        model.compile(loss='categorical_crossentropy', metrics=['accuracy'],
                      optimizer={{choice(['rmsprop', 'adam', 'sgd'])}})

        self.__model = model
        return model

    def _model(self, loss="binary_crossentropy", input_dim = 13, config=None):
        """
           :return:
        """

        #cl4 = partial(custom_loss_4, weights=weights_tensor)

        model = Sequential()
        model.add(Dense(256, input_dim=input_dim, kernel_initializer='uniform', activation='sigmoid'))
        model.add(Dropout(0.4))
        model.add(Dense(256, kernel_initializer='uniform', activation='sigmoid'))
        model.add(Dropout(0.4))
        model.add(Dense(1, kernel_initializer='uniform', activation='sigmoid'))
        # Compile model
        model.compile(loss=loss, metrics=['accuracy'], optimizer=Adam(lr=0.01))

        print(model.summary())
        self.__model = model
        return model


    def get_model(self):


        return self.__model


    def tune_model(self, X, Y, X_validation=None, Y_validation=None,  callback=None, loss='binary_crossentropy'):
        """

        :return:
        """


        if X_validation is None or Y_validation is None:

            result = self.__model.fit(X, Y,
                                      batch_size={{choice([64, 128, 300, 500, 1000, 2000, 4000])}},
                                      epochs=2,
                                      verbose=2,
                                      validation_data=(X_validation, Y_validation),
                                      callbacks=callback)
        else:
            result = self.__model.fit(X, Y,
                                      batch_size={{choice([64, 128, 300, 500, 1000, 2000, 4000])}},
                                      epochs=2,
                                      verbose=2,
                                      validation_data=(X_validation, Y_validation),
                                      callbacks=callback)

        validation_acc = np.amax(result.history['val_acc'])
        print('Best validation acc of epoch:', validation_acc)
        return {'loss': -validation_acc, 'status': STATUS_OK, 'model': self.__model}


    def pre_train(self, X, Y, X_validation=None, Y_validation=None,  callback=None, loss='binary_crossentropy'):
        """

        # Set learning rate:
        # Set Number of epochs
        :return:
        """


        try:
            lr = self.config.get("pretrain", 'lr')
        except Exception:

            lr = self.config.get("train", 'lr')

        try:
            epochs = self.config.get("pretrain", 'epochs')
        except Exception:
            epochs = self.config.get("train", 'epochs')


        try:
            batch_size = self.config.get("pretrain", 'batch_size')
        except Exception:
            batch_size = self.config.get("train", 'batch_size')

        #Recompile model with new settings
        self.__model.compile(loss=loss,
                             metrics=['accuracy'],
                             optimizer=Adam(lr=float(lr)))
        #Start Training:


        if X_validation is None or Y_validation is None:

            self.__model.fit(X, Y, epochs=int(epochs), batch_size=int(batch_size), callbacks=callback)
        else:
            self.__model.fit(X, Y, epochs=int(epochs), batch_size=int(batch_size),
                                       validation_data=(X_validation, Y_validation, ), callbacks=callback)  # , verbose=0)


        return


        def train(self, X, Y, X_validation=None, Y_validation=None, callback=None,  loss='binary_crossentropy'):
            """

            #Set Numb
            :return:
            """
            try:
                lr = self.config.get("train", 'lr')
            except Exception:

                lr = self.config.get("train", 'lr')

            try:
                epochs = self.config.get("train", 'epochs')
            except Exception:
                epochs = self.config.get("train", 'epochs')


            try:
                batch_size = self.config.get("train", 'batch_size')
            except Exception:
                batch_size = self.config.get("train", 'batch_size')


            #Recompile model with new settings
            self.__model.compile(loss=loss,
                                 metrics=['accuracy'],
                                 optimizer=Adam(lr=float(lr)))


            if X_validation is None or Y_validation is None:

                self.__model.fit(X, Y, epochs=int(epochs), batch_size=int(batch_size), callbacks=callback)
            else:
                self.__model.fit(X, Y, epochs=int(epochs), batch_size=int(batch_size),
                                           validation_data=(X_validation, Y_validation, ), callbacks=callback)  # , verbose=0)



            return


    def train_k_folds(self, X, Y, k_folds = 5, n_class=2):
        """


        :return:
        """
        #Kfold

        folds = StratifiedKFold(n_splits=3, shuffle=True, random_state=1)
        oof_preds = np.zeros((len(X), 1))
        clfs = []
        index = 0
        for fold_, (trn_, val_) in enumerate(folds.split(Y, Y)):
            index+=0
            _x_train, _y_train = X.ix[trn_], Y.ix[trn_]
            x_valid, y_valid = X.ix[val_], Y.ix[val_]
            model = self._model()
            history = model.fit(_x_train, _y_train, validation_data=[x_valid, y_valid], epochs=self.epochs, batch_size=self.batch_size,
                                shuffle=True)  # , verbose=0)
            self.plot_history(history, title=str(index))
            #plot_loss_acc(history, title="DNN_{0}".format(index))
            oof_preds[val_, :] = model.predict_proba(x_valid, batch_size=4000)
            clfs.append(model)
        self.clfs = clfs
        return clfs, oof_preds

        return


    def pop_layers(self):
        """

        :param model:
        :return:
        """
        #TODO implement stacked model
        self.__model.pop()
        self.__model.pop()
        self.__model.pop()
        for index in range(len(self.__model.layers)):
            if index <=4:
                self.__model.layers[index].trainable = False

        self.__model.add(Dense(256, kernel_initializer='uniform', activation='relu'))
        self.__model.add(Dropout(0.4))
        self.__model.add(Dense(1, kernel_initializer='uniform', activation='sigmoid'))
        self.__model.compile(loss='binary_crossentropy',
                             metrics=['accuracy'], optimizer=Adam(lr=0.001))

        return

    def store_model(self, model=None, model_name=None):
        """
        Store model weights to disk
        :return:
        """
        if model is None:
            model = self.get_model()
        if model_name is None:
            model_name = self.model_name

        model_json = model.to_json()
        with open("{0}/Model.json".format(self.dir +  model_name), "w") as json_file:
            json_file.write(model_json)
        # serialize weights to HDF5
        model.save_weights(self.dir +  model_name+"/Model_Weight.h5")
        print("Model is saved on disk")
        return


    def store_models(self, models=None, model_name=None):
        """

                :return:
                """
        if models is None:
            models = self.clfs
        if model_name is None:
            model_name = self.model_name
        index = 0
        for i in range(len(models)):
            index+=1
            model_json = models[i].to_json()
            with open("{0}/Model{0}.json".format(index).format(self.dir + self.model_name), "w") as json_file:
                json_file.write(model_json)
            # serialize weights to HDF5
            models[i].save_weights(self.dir + self.model_name + "/Model_weight_{0}".format(index))
            print("Model is saved on disk")
        return


    def load_model(self, MODEL_NAME):
        """

        :return:
        """
        #TODO implement this method
        # load json and create model
        json_file = open('GEN_MET_model.json', 'r')
        loaded_model_json = json_file.read()
        json_file.close()
        loaded_model = model_from_json(loaded_model_json)
        # load weights into new model
        loaded_model.load_weights(MODEL_NAME)
        print("Loaded model from disk")

        return


    def extract_weights(self, model):
        """

        :param model:
        :return:
        """
        W = [layer.get_weights()[0] for layer in model.layers]
        B = [layer.get_weights()[1] for layer in model.layers]

        return {"W": W, "B": B}


    def model_diagnostic(self):
        """

        :return:
        """
        #TODO provide simple way for model diagnostics
        return


    def evaluate_model(self, X, Y, model=None):
        """

        :param X:
        :param Y:
        :param model:
        :return:
        """
        #if model is None:
        #    model = self.get_model()

        # evaluate the model
        scores = self.__model.evaluate(X, Y, verbose=0)
        print("Accuraccy %s: %.2f%%" % (self.__model.metrics_names[1], scores[1] * 100))



    def get_results(self, _x_train, _y_train, x_test, y_test, _w_train, w_test):

        _df_train = pd.DataFrame()
        _df_test = pd.DataFrame()

        _df_train["train_labels"] = [i[0] for i in _y_train.as_matrix()]
        _df_test["test_labels"] = [i[0] for i in y_test.as_matrix()]
        _df_train["index"] = _x_train.index

        _df_train["train_output"] = self.__model.predict(_x_train, verbose=0)
        _df_test["test_output"] = self.__model.predict(x_test, verbose=0)

        _df_train["train_weights"] = _w_train.as_matrix()
        _df_test["test_weights"] = w_test.as_matrix()

    
        _df_train["train_pred"] = _df_train["train_output"].apply(lambda x: 1 if x > 0.5 else 0)
        _df_test["test_pred"] = _df_test["test_output"].apply(lambda x: 1 if x > 0.5 else 0)
        _df_test["index"] = x_test.index
        return _df_train, _df_test


    def lr_schedule(self):
        """

        :return:
        """
        #self.loss_history = LossHistory()
        #lrate = LearningRateScheduler(step_decay)

        #callbacks_list = [loss_history, lrate]
        #history = model.fit(X_train, y_train,
        #                validation_data=(X_test, y_test),
        #                epochs=epochs,
        #                batch_size=batch_size,
        #                callbacks=callbacks_list,
        #                verbose=2)
        return

    
