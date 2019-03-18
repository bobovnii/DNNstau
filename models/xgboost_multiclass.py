"""
Multiclass classification
"""

from sklearn.model_selection import StratifiedKFold
from sklearn.metrics import confusion_matrix
import matplotlib.pyplot as plt
import itertools
from collections import Counter
import os
import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from keras.utils import to_categorical
import numpy as np
from functools import partial, wraps
import uproot
from sklearn.ensemble import  AdaBoostClassifier
# Read from configuration:


DATASET = "/nfs/dust/cms/user/dydukhle/STAU/Training/MultiClassification/"
classes_names = ["signal", "ST", "TT", "ZZ", "WW", "WJ", "DY", "Fakes"]
VARS = ["met_pt", "Lept1Pt", "Lept2Pt", "EtaDil", "MTtot", "Dzeta", "dR", "Minv", "MT", "MCTb", "MT2lester",
        "dMETPhiL1", "dMETPhiL2"]
n_classes = len(classes_names)
split = 0.4

best_params = {
    'objective': 'multi:softprob',
   # 'eval_metric': 'mlogloss',
    'silent': True,
    'n_classes': 2,
    'booster': 'gbtree',
    'n_estimators': 1000,
    'tree_method': 'hist',
#    'grow_policy': 'lossguide',
    'max_depth': 7,
    'base_score': 0.25,
    'max_delta_step': 2,
    'seed': 538,
    'colsample_bytree': 0.3,
    'gamma': 0.1,
    'learning_rate': 0.02,
#    'max_leaves': 11,
    'min_child_weight': 64,
    'reg_alpha': 0.001,
    'reg_lambda': 10.0,
    'subsample': 0.9}


def multi_weighted_logloss(y_true, y_preds, classes, class_weights):
    """
    refactor from
    @author olivier https://www.kaggle.com/ogrellier
    multi logloss for PLAsTiCC challenge
    """
    y_p = y_preds.reshape(y_true.shape[0], len(classes), order='F')
    # Trasform y_true in dummies
    y_ohe = pd.get_dummies(y_true)
    # Normalize rows and limit y_preds to 1e-15, 1-1e-15
    y_p = np.clip(a=y_p, a_min=1e-15, a_max=1 - 1e-15)
    # Transform to log
    y_p_log = np.log(y_p)
    # Get the log for ones, .values is used to drop the index of DataFrames
    # Exclude class 99 for now, since there is no class99 in the training set
    # we gave a special process for that class
    y_log_ones = np.sum(y_ohe.values * y_p_log, axis=0)
    # Get the number of positives for each class
    nb_pos = y_ohe.sum(axis=0).values.astype(float)
    # Weight average and divide by the number of positives
    class_arr = np.array([class_weights[k] for k in sorted(class_weights.keys())])
    y_w = y_log_ones * class_arr / nb_pos

    loss = - np.sum(y_w) / np.sum(class_arr)
    return loss


def xgb_multi_weighted_logloss(y_predicted, y_true, classes, class_weights):
    """

    :param y_predicted:
    :param y_true:
    :param classes:
    :param class_weights:
    :return:
    """
    loss = multi_weighted_logloss(y_true.get_label(), y_predicted,
                                  classes, class_weights)
    return 'wloss', loss


def dataloader():
    df = uproot.open(DATASET + "TMVA_150withoutNormGiniIndexPray_Test.root")
    class_map = pd.read_csv(DATASET + "TMVA_150withoutNormGiniIndexPray_Test.root.csv", header=None,
                            names=['Dataset', 'id'])
    train = df['dataset']['TrainTree']
    df_train = pd.DataFrame(train.arrays())
    return df_train, class_map


def preprocess(df_train, class_map, classsplit, VARS):
    """
    """
    MAP = {
        "signal": [1, 2, 3, 4, 5],
        "ST": [6, 7, 8, 9, 10],
        "TT": [11, 12, 13, 14, 15, 16, 17, 18],
        "ZZ": [26, 32, 33, 34, 35, 36, 37],
        "WW": [19, 20, 21, 22, 23, 24, 25, 27, 28, 29, 30, 31],
        "WJ": [38, 38, 39, 40, 41, 42, 43],
        "DY": [44, 45, 46, 47, 48, 49],
        "Fakes": [50]
    }
    KEYS = ["signal", "ST", "TT", "ZZ", "WW", "WJ", "DY", "Fakes"]

    index_dict = {}
    name_dict = {}
    for key, value in MAP.items():
        for i in value:
            index_dict[i] = KEYS.index(key)
            name_dict[i] = key
    df_train.Dataset_id = df_train.Dataset_id.astype(int)
    df_train['process'] = df_train.Dataset_id.apply(lambda x: index_dict[x])
    train, test = train_test_split(df_train, test_size=0.4)

    x_train = train[VARS]
    y_train = train[["process"]]
    w_train = train['weight']
    x_test = test[VARS]
    y_test = test[["process"]]
    w_test = test['weight']

    # TODO get y_train/test categorical
    unique_y = np.unique(y_train)
    class_map = dict()
    for i, val in enumerate(unique_y):
        class_map[val] = i

    y_map = np.zeros((y_train.shape[0],))
    y_map = np.array([class_map[val] for val in y_train['process'].as_matrix()])

    y_train = y_train.astype(np.int32)
    y_test = y_test.astype(np.int32)
    y_train_map = to_categorical(y_train, num_classes=8)
    y_test_map = to_categorical(y_test, num_classes=8)

    return x_train, y_train, y_train_map, y_map, w_train, x_test, y_test, y_test_map, w_test


def _model(params):
    clf = XGBClassifier(**params)
    return clf


