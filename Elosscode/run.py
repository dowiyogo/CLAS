#!/usr/bin/env python                                                                                                                                                  
import os
###PARAMETERS ########################
Xf_limit = 0.1 # Xf cut
step_E = 1 # 1 MeV step in energy scan.
nu_up = 4.2 # Maximum NU                                                                                                                                              
nu_do = 3.2 # Minimum NU 
nbins_nu = 10 #Number of bins in Nu                                                                                                                                    
 ######################################


#########################################################
nbins_E = int(100/step_E) # Number of bins in the scan.
xbins_nu= np.arange(nu_do,nu_up, (nu_up-nu_do)/nbins_nu) #nu bins

for it in range(0,len(xbins_nu)-1):
    nu_min = xbins_nu[it]
    nu_max = xbins_nu[it+1]
    arg = str(it)+ ' ' + str(nu_min) + ' ' + str(nu_max) + ' ' +  str(Xf_limit) + ' ' + str(nbins_E) + ' ' + str(step_E)
    os.system('./newAnalysis Pb '+arg)
    os.system('./newAnalysis Fe '+arg)
    os.system('./newAnalysis C '+arg)

##########################################################
