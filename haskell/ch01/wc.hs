main = interact wc
	where wc input = show (length (lines input)) ++ "\n"

