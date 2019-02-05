import math




def step_decay(epoch, epochs_drop=10.0):
   initial_lrate = 0.1
   drop = 0.5
   lrate = initial_lrate * math.pow(drop,
           math.floor((1+epoch)/epochs_drop))
   return lrate


def periodical_lr():
    """

    :return:
    """
    return


def step_thresould(epoch, epochs_drop=10.0):
    """

    :return:
    """
    return