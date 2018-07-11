#!/usr/bin/perl -w --

use strict;
use Getopt::Std;
 
#
# run swig
#
chdir ("java_hp_bridge");
system "swig -v -java -c++ -package com.techsoft.hoops hjp_bridge.i";
exit;

