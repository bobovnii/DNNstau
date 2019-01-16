import pandas as pd
import numpy as np

def sf_bgd_train_test(test, train):
    "Number of bgd in test set"
    "Calculate TF for background"
    weight  = test[(test.classID == 0)]['weight']
    bgd_weights=[np.float32(i) for i in weight]
    test_hist, test_bins = np.histogram(test[(test.classID == 0)]['met_pt'], weights=bgd_weights,
                                  bins=10)#, normed=True)
    print("Test BGD:", test_hist.sum())

    weight  = train[(train.classID == 0)]['weight']
    bgd_weights=[np.float32(i) for i in weight]

    train_hist, train_bins = np.histogram(train[(train.classID == 0)]['met_pt'], weights=bgd_weights,
                                  bins=10)#, normed=True)
    print("Train BGD:", train_hist.sum())
    print("Scale F BGD Test:",  test_hist.sum()/(test_hist.sum()+train_hist.sum()))
    scale_f_test = test_hist.sum()/(test_hist.sum()+train_hist.sum())
    print("Scale F BGD Train:",  train_hist.sum()/(test_hist.sum()+train_hist.sum()))
    scale_f_train = train_hist.sum()/(test_hist.sum()+train_hist.sum())
    return scale_f_test, scale_f_train


def sf_signal_train_test(test, train):
    "Number of bgd in test set"
    "Calculate TF for background"
    weight  = test[(test.classID == 1)]['weight']
    signal_weights=[np.float32(i) for i in weight]
    test_hist, test_bins = np.histogram(test[(test.classID == 1)['met_pt'],weights=signal_weights, bins=10)#, normed=True)
    print("Test Signal:", test_hist.sum())
    weight  = train[(train.classID == 1)]['weight']
    signal_weights=[np.float32(i) for i in weight]

    train_hist, train_bins = np.histogram(train[(train.classID == 1)]['met_pt'], weights=signal_weights, bins=10)#, normed=True)
    print("Train Signal:", train_hist.sum())
    print("Scale F Signal Test:",  test_hist.sum()/(test_hist.sum()+train_hist.sum()))
    scale_f_test = test_hist.sum()/(test_hist.sum()+train_hist.sum())
    print("Scale F Signal Train:",  train_hist.sum()/(test_hist.sum()+train_hist.sum()))
    scale_f_train = train_hist.sum()/(test_hist.sum()+train_hist.sum())
    return scale_f_test, scale_f_train