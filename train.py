
from keras.models import Sequential
from keras.models import model_from_json
from keras.optimizers import Adam
from utils.logger import *
import numpy as np
import pandas as pd
from sklearn.model_selection import StratifiedKFold
from keras.layers import Input, Dense, Embedding, Lambda
from keras.models import Model
import keras.backend as K
from keras.layers.core import Dropout, Activation

np.random.seed(7)




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

    def _model(self, loss="binary_crossentropy", input_dim = 13, config=None):
        """
           :return:
        """

        #cl4 = partial(custom_loss_4, weights=weights_tensor)


        model = Sequential()
        model.add(Dense( 256, input_shape=(13,)))
        model.add(Activation('relu'))
        model.add(Dropout(0.7))

        model.add(Dense( 256))
        model.add(Activation('relu'))
        model.add(Dropout(0.4))

        model.add(Dense(256))
        model.add(Activation( 'relu'))
        model.add(Dropout(0.1))

        model.add(Dense(1))
        model.add(Activation('sigmoid'))


        model.compile(loss=loss, metrics=['accuracy'], optimizer=Adam(lr=0.001))

        print(model.summary())
        self.__model = model
        return model

    def get_model(self):


        return self.__model

    def train(self, X, Y, X_validation=None, Y_validation=None,  callback=None, loss='binary_crossentropy'):
        """

        :return:
        """
        lr = self.config.get("train", 'lr')

        epochs = self.config.get("train", 'epochs')

        batch_size = self.config.get("train", 'batch_size')


            # Recompile model with new settings
        self.__model.compile(loss=loss,
                             metrics=['accuracy'],
                             optimizer=Adam(lr=float(lr)))
        # Start Training:

        if X_validation is None or Y_validation is None:

            self.__model.fit(X, Y, epochs=int(epochs), batch_size=int(batch_size), callbacks=callback)
        else:
            self.__model.fit(X, Y, epochs=int(epochs), batch_size=int(batch_size),
                             validation_data=(X_validation, Y_validation,), callbacks=callback)  # , verbose=0)

        return

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
                                       validation_data=(X_validation, Y_validation ), callbacks=callback)  # , verbose=0)


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
            with open("{0}/Model{0}.json".format(self.dir + self.model_name), "w") as json_file:
                json_file.write(model_json)
            # serialize weights to HDF5
            models[i].save_weights(self.dir + self.model_name + "/Model_weight_{0}".format(index))
            print("Model is saved on disk")
        return

    def load_model(self):
        """

        :return:
        """
        # load json and create model
        json_file = open(os.path.join(self.dir + self.model_name + '/Model.json'),'r')
        loaded_model_json = json_file.read()
        json_file.close()
        loaded_model = model_from_json(loaded_model_json)
        # load weights into new model
        loaded_model.load_weights(self.dir + self.model_name + "/Model_weight.h5")
        print("Loaded model from disk")
        self.__model = loaded_model
        self.__model.compile(loss='binary_crossentropy', metrics=['accuracy'], optimizer=Adam(lr=0.01))
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

    def get_results(self, _x_train, _y_train, x_test, y_test, _w_train, w_test, index_train=None,
                    index_test=None):

        _df_train = pd.DataFrame()
        _df_test = pd.DataFrame()

        _df_train["train_labels"] = [i[0] for i in _y_train.values]
        _df_test["test_labels"] = [i[0] for i in y_test.values]

        if index_train is None:
            _df_train["index"] = _x_train.index
        else:
            _df_train["index"] = index_train

        _df_train["train_output"] = self.__model.predict(_x_train, verbose=0)
        _df_test["test_output"] = self.__model.predict(x_test, verbose=0)

        _df_train["train_weights"] = _w_train.values
        _df_test["test_weights"] = w_test.values

    
        _df_train["train_pred"] = _df_train["train_output"].apply(lambda x: 1 if x > 0.5 else 0)
        _df_test["test_pred"] = _df_test["test_output"].apply(lambda x: 1 if x > 0.5 else 0)

        if index_test is None:
            _df_test["index"] = x_test.index
        else:
            _df_test["index"] = index_test

        return _df_train, _df_test

    
