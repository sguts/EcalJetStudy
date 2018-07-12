#!/usr/bin/python2

import sys
print sys.version

#eval `scramv1 runtime -sh`
#Needs original file, location of outputfiles, number of events per file, number of files

if sys.argv[1].isdigit:
    JobNumber = sys.argv[1]
if len(sys.argv) < 1:
    sys.exit("Not enough arguments")


print 'test 2.1'
from subprocess import call
print 'test 3'
call(["splitterfinal.sh", "JetGenerator_QCD_P8.py", JobNumber ])
print 'test'
