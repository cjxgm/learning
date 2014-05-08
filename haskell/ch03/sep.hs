
sep _ [x] = x
sep c (x:xs) = x ++ [c] ++ (sep c xs)
--sep c (x:xs) = [x] ++ [[s]] ++ (sep s xs)

