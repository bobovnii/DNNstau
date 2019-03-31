
import os
import sys
import argparse
arguments = sys.argv


os.system("cd /nfs/dust/cms/user/dydukhle/CMSSW_9_4_0_patch1/src/DesyTauAnalyses/NTupleMaker/test")#;eval `scramv1 runtime -sh` ;
dir="/nfs/dust/cms/user/dydukhle/CMSSW_9_4_0_patch1/src/DesyTauAnalyses/NTupleMaker/test"
i=0
print(arguments)
channel=arguments[2]
channel2=arguments[2]
dirhere=arguments[2]
btag="0.8484"

if len(arguments) >3:
    systematics=arguments[3]
    syst = "Nominal"
else:
    systematics="Nominal"
    syst = "Nominal"


###########start looking 
with open(arguments[1]) as f:
    content = f.readlines()

content = [x.strip() for x in content]
for i in range(len(content)):
    filename = content[i]
    isDataSyst=0
    isSystTopZPt=0

    file=content[i].split("/")[-1]
    fileB=content[i].split("/")[-1].split(".")[0]

    path = "dir_{0}_{1}_{2}".format(fileB, channel, syst)
    print(file)
    try:
            os.mkdir(path)
    except Exception:
            pass
    working_dir = "dir_{0}_MuTau_{2}".format(fileB, channel, syst)
    print(" ")
    print("Workdir", working_dir)
    #Check if data:
    if "Single" in file or "MuonEG" in file or "Nominal" not in syst:
        print("For data we dont run systematics....")
        isDataSyst=1

    fileB=fileB+"_"+syst


    ######### B region non normal Tau
    print("{0} , {0} , {0} , {0} ".format(file, fileB, channel, channel2))

    ######### signal
    #if [[ $file == *".root"*  ]] || [[ $file == *"stau"* ]] &&  [[ ! -f $dir/plots_$channel/${fileB}_B.root ]] ; then


    #echo Signal file here .....

    print("echo the signal filein : {0} , the fileout : {1}_A.root normal Tau".format(file, fileB))
    if os.path.exists("./plots_MuTau/{0}_A.root".format(fileB)):
    	continue
    os.system("mkdir {0}".format(working_dir))
    os.system("cp {0}/analyzer_MuTau_h {0}/{1}/analyzer.h".format(dir, working_dir))
    os.system("cp {0}/analyzer_MuTau_C {0}/{2}/analyzer.C".format(dir, channel, working_dir))
    os.system("cp {0}/runme.C {0}/{1}/.".format(dir,working_dir))
    os.system("cp {0}/plots_MuTau.h {0}/{1}/plots.h".format(dir, working_dir))

    print("Go to dir {0}/{1}".format(dir, working_dir))
    #os.system("cd {0}/{1}".format(dir, working_dir))
    os.chdir("{0}/{1}".format(dir, working_dir))

    print("Changed dir")

    os.system("sed -i 's/CHIMASSS/{0}/g' analyzer*.*C".format(0, working_dir))
    os.system("sed -i 's/CHANNELHERE/{0}/g' analyzer*.*".format(channel, working_dir))
    ##TODO: find variable dirhere
    os.system("sed -i 's/DIRHERE/{0}/g' analyzer*.*".format(dirhere, working_dir))
    os.system("sed -i 's/SYSTEMATICHERE/{0}/g' analyzer*.*".format(syst, working_dir))
    os.system("sed -i 's/REGION/{0}/g' analyzer*.*".format('CR', working_dir))



    #echo "FILEIN" $filename
    print("File in:", filename)
    os.system("sed -i 's/FILEIN/{0}/g' analyzer.*".format(file, working_dir))
    os.system("sed -i 's/LEPTONHERE/false/g' analyzer.*".format('false', working_dir))
    os.system("sed -i 's/SIGNHERE/{0}/g' analyzer.C".format('OS', working_dir))
    os.system("sed -i 's/CHANNELHERE/{0}/g' analyzer*.*".format(channel, working_dir))
    os.system("sed -i 's/DIRHERE/{0}/g' analyzer*.*".format(dirhere, working_dir))
    os.system("sed -i 's/BTAGCUT/{0}/g' analyzer*.*".format(0.8484, working_dir))
    os.system("sed -i 's/SYSTEMATICHERE/{0}/g' analyzer*.*".format(syst, working_dir))

  
    #os.system("cd {0}/{1}".format(dir, working_dir))
    
    os.system("rm plots.root".format(dir,working_dir))
    print("Run root")
    os.system("root -l -q -b runme.C".format(dir, working_dir))
    os.system("mv {0}/{3}/plots.root {0}/plots_MuTau/{2}_A.root".format(dir, channel, fileB, working_dir))




