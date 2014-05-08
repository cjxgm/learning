lns "" = []
lns xs = pre : (lns (dropWhile eol post)) where
	(pre, post) = break eol xs
	eol x = x=='\n' || x=='\r'
mfoldr step initial (x:xs) = step x (mfoldr step initial xs)
mfoldr _ initial _ = initial
