"""
Warm restart scheme
From https://arxiv.org/pdf/1608.03983.pdf
"""
import  math
def warm_restart():
    """

    :return:
    """
    list_of_lr = []
    T_i = epoch_number_period
    n_max = max(list_of_lr)
    n_min = min(list_of_lr)
    T_cur = n_epoch_performed_from_restart
    #Optional variable - T_mul


    new_lr = n_min + 0.5*(n_max - n_min)*(1 + math.cos(3.14*T_cur/T_i))
    return new_lr