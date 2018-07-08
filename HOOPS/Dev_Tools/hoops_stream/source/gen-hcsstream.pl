#!/usr/bin/perl -w --

use strict;
use Getopt::Std;
 
#
# run swig
#
my $hversion = &get_version();
my $dll = "hcsstream$hversion";
chdir ("csharp_stream");
system "swig -v -csharp -c++ -dllimport $dll hcsstream.i";

#
# post-process for proper DEBUG linkage
#
my $line;
open (FH, "<HCSSTREAMPINVOKE.cs");
open (FH_NEW, ">HCSSTREAMPINVOKE.cs.new");

while ($line = <FH>)
{
	$line =~ s/\"$dll\"/HCSSTREAMDLL/g;

	if ($line =~ /^public\s+?class\s+?HCSSTREAMPINVOKE/)
	{
		print FH_NEW "\n";
		print FH_NEW $line;
		print FH_NEW <<EOF;

#if DEBUG
#    if _M_X64
const string HCSSTREAMDLL = "$dll\_x64d";
#    else
const string HCSSTREAMDLL = "${dll}d";
#    endif
#else
#    if _M_X64
const string HCSSTREAMDLL = "$dll\_x64";
#    else
const string HCSSTREAMDLL = "$dll";
#    endif
#endif

EOF
	} else {

		print FH_NEW $line;
	}
}

close (FH);
close (FH_NEW);

unlink("HCSSTREAMPINVOKE.cs");
rename("HCSSTREAMPINVOKE.cs.new", "HCSSTREAMPINVOKE.cs");

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


