data BinTree a =
	  Node a (BinTree a) (BinTree a)
	| Leaf a
	| Empty
	deriving (Show)

inwalk :: BinTree a -> Maybe x
inwalk (Node x l r) =
	inwalk l
	print x
	inwalk r
	Nothing
inwalk (Leaf x) =
	print x
	Nothing
inwalk (Empty) = Nothing

