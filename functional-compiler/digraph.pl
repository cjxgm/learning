#!/usr/bin/perl -w
system "rm -rf digraph/ && mkdir digraph" and die $?;

my $header = <<EOF;
	node [fontname=monospace]
EOF
my $format = "svg";

my $code;
my $name;
my $id = 0;

while (<>) {
	if (defined $code) {
		if (/^$/) {
			$code .= "}\n";
			open my $f, "|dot -T$format -odigraph/$name.$format";
			print $f $code;
			close $f;
			print "\n.. image:: digraph/$name.$format\n";
			$code = undef;
		}
		else { $code .= $_ }
	} else {
		if (/^\s*\.\. digraph::\s*([^\s]*)\s*$/) {
			$name = $1 || "digraph-@{[$id++]}";
			$code = "digraph\n{\n$header";
		}
		else { print }
	}
}
