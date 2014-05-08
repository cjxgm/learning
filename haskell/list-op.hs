-- pseudo code just for reference

---- simple ----
xs = head  :  tail
xs = init ++ [last]

null [] = True
null _  = False

length [] = 0
length (x:xs) = 1 + (length xs)

---- more ----
++ [] bs = bs
++ as bs = (init as) ++ ((last as) : bs)

concat [] = []
concat (x:xs) = x ++ (concat xs)

reverse [] = []
reverse (x:xs) = (reverse xs) ++ [x]

and [] = True
and (x:xs) = x && (and xs)

or [] = False
or (x:xs) = x || (and xs)

all f xs = and (map f xs)

any f xs = or (map f xs)

---- sublists ----
take _ [] = []
take 0 xs = []
take n (x:xs) = x : (take (n-1) xs)

drop n (x:xs) | n>0 = drop (n-1) xs
drop _ xs = xs

splitAt n xs = (take n xs, drop n xs)

takeWhile f (x:xs) | f x = x : (takeWhile f xs)
takeWhile _ _ = []

dropWhile f (x:xs) | f x = dropWhile f xs
dropWhile _ xs = xs

span f xs = (takeWhile f xs, dropWhile f xs)

break f xs = (takeWhile notf xs, dropWhile notf xs)
	where notf x = not (f x)

---- search ----
_ `elem` [] = False
n `elem` (x:xs)
	| x==n		= True
	| otherwise	= n `elem` xs

n `notElem` xs = not (n `elem` xs)

filter _ [] = []
filter f (x:xs)
	| f x		= x : (filter f xs)
	| otherwise	= filter f xs

-- +Data.List
(a:as) `isPrefixOf` (b:bs) | a==b = as `isPrefixOf` bs
[] `isPrefixOf` _ = True
_ `isPrefixOf` _ = False

-- +Data.List
as `isInfixOf` bs | as `isPrefixOf` bs = True
as `isInfixOf` (_:bs) | as `isInfixOf` bs = True
_ `isInfixOf` _ = False

-- +Data.List
as `isSuffixOf` bs = as == drop ((length bs) - (length as)) bs

---- several lists ----
zipWith f (a:as) (b:bs) = (f a b) : (zipWith f as bs)
zipWith _ _ _ = []

zip as bs = zipWith pack as bs
	where pack a b = (a, b)

---- string ----
-- NOT IN HASKELL LIBS
breakString _ "" = []
breakString f str = prefix : (breakString f (dropWhile f postfix))
	where (prefix, postfix) = break f str

-- NOT IN HASKELL LIBS
unbreakString ss (x:xs) = x ++ ss ++ (unbreakString xs)
unbreakString _ [] = ""

-- linux only
lines str = breakString eol str
	where eol x = x=='\n'

-- linux only
unlines strs = unbreakString "\n" strs

-- linux only
words str = breakString blank str
	where blank x = x=='\n' || x==' ' || x=='\t'

-- linux only
unwords strs = unbreakString " " strs

