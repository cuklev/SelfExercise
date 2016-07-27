-- problem 21
insertAt :: a -> [a] -> Int -> [a]
insertAt n xs 1 = n : xs
insertAt n (x:xs) c = x : insertAt n xs (c - 1)

-- problem 22
range :: (Enum a, Eq a) => a -> a -> [a]
range x y | x == y    = [x]
          | otherwise = x : range (succ x) y
-- range x y = [x..y]

-- problem 23 requires random
-- problem 24 requires random
-- problem 25 requires random

-- problem 26
combinations :: Int -> [a] -> [[a]]
combinations 0 _ = []
combinations 1 xs = map return xs
combinations _ [] = []
combinations k (x:xs) = map (x:) (combinations (k - 1) xs) ++ combinations k xs

-- problem 27 maybe later
-- problem 28 some strange sort
