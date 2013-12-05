#!/usr/bin/perl -w
system "rm -rf digraph/ && mkdir digraph" and die $?;

my $header = <<EOF;
	node [fontname=cantarell]
EOF
my $code;
my $name;
my $id = 0;

while (<>) {
	if (defined $code) {
		if (/^$/) {
			$code .= "}\n";
			open my $f, "|dot -Tpng -odigraph/$name.png";
			print $f $code;
			close $f;
			print "\n.. image:: digraph/$name.png\n";
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
