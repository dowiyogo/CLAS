#!/usr/bin/env python
import os
from sys import argv
import subprocess
from time import sleep

TARGET_POS = [16392,17983,19497,21080,22623,24262,25898,27545]

cmd = "./app/read_write_ADC_RBPi"
proc = subprocess.Popen([cmd],shell=True,stdout=subprocess.PIPE)

curr_state = int(proc.communicate()[0].strip().split('\n')[-1].split('\t')[0])

last_dir = 1;
progdir = os.environ["PROGRAMS"]

count = 0
ofile_name  = progdir + "/DATA/ADC_Values_{0}.txt".format(count)
dtime = 0.15

while (1):
    if (os.path.exists(ofile_name)):
        if (os.path.getsize(ofile_name) > 1000000):
            count += 1
            ofile_name  = progdir + "/DATA/ADC_Values_{0}.txt".format(count)
            
    ofile = open(ofile_name,"a+")
   
    if last_dir == 1:
        TARGET_POS.reverse()
        for pos in TARGET_POS:
            cmd = progdir + "/app/read_write_ADC_RBPi"
            proc = subprocess.Popen([cmd],shell=True,stdout=subprocess.PIPE)
            out = proc.communicate()
            curr_state = int(out[0].strip().split('\n')[-1].split('\t')[0])
            while curr_state<pos:
                cmd = progdir + "/app/move_piezomotor"
                subprocess.call([cmd,"+10"])
                sleep(dtime)
                cmd = progdir + "/app/read_write_ADC_RBPi"
                proc = subprocess.Popen([cmd],shell=True,stdout=subprocess.PIPE)
                out = proc.communicate()
                curr_state = int(out[0].strip().split('\n')[-1].split('\t')[0])
                print out[0].strip().split('\n')[-1]
                iurr_state = int(out[0].strip().split('\n')[-1].split('\t')[0])
                ofile.write(out[0].strip().split('\n')[-1])
                ofile.write("\n")
                
            while curr_state>pos:
                cmd = progdir + "/app/move_piezomotor"
                subprocess.call([cmd,"-10"])
                sleep(dtime)
                cmd = progdir + "/app/read_write_ADC_RBPi"
                proc = subprocess.Popen([cmd],shell=True,stdout=subprocess.PIPE)
                out = proc.communicate()
                curr_state = int(out[0].strip().split('\n')[-1].split('\t')[0])
                print out[0].strip().split('\n')[-1]
                ofile.write(out[0].strip().split('\n')[-1])
                ofile.write("\n")
            sleep(40)
            
        last_dir = -1
        TARGET_POS.reverse()
        
    elif last_dir== -1:
        for pos in TARGET_POS:
            cmd = progdir + "/app/read_write_ADC_RBPi"
            proc = subprocess.Popen([cmd],shell=True,stdout=subprocess.PIPE)
            out = proc.communicate()
            curr_state = int(out[0].strip().split('\n')[-1].split('\t')[0])
            while curr_state<pos:
                cmd = progdir + "/app/move_piezomotor"
                subprocess.call([cmd,"+10"])
                sleep(dtime)
                cmd = progdir + "/app/read_write_ADC_RBPi"
                proc = subprocess.Popen([cmd],shell=True,stdout=subprocess.PIPE)
                out = proc.communicate()
                curr_state = int(out[0].strip().split('\n')[-1].split('\t')[0])
                print out[0].strip().split('\n')[-1]
                ofile.write(out[0].strip().split('\n')[-1])
                ofile.write("\n")
            while curr_state>pos:
                cmd = progdir + "/app/move_piezomotor"
                subprocess.call([cmd,"-10"])
                sleep(dtime)
                cmd = progdir + "/app/read_write_ADC_RBPi"
                proc = subprocess.Popen([cmd],shell=True,stdout=subprocess.PIPE)
                out = proc.communicate()
                curr_state = int(out[0].strip().split('\n')[-1].split('\t')[0])
                print out[0].strip().split('\n')[-1]
                ofile.write(out[0].strip().split('\n')[-1])
                ofile.write("\n")
            sleep(40)
        last_dir = +1
    ofile.close()

