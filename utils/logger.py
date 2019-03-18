import logging
import os


def set_logger(file_name=None, config=None):
    ###Set logger
    if file_name is None:
        model_dir = config.model_dir
        logging_dir = config.get("LOG_DIR")
        model_version = config.get("MODEL_VERSION")

        training_name = config.get("TRAINING_NAME")

        file_name = os.path.join(model_dir, logging_dir) + "{0}_{1}.log".format(model_version, training_name)

    logger = logging.getLogger()
    logger.setLevel(logging.INFO)
    print("Logger:", file_name)
    file_handler = logging.FileHandler('{0}'.format(file_name))
    file_handler.setFormatter(logging.Formatter('%(asctime)s:%(levelname)s: %(message)s'))
    logger.addHandler(file_handler)
    ##TODO add model definition to the logging: WHY?
