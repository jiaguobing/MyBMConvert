#!/usr/bin/perl -w --

use strict;
use Getopt::Std;
 
getopts("a:");

my $foo = '';
if ( defined $Getopt::Std::opt_a && $Getopt::Std::opt_a =~ /64/ ) {
    $foo = '64';
}

#
# run swig
#
my $hversion = &get_version();

chdir ("java_stream");

system "swig -v -java -c++ -package com.techsoft.hoops$foo hjstream.i";

exit;


sub get_version {

    my $major_version; undef $major_version;
    my $sub_version; undef $sub_version;
    my $line;

    open (VERSION, "../../../global.mk") or die "can't open global.mk\n";
    while ($line = <VERSION>) {
    	if ($line =~ /^VERSION=(.*$)/) { $major_version = $1; next; }
    	if ($line =~ /^SUBVERSION=(.*$)/) { $sub_version = $1; next; }
    	if ( (defined ($major_version)) && (defined ($sub_version)) ) { last;}
    }
    close (VERSION);

    my $hoops_version = "$major_version$sub_version";

    return $hoops_version;
}


