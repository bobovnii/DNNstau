
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
    new_weight = weigh  t  # .as_matrix()
    TP = np.sum(np.logical_and(new_y_pred == 1, new_y_true == 1).astype(np.int32 ) *new_weigh t *signal_test_sf)
    FP = np.sum(np.logical_and(new_y_pred == 1, new_y_true == 0).astype(np.int32 ) *new_weigh t *bgd_test_sf)
    if TP == 0:
        asimov = 0.0
    else:
        asimov = T P /np.sqrt(F P +TP)
    return asimov