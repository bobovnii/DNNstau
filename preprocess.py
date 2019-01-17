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



def convert_to_df(path, directory, map_path):
    """

    :return:
    """
    root_file = uproot.open(path)[directory]
    map_df = pd.read_csv(map_path)
    df = pd.DataFrame(root_file.arrays())




    return

def combine_points():
    """

    :return:
    """