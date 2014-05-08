data Tree a =
	  Empty
	| Node a (Tree a) (Tree a)

height Empty = 0
height (Node _ Empty Empty) = 1
height (Node _ l     r    ) = 1 + (max (height l) (height r))