def plot_loss_acc(history, title):
    plt.plot(history.history['loss'][1:])
    plt.plot(history.history['val_loss'][1:])
    plt.title('model loss')
    plt.ylabel('val_loss')
    plt.xlabel('epoch')
    plt.legend(['train', 'Validation'], loc='upper left')
    plt.savefig('Loss_{0}.pdf'.format(title))

    plt.plot(history.history['acc'][1:])
    plt.plot(history.history['val_acc'][1:])
    plt.title('model Accuracy')
    plt.ylabel('val_acc')
    plt.xlabel('epoch')
    plt.legend(['train', 'Validation'], loc='upper left')
    plt.savefig('Acc_{0}.pdf'.format(title))


def train(x_train, y_train_map, y_map, class_weights, classes, params):
    """
    """
    clfs = []
    x_train = x_train.reset_index(drop=True)

    folds = StratifiedKFold(n_splits=4, shuffle=True, random_state=1)
    index = 0
    oof_preds = np.zeros((len(x_train), n_classes))
    func_loss = partial(xgb_multi_weighted_logloss,
                        classes=classes,
                        class_weights=class_weights)
    # w = y.value_counts()
    # weights = {i: np.sum(w) / w[i] for i in w.index}
    weights = {i: 1 for i in classes}

    for fold_, (trn_, val_) in enumerate(folds.split(y_map, y_map)):
        index += 1
        _x_train, _y_train = x_train.ix[trn_], y_train_map[trn_]
        x_valid, y_valid = x_train.ix[val_], y_train_map[val_]
        model = _model(params)
        model.fit(
            _x_train, _y_train,
            eval_set=[(_x_train, y_valid), (x_valid, y_valid)],
            eval_metric=func_loss,
            verbose=100,
            early_stopping_rounds=50,
            sample_weight=_y_train.map(weights)
        )
        clfs.append(model)
        oof_preds[val_, :] = model.predict_proba(x_valid, ntree_limit=model.best_ntree_limit)
        print('no {}-fold loss: {}'.format(fold_ + 1, multi_weighted_logloss(y_valid, oof_preds[val_, :],
                                                                             classes, class_weights)))

    return clfs, oof_preds


def voting_prediction(clfs, x_test):  # _df_test):
    """
    """
    preds_ = None
    i = 0
    for clf in clfs:
        i += 1
        if preds_ is None:
            preds_ = clf.predict_proba(x_test.values)
        else:
            preds_ += clf.predict_proba(x_test.values)

    preds_ = preds_ / len(clfs)
    preds_df = pd.DataFrame(preds_)
    return preds_df


def plot_confusion_matrix(cm, classes,
                          normalize=False,
                          title='Confusion matrix',
                          cmap=plt.cm.Blues):
    """
    # http://scikit-learn.org/stable/modules/generated/sklearn.metrics.confusion_matrix.html

    This function prints and plots the confusion matrix.
    Normalization can be applied by setting `normalize=True`.
    """
    if normalize:
        cm = cm.astype('float') / cm.sum(axis=1)[:, np.newaxis]
        print("Normalized confusion matrix")
    else:
        print('Confusion matrix, without normalization')

    print(cm)

    plt.imshow(cm, interpolation='nearest', cmap=cmap)
    plt.title(title)
    plt.colorbar()
    tick_marks = np.arange(len(classes))
    plt.xticks(tick_marks, classes, rotation=45)
    plt.yticks(tick_marks, classes)

    fmt = '.2f' if normalize else 'd'
    thresh = cm.max() / 2.
    for i, j in itertools.product(range(cm.shape[0]), range(cm.shape[1])):
        plt.text(j, i, format(cm[i, j], fmt),
                 horizontalalignment="center",
                 color="white" if cm[i, j] > thresh else "black")

    plt.ylabel('True label')
    plt.xlabel('Predicted label')
    plt.savefig('{0}.pdf'.format(title))
    plt.tight_layout()


def conf_matrix(y_true, y_pred, title='Confusion matrix'):
    """
    """
    # plt.figure(figsize=(12, 12))
    cnf_matrix = confusion_matrix(y_true, np.argmax(y_pred, axis=-1))
    foo = plot_confusion_matrix(cnf_matrix, classes=classes_names, normalize=True,
                                title=title)
    return


def get_results():
    """

    """

    return


def main():
    """

    """

    split = 0.3
    # load data:

    df_train, class_map = dataloader()

    # preprocess:
    x_train, y_train, y_train_map, y_map, w_train, x_test, y_test, y_test_map, w_test = preprocess(df_train, class_map,
                                                                                                   split, VARS)

    # train:
    classes = list(range(0, 8))
    class_weights = [1 for i in classes]
    clsf, oof_pred = train(x_train, y_train_map, y_map, classes, class_weights, params=best_params)

    # evaluate :
    # oof_pred = np.argmax(oof_pred, axis=-1)
    print("Pred", oof_pred)

    # conf_matrix(y_true=y_train_map, y_pred=oof_pred, title='Confusion_matrix_train')

    # test :
    preds_df = voting_prediction(clsf, x_test)
    y_pred = preds_df.as_matrix()
    # save
    # conf_matrix(y_true=y_test_map, y_pred=y_pred, title='Confusion_matrix_test')

    preds_df = pd.DataFrame(preds_df)
    
    oof_pred = pd.DataFrame(oof_pred)
    preds_df.to_csv("DNN_Test_Prediction.csv", header=False)
    oof_pred.to_csv("DNN_Train_Prediction.csv", header=False)
    
    y_train_map_df = pd.DataFrame(y_train_map)
    y_test_map_df = pd.DataFrame(y_test_map)
    
    y_train_map_df.to_csv("DNN_Train_True.csv", header=False)
    y_test_map_df.to_csv("DNN_Test_True.csv", header=False)
    
    
    return


main()
