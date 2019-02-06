"""
Utils:
"""
import pandas as pd
import numpy as np
##from sklearn.metrics import accuracy_score
from sklearn.metrics import roc_auc_score
from sklearn.utils import shuffle
import math
import  keras
from loss import asimov
from Plotter import plot_asimov

class Histories(keras.callbacks.Callback):

    def set_up_confi(self, config):
        """

        :param config:
        :return:
        """


    def set_up_train_weight(self, weight):
        """

        :param weight:
        :return:
        """
        self.train_weight = weight

    def set_up_val_weight(self, weight):
        """

        :param weight:
        :return:
        """
        self.val_weight = weight


    def on_train_begin(self, logs={}):
        """

        :param logs:
        :return:
        """

        self.aucs = {'train':[], 'val':[]}
        self.losses = {'train':[], 'val':[]}
        self.asimov = {'train':[], 'val':[]}
        self.acc = {'train':[], 'val':[]}


    def on_train_end(self, logs={}):
        """

        :param logs:
        :return:
        """
        #Run Plotter
        plot_asimov(history, title="", dir="", model_name="")
        return


    def on_epoch_begin(self, epoch, logs={}):

        return


    def on_epoch_end(self, epoch, logs={}):
        """

        :param epoch:
        :param logs:
        :return:
        """
        #Loss
        self.losses['train'].append(logs.get('loss'))
        self.losses['val'].append(logs.get('val_loss'))

        #Accuracy
        self.acc['train'].append(logs.get('acc'))
        self.acc['val'].append(logs.get('val_acc'))

        y_pred = self.model.predict(self.validation_data[0])

        #AUC
        self.aucs['train'].append(roc_auc_score(self.validation_data[1], y_pred))
        self.aucs['val'].append(roc_auc_score(self.validation_data[1], y_pred))

        #Asimov
        self.asimov['train'].append(asimov(self.validation_data[1], y_pred, self.train_weight))
        self.asimov['val'].append(asimov(self.validation_data[1], y_pred, self.val_weight))

        return


	def on_batch_begin(self, batch, logs={}):
		return


	def on_batch_end(self, batch, logs={}):
		return



def _overbalance(train):
    """
    Return Oversampled dataset

    :param train:
    :return:
    """
    count_class_0, count_class_1 = train.classID.value_counts()
    # Divide by class
    df_class_0 = train[train['classID'] == 0]
    df_class_1 = train[train['classID'] == 1]
    df_class_1_over = df_class_1.sample(count_class_0, replace=True)
    df_over = pd.concat([df_class_0, df_class_1_over], axis=0)
    df_over = shuffle(df_over)
    return df_over



def label_correction(df, labels=[1,0], class_names=["signal","background"], col_names=["classID", "className"]):
    """

    :param df:
    :param labels:
    :param class_names:
    :param col_names:
    :return:
    """
    df['classID'] = df['classID'].apply(lambda x: 1-np.int32(x>0.5))
    return df



def get_results(model, _x_train, _y_train, x_test, y_test, _w_train, w_test):
    """

    :param model:
    :param _x_train:
    :param _y_train:
    :param x_test:
    :param y_test:
    :param _w_train:
    :param w_test:
    :return:
    """
    _df_train = pd.DataFrame()
    _df_test = pd.DataFrame()

    _df_train['train_labels'] =  [i[0] for i in _y_train.as_matrix()]
    _df_test['test_labels'] = [i[0] for i in y_test.as_matrix()]
    _df_train['index'] = _x_train.index
        
    _df_train['train_output'] = model.predict(_x_train, verbose=0)
    _df_test['test_output'] = model.predict(x_test, verbose=0)
        
    _df_train['train_weights'] = _w_train.as_matrix()
    _df_test['test_weights'] = w_test.as_matrix()
    
    _df_train['train_pred'] = _df_train['train_output'].apply(lambda x: 1 if x>0.5 else 0)
    _df_test['test_pred'] = _df_test['test_output'].apply(lambda x: 1 if x>0.5 else 0)
    _df_test['index'] = x_test.index
    return _df_train, _df_test