sub fill_stat(%)
{
	our %stat = @_;

	sub set_if_empty
	{
		my ($key, $value) = @_;
		$stat{$key} = $value if !$stat{$key};
	}
	&set_if_empty('dev', 0);
	&set_if_empty('ino', 0);
	&set_if_empty('type', 'regular');
	&set_if_empty('mode', 0777);
	&set_if_empty('nlink', 1);
	&set_if_empty('uid', 1000);
	&set_if_empty('gid', 100);
	&set_if_empty('rdev', 0);
	&set_if_empty('size', 4096);
	&set_if_empty('atime', time);
	&set_if_empty('mtime', time);
	&set_if_empty('ctime', time);
	&set_if_empty('blksize', 4096);
	&set_if_empty('blocks', 1);

	my %type_name_to_value = (
		regular => 0100,
		dir     => 0040,
		# TODO: more
	);
	$stat{type} = $type_name_to_value{$stat{type}} || 0100;

	(
		$stat{dev},
		$stat{ino},
		($stat{type}<<9) | $stat{mode},
		$stat{nlink},
		$stat{uid},
		$stat{gid},
		$stat{rdev},
		$stat{size},
		$stat{atime},
		$stat{mtime},
		$stat{ctime},
		$stat{blksize},
		$stat{blocks},
	);
}

1;

