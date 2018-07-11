#!/usr/bin/perl -w --

use strict;
use Getopt::Std;
 
#
# run swig
#
my $hversion = &get_version();
my $dll = "hcsp_bridge$hversion";
chdir ("csharp_hp_bridge");
system "swig -v -csharp -c++ -dllimport $dll hcsp_bridge.i";

#
# post-process for long types
#
my $junk;
my @csfiles;

opendir (DH,".");
($junk, $junk, @csfiles) = readdir (DH);
closedir(DH);

my $file;
foreach $file (@csfiles)
{
	next unless ($file =~ /.*?\.cs/);
	next if ($file =~ /HCSP\.cs/);
	next if ($file =~ /HCSPPINVOKE\.cs/);
	next if ($file =~ /hcspinfo\.cs/);
	next if ($file =~ /SWIGTYPE/);
	next if (-d "$file");

	open (FH, "<$file"); 
	open (NEW, ">$file\.tmp"); 

	my $line;
	while ($line = <FH>)
	{
		if ($line =~ /^using\s*System\.Runtime\.InteropServices;$/)
		{
			$line = $line."\n#if _M_X64\n".
					"using HLONG = System.Int64;\n".
					"#else\n".
					"using HLONG = System.Int32;\n".
					"#endif\n\n";
		}
		print NEW $line;
	}

	close(NEW);
	print "$file\n";
	close(FH);

	unlink("$file");
	rename("${file}.tmp", "$file");
}

#
# post-process for proper DEBUG linkage
#
my $line;
open (FH, "<HCSPPINVOKE.cs");
open (FH_NEW, ">HCSPPINVOKE.cs.new");

while ($line = <FH>)
{
	$line =~ s/\"$dll\"/HCSPDLL/g;

	if ($line =~ /^unsafe\s+?public\s+?class\s+?HCSPPINVOKE/)
	{
		print FH_NEW "\n";
		print FH_NEW $line;
		print FH_NEW <<EOF;

#if DEBUG
#    if _M_X64
internal const string HCSPDLL = "${dll}_x64d";
#    else
internal const string HCSPDLL = "${dll}d";
#    endif
#else
#    if _M_X64
internal const string HCSPDLL = "${dll}_x64";
#    else
internal const string HCSPDLL = "$dll";
#    endif
#endif

EOF
	} else {

		print FH_NEW $line;
	}
}

close (FH);
close (FH_NEW);

unlink("HCSPPINVOKE.cs");
rename("HCSPPINVOKE.cs.new", "HCSPPINVOKE.cs");

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


