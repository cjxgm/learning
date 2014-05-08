foldl step initial (x:xs) = foldl step (step initial x) xs
foldl _ initial _ = initial

foldr step initial (x:xs) = step x (foldr step initial xs)
foldr _ initial _ = initial

filter pred xs = foldr step [] xs where
	step x xs | pred x = x:xs
	step _ xs = xs

map f xs = foldr step [] xs where
	step x xs = f x : xs

