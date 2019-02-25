
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

    path = "dir_{0}_{1}_{2}".format(file, channel, syst)
    print(file)
    try:
            os.mkdir(path)
    except Exception:
            pass
    working_dir = "./dir_{0}_{1}_{2}".format(file, channel, syst)

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

    print("echo the signal filein : {0} , the fileout : {1}_B.root normal Tau".format(file, fileB))
    if os.path.exists("./plots_mutau/{0}_B.root".format(fileB)):
    	continue

    os.system("cp {0}/analyzer_h analyzer.h".format(dir))
    os.system("cp {0}/analyzer{1}_C analyzer.C".format(dir, channel))
    os.system("sed -i 's/CHIMASSS/{0}/g' analyzer*.*C".format(0))
    os.system("sed -i 's/CHANNELHERE/{0}/g' analyzer*.*".format(channel2))
    os.system("sed -i 's/DIRHERE/{0}/g' analyzer*.*".format(dirhere))
    os.system("sed -i 's/SYSTEMATICHERE/{0}/g' analyzer*.*".format(syst))
    os.system("sed -i 's/REGION/{0}/g' analyzer*.*".format('SR'))


    os.system("cp $dir/runme.C .")
    os.system("cp $dir/plots.h .")

    #echo "FILEIN" $filename
    print("File in:", filename)
    os.system("sed -i 's/FILEIN/{0}/g' analyzer.*".format(file))
    os.system("sed -i 's/LEPTONHERE/{0}/g' analyzer.C".format('false'))
    os.system("sed -i 's/SIGNHERE/{0}/g' analyzer.C".format('OS'))
    os.system("sed -i 's/CHANNELHERE/{0}/g' analyzer*.*".format(channel2))
    os.system("sed -i 's/DIRHERE/{0}/g' analyzer*.*".format(dirhere))
    os.system("sed -i 's/BTAGCUT/{0}/g' analyzer*.*".format(0.8484))
    os.system("sed -i 's/SYSTEMATICHERE/{0}/g' analyzer*.*".format(syst))

    os.system("rm plots.root")
    os.system("root -l -q -b runme.C")
    os.system("mv plots.root {0}/plots_{1}/{2}_B.root".format(dir, channel, fileB))



    ######### A region loose no tight Tau

    os.system("echo {0}".format(fileB))


    #if [[ ! -f $dir/plots_$channel/${fileB}_A.root ]] && [[ $file != *"stau"* && $file != *"C1"*  &&  $file != *"Chi"* && $2 != "Ttemplate" && $2 != "mumu" && $2 != "WJETSMU"    &&  $2 != *"fakesmu"*  &&  $2 != *"WJetsCR"*   ]] ;then
    #echo Start!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    os.system("cp {0}/analyzer_h analyzer.h".format(dir))
    os.system("cp {0}/analyzer{0}_C analyzer.C".format(dir, channel))
    os.system("cp {0}/runme.C .".format(dir))
    os.system("cp {0}/plots.h .".format(dir))

    print("the filein : {0} , the fileout : {1}_A.root , region loose no tight Tauecho ".format(file,fileB))

    os.system("sed -i 's/FILEIN/{0}/g' analyzer.*".format(file))
    os.system("sed -i 's/LEPTONHERE/{0}/g' analyzer.C".format('false'))
    os.system("sed -i 's/SIGNHERE/{0}/g' analyzer.C:".format('OS'))
    os.system("sed -i 's/CHANNELHERE/{0}/g' analyzer.*".format(channel2))
    os.system("sed -i 's/DIRHERE/{0}/g' analyzer.*".format(dirhere))
    os.system("sed -i 's/BTAGCUT/{0}/g' analyzer.*".format(btag))
    os.system("sed -i 's/REGION/{0}/g' analyzer.*".format('CR'))
    os.system("sed -i 's/SYSTEMATICHERE/{0}/g' analyzer*.*".format(syst))

    os.system("rm plots.root")
    os.system("root -l -q -b runme.C")
    os.system("mv plots.root {0}/plots_{1}/{2}_A.root".format(dir, channel, fileB))


    os.system("cd {0}".format(dir))
    os.system("rm -fr dir_{0}_{1}_{2}".format(file, channel, syst))

