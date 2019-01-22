import logging
import os


def set_logger(config=None, file_name=None):
    ###Set logger
    if file_name is None:
        model_dir = config.get("model","dir")
        logging_dir =config.get("model","model_name")

        file_name = os.path.join(model_dir, logging_dir)
    logger = logging.getLogger()
    logger.setLevel(logging.INFO)
    file_handler = logging.FileHandler('{0}'.format(file_name))
    file_handler.setFormatter(logging.Formatter('%(asctime)s:%(levelname)s: %(message)s'))
    logger.addHandler(file_handler)
    ##TODO add model definition to the logging: WHY?