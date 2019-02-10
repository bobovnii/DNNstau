import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os
from sklearn import  metrics


class Plotter():
    def __init__(self, working_dir):
        """
        Simple Plotter
        
        """
        self.dir = working_dir

        #if directory doesn't exist create new one:
        if os.path.exists(working_dir):
            pass
        else:
            os.mkdir(working_dir)

        return
    
    def train_test_plot(self, df_train, df_test, bgd_train_sf, bgd_test_sf, signal_train_sf, signal_test_sf):
        """
        Train test plots from https://github.com/aelwood/hepML
        """
        decisions = []
        bins=30
        for X,y in ((df_train, df_train['train_labels']), (df_test, df_test['test_labels'])):
            try:
                d1 = X[y>0.5]['train_output'].ravel()
                d2 = X[y<0.5]['train_output'].ravel()
            except Exception:
                d1 = X[y>0.5]['test_output'].ravel()
                d2 = X[y<0.5]['test_output'].ravel()
            finally:
                decisions += [d1, d2]
        low = min(np.min(d) for d in decisions)
        high = max(np.max(d) for d in decisions)
        low_high = (low,high)
        
      
        bgd_weights=[np.float32(i)*np.float32(bgd_train_sf) for i in df_train[df_train.train_labels==0]['train_weights']]
        signal_weights=[np.float32(41.6*i)*np.float32(signal_train_sf) for i in  df_train[df_train.train_labels==1]['train_weights']]
        
        
        plt.hist(decisions[0],
                 color='r', alpha=0.5, range=low_high, bins=bins,
                 histtype='stepfilled',  weights=signal_weights, #normed=True,
                 label='S (train)')
        plt.hist(decisions[1], 
                 color='b', alpha=0.5, range=low_high, bins=bins,
                 histtype='stepfilled', weights=bgd_weights, #normed=True, 
                 label='B (train)')

        bgd_weights=[np.float32(i)*np.float32(bgd_test_sf) for i in df_test[df_test.test_labels==0]['test_weights']]
        signal_weights=[np.float32(i)*np.float32(signal_test_sf) for i in  df_test[df_test.test_labels==1]['test_weights']]

        
        hist, bins = np.histogram(decisions[2], weights=signal_weights,
                                  bins=bins, range=low_high)
        print("signal:", sum(hist))
        scale = len(decisions[2]) / sum(hist)
        err = np.sqrt(hist * scale) / scale

        width = (bins[1] - bins[0])
        center = (bins[:-1] + bins[1:]) / 2
        plt.errorbar(center, hist, yerr=err, fmt='o', c='r', label='S (test)')

        hist, bins = np.histogram(decisions[3], weights=bgd_weights,
                                  bins=bins, range=low_high)
        print("background:", sum(hist))

        scale = len(decisions[3]) / sum(hist)
        err = np.sqrt(hist * scale) / scale

        plt.errorbar(center, hist, yerr=err, fmt='o', c='b', label='B (test)')

        plt.xlabel("Classifier output")
        plt.ylabel("Arbitrary units")
        plt.yscale('log')
        plt.legend(loc='best')
        #if not os.path.exists(output): os.makedirs(output)
        #plt.show()
        plt.savefig(os.path.join(self.dir,'compareTrainTest.pdf'))
        plt.clf()

        return
    
    def class_prediction(self, df, save=False, bgd_test_sf=1, signal_test_sf=1):
        """
        Class prediction plots  from https://github.com/aelwood/hepML
        """

        bgd_weights=[np.float32(i)*np.float32(bgd_test_sf) for i in df[df.test_labels==0]['test_weights']]
        signal_weights=[np.float32(i)*np.float32(signal_test_sf) for i in  df[df.test_labels==1]['test_weights']]


        self.h1=plt.hist(df[df.test_labels==0]['test_output'], weights=bgd_weights, bins=5000,color='b',alpha=0.8,label='background',cumulative=-1)
        self.h2=plt.hist(df[df.test_labels==1]['test_output'], weights=signal_weights, bins=5000,color='r',alpha=0.8,label='signal',cumulative=-1)
        plt.yscale('log')
        plt.ylabel('Cumulative event counts / 0.02')
        plt.xlabel('Classifier output')
        plt.legend()
        if save:
            plt.savefig(os.path.join(self.dir,'class_prediction.pdf'))
        #plt.show()
        return
    
    def significance_plot(self, df_test, bgd_test_sf, signal_test_sf):
        """
        Significance plots from https://github.com/aelwood/hepML
        """
        self.class_prediction(df=df_test, bgd_test_sf=bgd_test_sf, signal_test_sf=signal_test_sf)
        s=self.h2[0]
        b=self.h1[0]

        plt.plot((self.h1[1][:-1]+self.h1[1][1:])/2,s/b)
        plt.title('sig/bkgd on test set')
        plt.savefig(os.path.join(self.dir,'sigDivBkgdDiscriminator.pdf'))
        #plt.show()
        plt.clf()
        
        plt.plot((self.h1[1][:-1]+self.h1[1][1:])/2,s/np.sqrt(s+b))
        plt.title('sig/sqrt(sig+bkgd) on test set, best is '+str(max(s/np.sqrt(s+b))))
        plt.savefig(os.path.join(self.dir,'sensitivityDiscriminator.pdf'))
        #plt.show()
        plt.clf()
        
        return

    def roc_curve(self, df_train, df_test):
        
        _roc_auc_score = metrics.roc_auc_score(df_test['test_labels'], df_test['test_pred'])

        _fpr, _tpr, _thresholds = metrics.roc_curve(df_train['train_labels'], df_train['train_output'])
        fpr, tpr, thresholds = metrics.roc_curve(df_test['test_labels'], df_test['test_output'])

        plt.figure()
        lw = 2
        plt.plot(fpr, tpr, color='darkorange',
                  label='ROC curve')
        plt.plot(_fpr, _tpr, color='darkred',
                  label='Train ROC curve')
        plt.plot([0, 1], [0, 1], color='navy', lw=lw, linestyle='--')
        plt.xlim([0.0, 1.0])
        plt.ylim([0.0, 1.05])
        plt.xlabel('False Positive Rate')
        plt.ylabel('True Positive Rate')
        plt.title('Receiver operating characteristic at AUC %f'%_roc_auc_score)
        plt.legend(loc="lower right")
        plt.savefig(os.path.join(self.dir,'ROC_Curva.pdf'))
        #plt.show()
        return


