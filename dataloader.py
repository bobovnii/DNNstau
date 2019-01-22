import pandas as pd
import numpy as np
import uproot 

class DataLoader():
    """
    """
    def __init__(self,config):
        self.config = config
        return
    
    
    def _get_train(self, convert_to_pd = True):
        
        """
        """
        self.path = self.config.get("data","path")
        self.train_path = self.config.get("data","train")
        self.train_directory = self.config.get("data","train_directory")
        train_df = uproot.open(self.path+self.train_path)[self.train_directory]
        if convert_to_pd ==True:
            train_df = pd.DataFrame(train_df.arrays())
        return train_df
    
    
    def _test_test(self, batch_size):
        """
        """
        self.test_path = self.config.get("data","test")
        self.test_directory = self.config.get("data","test_directory")
        test_df = uproot.open(self.test_path)[self.test_directory]
        #if convert_to_pd ==True:
        #    train_df = pd.DataFrame(train_df.arrays())
    
        return test_df
    
    
    def _get_full_dataset(self, batch_size):
        """
        """
        return
    
    