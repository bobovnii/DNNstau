import uproot
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
from sklearn.metrics import classification_report
from sklearn.utils import shuffle
from sklearn.metrics import roc_auc_score
from sklearn.metrics import roc_curve
import matplotlib.pyplot as plt
from sklearn.model_selection import KFold
import os
import math

import logging
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
from sklearn.metrics import classification_report
from sklearn.utils import shuffle
from sklearn.metrics import roc_auc_score
from sklearn.metrics import roc_curve
import matplotlib.pyplot as plt
from sklearn.model_selection import KFold


def convert_to_df(path, directory):
    """

    :return:
    """
    root_file = uproot.open(path)[directory]
    df = pd.DataFrame(root_file.arrays())

    return df

def combine_points():
    """

    :return:
    """
    return

def test_train_split(df, split=0.4):
    """

    :param split:
    :return:
    """
    train, test = train_test_split(df, test_size=split)
    return train, test