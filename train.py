"""
Script for the pretraining:
"""
from loss_function import *
from model_diagnostic import train_plots

def _model():
    """
    Definition of model:
    """
    model = Sequential()
    model.add(Dense(256, input_dim=13, kernel_initializer='uniform', activation='relu'))
    model.add(Dense(256, kernel_initializer='uniform', activation='relu'))
    model.add(Dense(256, kernel_initializer='uniform', activation='relu'))
    model.add(Dense(1, kernel_initializer='uniform', activation='sigmoid'))
    # Compile model
    model.compile(loss='binary_crossentropy', metrics=['accuracy'], optimizer=Adam(lr=0.001))

    print(model.summary())
    return model
    
    
    
def train(train_x, train_y, train_w):
    """
    """
    #Define the mode:
    model = _model()
    
    #Fit the model
    history = model.fit(X, Y, epochs=10, batch_size=4000)#, verbose=0)
    #Store logs 
    
    #history.history save to 
    
    #
    
    return model
    
    
def cv_train(train_x, train_y, train_w, num_folds):
    """
    """
    
    return
    
def evaluate_model():
    """
    """
    return
    
    
    