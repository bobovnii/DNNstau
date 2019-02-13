import uproot
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.utils import shuffle



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
    signal = df[(df.classID == 1)]
    background = df[(df.classID == 0)]

    signal_train, signal_test = train_test_split(signal, split=split)
    background_train, background_test = train_test_split(background,
                                                         split=split)

    train = shuffle(pd.concat([signal_train, background_train]))
    test = shuffle(pd.concat([signal_test, background_test]))


    return train, test