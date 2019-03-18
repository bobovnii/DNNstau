"""
Utils:
"""
import pandas as pd
import numpy as np
from sklearn.metrics import roc_auc_score
from sklearn.utils import shuffle
import  keras
from metric.loss import asimov
from plotter.Plotter import plot_asimov, plot_history
import keras.backend as K
import os

class Histories(keras.callbacks.Callback):
    """

    """
    def set_up_config(self, config):
        """

        :param config:
        :return:
        """
        self.config=config
        return


    def set_mode(self, mode="train"):
        """

        :param mode:
        :return:
        """
        self.mode = mode
        if os.path.exists(self.config.get("model","dir")+self.config.get("model","model_name")):
            pass
        else:
            os.mkdir(self.config.get("model","dir")+self.config.get("model","model_name"))

        return


    def set_train_mode(self, mode="train"):
        """

        :param mode:
        :return:
        """
        self.train_mode  = mode
        return


    def set_up_train_weight(self, weight):
        """

        :param weight:
        :return:
        """
        self.train_weight = weight
        return


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
        self.lr = []



    def on_train_end(self, logs={}):
        """

        :param logs:
        :return:
        """
        #Run Plotter

        #TODO add conditions when to store or not training history

        dir = self.config.get("model", "dir")
        model_name = self.config.get("model", "model_name")
        plot_asimov(self.asimov, title="Asimov Significance", dir=dir, model_name=model_name, mode=self.mode)
        plot_asimov(history={'val': self.lr}, title="Learning Rate", dir=dir, model_name=model_name, mode=self.mode)
        plot_history(history={"loss": self.losses, "acc": self.acc},  dir=dir, model_name=model_name, mode=self.mode)
        self.store_history()
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
        #y_train_pred = self.model.predict(self.x)

        #AUC
        #self.aucs['train'].append(roc_auc_score(self.y, y_pred))
        self.aucs['val'].append(roc_auc_score(self.validation_data[1], y_pred))

        #Asimov
        if self.mode!="fit":
            try:
                #self.asimov['train'].append(asimov(self.x, y_train_pred, self.train_weight))
                self.asimov['val'].append(asimov(self.validation_data[1], y_pred, self.val_weight))
            except Exception:
                pass
        else:
            pass

        #Learning Rate:
        self.lr.append(K.get_value(self.model.optimizer.lr))

        return


	def on_batch_begin(self, batch, logs={}):
		return


	def on_batch_end(self, batch, logs={}):
		return


    def store_history(self):
        """

        :return:
        """
        #Save train
        df = pd.DataFrame()
        df['train_accuracy'] = self.acc['train']
        df['train_loss'] =  self.losses['train']
        df['test_accuracy'] = self.acc['val']
        df['test_loss'] =  self.losses['val']
        df['test_auc'] =  self.aucs['val']
        df['asimov'] =  self.asimov['val']
        df['lr'] = self.lr
        #Save test
        dir = self.config.get("model", "dir")
        model_name = self.config.get("model", "model_name")
        df.to_csv(dir+model_name+'/history_{0}.csv'.format(self.mode))
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



def label_correction(df):
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
        
    _df_train['train_output'] =  [i[0] for i in model.predict_proba(_x_train)]
    _df_test['test_output'] = [i[1] for i in model.predict_proba(x_test)]
        
    _df_train['train_weights'] = _w_train.as_matrix()
    _df_test['test_weights'] = w_test.as_matrix()
    
    _df_train['train_pred'] = _df_train['train_output'].apply(lambda x: 1 if x>0.5 else 0)
    _df_test['test_pred'] = _df_test['test_output'].apply(lambda x: 1 if x>0.5 else 0)
    _df_test['index'] = x_test.index
    return _df_train, _df_test






