-- my own drop and take

-- drop the first n elements
my_drop n xs =
	if n<=0 || null xs
	then xs
	else my_drop (n-1) (tail xs)

-- take the first n elements
my_take n xs =
	if n<=0 || null xs
	then []
	else (head xs) : (my_take (n-1) (tail xs))

