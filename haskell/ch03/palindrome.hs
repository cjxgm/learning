-- generate palindrome
palindrome xs = xs ++ (reverse xs)

-- check palindrome
is_palindrome [] = True
is_palindrome [x] = True
is_palindrome xs
	| head_eq_last xs	= is_palindrome (middle xs)
	| otherwise			= False
	where
		middle xs = init (tail xs)
		head_eq_last xs = (head xs) == (last xs)

