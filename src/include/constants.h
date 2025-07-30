
#ifndef CONSTANTS
#define CONSTANTS

const char *help = "Usage: NetworkList [OPTION] INPUT_DATA\n"
"Information:\n"
"   -h      prints this menu\n"
"   -v      prints version of program\n"
"Action:\n"
"   -a      adds given Network Address\n"
"           Needs -i and -m\n"
"   -r      removes given Network Address\n"
"           Needs -i and -m\n"
"   -s      searches known networks for host\n"
"           Requires -i, will ignore -m\n"
"Defines:\n"
"   -i      defines IP address\n"
"   -m      defines mask of given network\n"
"Returns:\n"
"   0 if OK\n"
"   1 if Error in options occures\n"
"   2 if given IP address or Mask is incorrect\n"
"   3 if given IP isn't network address, but a host, which cannot be added to table\n"
"   4 if file Error occures\n"
"Examples:\n"
"NetworkList -a -i 192.168.0.0 -m 24 <-- adds network 192.168.0.0/24\n";

const char *version = "NetworkList Version 0.1.0\n";


#endif
