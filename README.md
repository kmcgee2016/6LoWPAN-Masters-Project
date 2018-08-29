# 6LoWPAN-Masters-Project
This repository contains all relevant software developed for comparing 6LoWPAN forwarding approaches and a final 6LoWPAN:CoAP application 

The simulations were performed in NS3 and all modified network stack files can be seen in the "/Simulation-Development/lr-wpan" directory.
All relevant modifications will have a comment at the start, in the form of "//KMG". All other files found under "/Simulation-Development"
are application codes, used for the various tests.

The RIOT-OS modifications made in this project are mainly related to the CLI interface and subsequent UART output. Therefore the files 
included in this repository are only those related to the "RIOT-OS/RIOT/sys/shell/commands" directory, found in the RIOT-OS Git repository. The OpenThread modifications are also only related to CLI operations and are fully contained in the files; "cli.cpp" and "cli.hpp", which are modified versions of those found in the OpenThread git repository at the location "openthread/src/examples/cli".
The spectrum scanner application found in the "/Hardware-Development/Other Applications/updated-spectrum-scanner" folder is an updated
version of that found in the "RIOT-OS/applications/spectrum-scanner" directory. It has bee nmodified so that it compiles with the latest version of the RIOT-OS, as significant stack changes have been made since this application's development(No longer uses PIDs as a means of grabbing interfaces). 

The final application code contains a modified version of the nanocoap_server application found in the "RIOT-OS/RIOT/examples/nanocoap_server" directory of the RIOT-OS git repository. The radvd.conf file used in the border router E. Linux device is included in the "/Final-Application/Border-Router" directory. The actual border router used in this application is the one provided in the "RIOT-OS/RIOT/examples/gnrc_border_router" directory of the RIOT-OS Git repository. The ThingWorx files are merely for proof of 
