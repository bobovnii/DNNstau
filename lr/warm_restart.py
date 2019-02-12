"""
Warm restart scheme
From https://arxiv.org/pdf/1608.03983.pdf
"""
import  math

def warm_restart(T_i, list_of_lr, T_cur):
    """

    :return:
    """
    n_max = max(list_of_lr)#0.1  paremeter
    n_min = min(list_of_lr)#0.0001 parameter
    #Optional variable - T_mul


    new_lr = n_min + 0.5*(n_max - n_min)*(1 + math.cos(3.14*T_cur/T_i))
    return new_lr