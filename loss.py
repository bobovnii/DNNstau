import numpy as np


def Asimov_Loss(y_true, y_pred, weight, signal_test_sf, bgd_test_sf):
    """
    """
    # Get weighted true positive;
    # Get weighted false negative;
    # Calculate AMS;
    new_y_pred = y_pred
    new_y_true = y_true
    # new_y_pred = [i[0] for i in y_pred]
    # new_y_true = [i[0] for i in y_true.as_matrix()]#np.array
    new_weight = weight  # .as_matrix()
    TP = np.sum(np.logical_and(new_y_pred == 1, new_y_true == 1).astype(np.int32 ) *new_weight *signal_test_sf)
    FP = np.sum(np.logical_and(new_y_pred == 1, new_y_true == 0).astype(np.int32 ) *new_weight *bgd_test_sf)
    if TP == 0:
        asimov = 0.0
    else:
        asimov = TP /np.sqrt(FP +TP)
    return asimov


def significanceLoss(expectedSignal,expectedBkgd):
    '''Define a loss function that calculates the significance based on fixed
    expected signal and expected background yields for a given batch size'''


    def sigLoss(y_true,y_pred):
        #Continuous version:

        signalWeight=expectedSignal/K.sum(y_true)
        bkgdWeight=expectedBkgd/K.sum(1-y_true)

        s = signalWeight*K.sum(y_pred*y_true)
        b = bkgdWeight*K.sum(y_pred*(1-y_true))

        return -(s*s)/(s+b+K.epsilon()) #Add the epsilon to avoid dividing by 0

    return sigLoss