#!/bin/sh
#
#(make sure the right shell will be used)
#$ -S /bin/sh
#
#(the cpu time for this job)
#$ -l h_cpu=1:29:00
#
#(the maximum memory usage of this job)
#$ -l h_vmem=5000M
#
#(use hh site)
#$ -l site=hh
#(stderr and stdout are merged together to stdout)
#$ -j y
#
# use SL5
#$ -l os=sld6
#
# use current dir and current environment
#$ -cwd
#$ -V
#

#systematics="Nominal JetEnUp JetEnDown TauEnUp TauEnDown ElEnUp ElEnDown MuEnUp MuEnDown UnclEnUp UnclEnDown"
#systematics="Nominal JetEnUp JetEnDown TopPtUp TopPtDown ZPtUp ZPtDown TauEnUp TauEnDown ElEnUp ElEnDown MuEnUp MuEnDown UnclEnUp UnclEnDown genMET ScalesDown ScalesUp PDFUp PDFDown BTagUp BTagDown METRecoilUp METRecoilDown TFRJetEnUp TFRJetEnDown TFRMuEnUp TFRMuEnDown TFRTauEnUp TFRTauEnDown"
systematics="Nominal JetEnUp JetEnDown TopPtUp TopPtDown ZPtUp ZPtDown TauEnUp TauEnDown ElEnUp ElEnDown MuEnUp MuEnDown UnclEnUp UnclEnDown genMET ScalesDown ScalesUp PDFUp PDFDown BTagUp BTagDown METRecoilUp METRecoilDown"
#systematics="Nominal"

cd /nfs/dust/cms/user/dydukhle/CMSSW_9_4_0_patch1/src/DesyTauAnalyses/NTupleMaker/test;eval `scramv1 runtime -sh` ;
rm jobss

channel=$2

	if [[  -z "$2" ]] ;then

		echo you must provide a channel....
		return 1
	fi

while read line
do

if [[  -z "$3" || $3 == "Nominal" ]] ;then
systematics="Nominal"
fi

if [[  $3 == "list" ]] ;then
systematics="list"
systematics="Nominal JetEnUp JetEnDown TopPtUp TopPtDown ZPtUp ZPtDown TauEnUp TauEnDown ElEnUp ElEnDown MuEnUp MuEnDown UnclEnUp UnclEnDown ScalesDown ScalesUp PDFUp PDFDown BTagUp BTagDown METRecoilUp METRecoilDown"
fi

if [[  $3 == "listData" ]] ;then
systematics="listData"
fi

if [[  $3 == "listSignal" ]] ;then
#systematics="Nominal JetEnUp JetEnDown TauEnUp TauEnDown ElEnUp ElEnDown MuEnUp MuEnDown UnclEnUp UnclEnDown BTagUp BTagDown"
systematics="listSignal"
fi

if [[  $3 == "listmuel" ]] ;then
systematics="listmuel"
fi

if [[  $3 == "listTFR" ]] ;then
systematics="TFRJetEnUp TFRJetEnDown TFRMuEnUp TFRMuEnDown TFRTauEnUp TFRTauEnDown"
fi



if [[  $3 == "listTT" ]] ;then
systematics="Nominal JetEnUp JetEnDown TopPtUp TopPtDown ZPtUp ZPtDown ElEnUp ElEnDown MuEnUp MuEnDown UnclEnUp UnclEnDown ScalesDown ScalesUp PDFUp PDFDown BTagUp BTagDown METRecoilUp METRecoilDown BTagUp BTagDown"
fi

if [[  $3 == "listDY" ]] ;then
systematics="Nominal JetEnUp JetEnDown ZPtUp ZPtDown MuEnUp MuEnDown UnclEnUp UnclEnDown ScalesDown ScalesUp PDFUp PDFDown METRecoilUp METRecoilDown"
fi



if [[  $3 == "listWJ" ]] ;then
systematics="Nominal JetEnUp JetEnDown MuEnUp MuEnDown UnclEnUp UnclEnDown ScalesDown ScalesUp PDFUp PDFDown METRecoilUp METRecoilDown BTagUp BTagDown ZPtUp ZPtDown TopPtUp TopPtDown"
#systematics="TopPtUp TopPtDown ZPtUp ZPtDown"
#systematics="JetEnUp JetEnDown MuEnUp MuEnDown UnclEnUp UnclEnDown BTagUp BTagDown"
#systematics="BTagDown BTagUp"
fi

if [[  $3 == "top" ]] ;then
systematics="TopPtUp TopPtDown"
fi

if [[  $3 == "Tau" ]] ;then
systematics="TauEnUp TauEnDown"
fi
if [[  $3 == "Scales" ]] ;then
systematics="ScalesUp ScalesDown"
fi
if [[  $3 == "PDF" ]] ;then
systematics="PDFUp PDFDown"
fi
if [[  $3 == "El" ]] ;then
systematics="ElEnUp ElEnDown"
systematics="ElEnUp"
fi
if [[  $3 == "Mu" ]] ;then
systematics="MuEnUp MuEnDown"
fi

if [[  $3 == "Jet" ]] ;then
systematics="JetEnUp JetEnDown"
fi

if [[  $3 == "new" ]] ;then
systematics="PDFUp PDFDown ScalesUp ScalesDown BTagUp BTagDown"
systematics="PDFUp PDFDown ScalesUp ScalesDown"
fi

if [[  $3 == "MET" ]] ;then
systematics="METRecoilUp METRecoilDown"
fi

lt=`echo $line`
_lt=`echo $line | cut -d '/' -f2`

	echo $lt > list_$_lt
	
	for syst in $systematics
	do
	
		ltt=`echo $_lt | awk -F "_B_OS.root" '{print $1}'`
		#if [[ ! -f plots_${2}/${ltt}_${syst}_B.root ]] ; then

		if [[ true ]] ; then


		echo  plots for channel $2 and syst $syst and $ltt plots_${2} ${ltt}_${syst}_B.root
	 	#qsub -N p$2 -l h_rt=2:30:00 -l h_cpu=8000M run_plots_newDataDriven.sh list_$lt $2 $syst 
	 	#qsub -js 15 -l h_cpu=20:29:00 -l h_vmem=8000M -l h_rt=20:29:00 -N p$2  run_plots_newDataDriven.sh list_$lt $2 $syst 
	 	#qsub -js 15 -l h_cpu=20:29:00 -l h_vmem=8000M -l h_rt=20:29:00 -N p$2  run_plots_newTF.sh list_$lt $2 $syst 
#	 	qsub -js 15 -l h_cpu=20:29:00 -l h_vmem=8000M -l h_rt=20:29:00 -N p$2  run_plots_newWW.sh list_$lt $2 $syst 
		cat bss > job_MuTau_${channel}_${_lt}_$syst
#		echo ./run_plots_newDataDriven.sh list_$lt $2 $syst >> job_${channel}_${lt}_$syst
		echo python /nfs/dust/cms/user/dydukhle/CMSSW_9_4_0_patch1/src/DesyTauAnalyses/NTupleMaker/test/run_plots_newDataDriven_new_MuTau.py list_$_lt $2 $syst >> job_MuTau_${channel}_${_lt}_$syst
#		echo ./run_plots_newTF.sh list_$lt $2 $syst >> job_${channel}_${lt}_$syst

		chmod 777 job_MuTau_${channel}_${_lt}_$syst
		echo job_MuTau_${channel}_${_lt}_$syst
		echo MuTau_${channel}_${_lt}_$syst
		./HTC_submit.sh job_MuTau_${channel}_${_lt}_$syst MuTau_${channel}_${_lt}_$syst


	fi
	done
done<$1

