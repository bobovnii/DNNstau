#!/usr/bin/env python

from ROOT import TMVA, TFile, TTree, TCut
from subprocess import call
from os.path import isfile
from keras.models import Sequential
from keras.layers import Dense, Dropout
from keras.models import model_from_json
from keras.utils import plot_model
from keras.models import Sequential
from keras.layers import Dense, Activation
from keras.regularizers import l2
from keras import initializers
from keras.optimizers import Adam, SGD
from os import environ

environ['KERAS_BACKEND'] = 'tensorflow'
FILES_LIST = "datasets2"

import os
import sys
path = sys.argv[-1]


###  Define constant:
MuMass = 0.105658367
tauMass = 1.776
electronMass = 0.51100e-3
muonMass = 0.10565837
pionMass = 0.1396
bTagCut  = 0.8000


#### variables to train with
variables = ["met_pt"
            "Lept1Pt",
            "Lept2Pt",
            "EtaDil",
            "MTtot",
            "Dzeta",
            "Minv",
            "dR",
            "MT",
            "MCTb",
            "MT2lester",
            "dMETPhiL1",
            "dMETPhiL2"]

# Setup TMVA
numVariables = len(variables)
TMVA.Tools.Instance()
TMVA.PyMethodBase.PyInitialize()

# output file
# TMVA factory
output = TFile.Open('TMVA.root', 'RECREATE')
factory = TMVA.Factory('TMVAClassification', output,
                       '!V:!Silent:Color:DrawProgressBar:Transformations=I:AnalysisType=Classification')
# Load data
TMVA_FILE = "/nfs/dust/cms/user/dydukhle/CMSSW_9_4_0_patch1/src/DesyTauAnalyses/NTupleMaker/" \
            "test/MVA/TMVA_100withoutNormGiniIndexPray_Test_datasets2_gen_level_met_all_features.root"

File_In = TFile.Open(TMVA_FILE)

signal = File_In.Get('dataset/TrainTree')
bgd = File_In.Get('dataset/TestTree')

# intiate the dataloader
# signal tree name
dataloader = TMVA.DataLoader('dataset')

# get the list of branches ---> corresponding to the variable names mentioned above
for branch in signal.GetListOfBranches():
    if branch.GetName() in variables:
        dataloader.AddVariable(branch.GetName())

# preselections
sigCuts = TCut("classID==1")
bkgCuts = TCut("classID==0")


# load the signal tree
dataloader.SetInputTrees(signal, sigCuts, bkgCuts)


dataloader.PrepareTrainingAndTestTree('nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V')


# later...
MODEL_NAME = "Model.json"
MODEL_WEIGHT = "Model_Weight.h5"


# load json and create model
json_file = open(MODEL_NAME, 'r')
loaded_model_json = json_file.read()
json_file.close()
loaded_model = model_from_json(loaded_model_json)
# load weights into new model
loaded_model.load_weights(MODEL_WEIGHT)
# evaluate loaded model on test data
loaded_model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])

loaded_model.save('model.h5')
loaded_model.summary()
plot_model(loaded_model, to_file='model.png')

# Book methods
# factory.BookMethod(dataloader, TMVA.Types.kFisher, 'Fisher',
#                   '!H:!V:Fisher:VarTransform=D,G')
factory.BookMethod(dataloader, TMVA.Types.kPyKeras, 'PyKeras',
                   'H:!V:VarTransform=D,G:FilenameModel=model.h5:NumEpochs=10:BatchSize=32:Tensorboard=./logs',)

# Run training, test and evaluation
#factory.TrainAllMethods()
#factory.TestAllMethods()
factory.EvaluateAllMethods()