def plot_asimov(history, title="", dir="", model_name=""):
    """

    :param hisotry:
    :param title:
    :return:
    """
    bbox = dict(boxstyle="round", fc="blue", alpha=0.1)
    print(history)
    #plt.plot(history["train"])
    plt.plot(history["val"])
    plt.title("model significance")
    plt.ylabel("significance")
    plt.xlabel("epoch")
    plt.legend(["validation"], loc="upper left")


    try:
        _max = max(history.history["val"])
        _index = history.history["val"].index(_max)
    except Exception:
        pass
    else:
        plt.annotate('{0}'.format(_max),
                 size=5,
                 xy=(_index - 0.05, _max - 0.01),
                 xytext=(_index - 0.05, _max - 0.01),
                 bbox=bbox  # arrowprops=dict(facecolor='black', shrink=0.05),
                 )

    plt.savefig(os.path.join(dir + model_name, "Significance_{0}.pdf".format(title)))
    # plt.show()
    plt.clf()


def plot_history(history, title="", dir="", model_name=""):
        """"

        """


        # summarize history for accuracy


        bbox = dict(boxstyle="round", fc="blue", alpha=0.1)


        plt.plot(history["acc"]['train'])
        plt.plot(history["acc"]['val'])
        plt.title("model accuracy")
        plt.ylabel("accuracy")
        plt.xlabel("epoch")
        plt.legend(["train", "validation"], loc="upper left")

        _max =max(history["acc"]['train'])
        _index = history["acc"]['train'].index(_max)

        plt.annotate('{0}'.format(_max),
                     size=5,
                     xy=(_index - 0.05, _max - 0.01),
                     xytext=(_index - 0.05, _max - 0.01),
                     bbox=bbox  # arrowprops=dict(facecolor='black', shrink=0.05),
                     )

        _max = max(history["acc"]['val'])
        _index = history["acc"]['val'].index(_max)

        plt.annotate('{0}'.format(_max),
                     size=5,
                     xy=(_index - 0.05, _max - 0.01),
                     xytext=(_index - 0.05, _max - 0.01),
                     bbox=bbox  # arrowprops=dict(facecolor='black', shrink=0.05),
                     )

        plt.savefig(os.path.join(dir +  model_name, "Accuracy_{0}.pdf".format(title)))
        #plt.show()
        plt.clf()

        # summarize history for loss
        plt.plot(history["loss"]['train'])
        plt.plot(history["loss"]['val'])
        plt.title("model loss")
        plt.ylabel("loss")
        plt.xlabel("epoch")
        plt.legend(["train", "validation"], loc="upper left")

        _min = min(history["loss"]['train'])
        _index = history["loss"]['train'].index(_min)

        plt.annotate('{0}'.format(_min),
                     size=5,
                     xy=(_index - 0.05, _min - 0.01),
                     xytext=(_index - 0.05, _min-0.01),
                     bbox=bbox)  # arrowprops=dict(facecolor='black', shrink=0.05),

        _min = min(history["loss"]['val'])
        _index = history["loss"]['val'].index(_min)

        plt.annotate('{0}'.format(_min),
                     size=5,
                     xy=(_index - 0.05, _min + 0.01),
                     xytext=(_index - 0.05, _min + 0.01),
                     bbox=bbox) # arrowprops=dict(facecolor='black', shrink=0.05),


        plt.savefig(os.path.join(dir +  model_name, "Loss_{0}.pdf".format(title)))
        #plt.show()
        plt.clf()

        return



