import os
cfg='GenAnalyzer/python/configHToEleEle_cfg.py'
#inputFiles_='file:/afs/cern.ch/user/c/ccrovell/cernbox/run3_massregression/CMSSW_13_0_13/src/MCProduction/E2E-AToEleEle/GEN_AToEleEle_m00p1To1p2_pythia8.root'
inputFiles_ = open("list_gen_files_small.txt","r").readlines()
maxEvents_=-1
skipEvents_=0#
outputFile_='GenInfo_HToEleEle_0p01GeV_to_1p2GeV.root'
cmd="cmsRun %s inputFiles=%s maxEvents=%d skipEvents=%d outputFile=%s"%(cfg,inputFiles_,maxEvents_,skipEvents_,outputFile_)
print(cmd)
os.system(cmd)
