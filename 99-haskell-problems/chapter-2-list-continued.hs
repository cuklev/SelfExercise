-- problem 11
data RunLength a = Single a | Multiple Int a deriving Show

encodeModified :: Eq a => [a] -> [RunLength a]
encodeModified [] = []
encodeModified [x] = [Single x]
encodeModified (x:y:ys) | x == y    = (\(x:xs) -> (oneMore x : xs)) $ encodeModified (y:ys)
                        | otherwise = Single x : encodeModified (y:ys)
                            where oneMore (Single x) = Multiple 2 x
                                  oneMore (Multiple c x) = Multiple (c + 1) x

-- problem 12
decodeModified :: [RunLength a] -> [a]
decodeModified [] = []
decodeModified (x:xs) = decodeRunLength x ++ decodeModified xs
    where decodeRunLength (Single x) = [x]
          decodeRunLength (Multiple 0 _) = []
          decodeRunLength (Multiple c x) = x : decodeRunLength (Multiple (c - 1) x)

-- problem 13
encodeDirect :: Eq a => [a] -> [RunLength a]
encodeDirect = encodeModified

-- problem 14
dupli :: [a] -> [a]
dupli [] = []
dupli (x:xs) = x : x : dupli xs

-- problem 15
repli :: [a] -> Int -> [a]
repli [] _ = []
repli (x:xs) c = repli' x c ++ repli xs c
    where repli' _ 0 = []
          repli' x c = x : repli' x (c - 1)

-- problem 16
dropEvery :: [a] -> Int -> [a]
dropEvery x c = dropEvery' x c c
    where dropEvery' [] _ _ = []
          dropEvery' (x:xs) c 1 = dropEvery' xs c c
          dropEvery' (x:xs) c i = x : dropEvery' xs c (i - 1)

-- problem 17
split :: [a] -> Int -> ([a], [a])
split x 0 = ([], x)
split (x:xs) c = (\(xs, ys) -> ((x:xs), ys)) $ split xs $ c - 1

-- problem 18
slice :: [a] -> Int -> Int -> [a]
slice x 1 c = fst $ split x c
slice (_:xs) s c = slice xs (s - 1) (c - 1)

-- problem 19
rotate :: [a] -> Int -> [a]
rotate x 0 = x
rotate x c = (\(a, b) -> b ++ a) $ split x $ rc
    where len = length x
          mc = c `mod` len
          rc = if mc < 0 then mc + len else mc

-- problem 20
removeAt :: Int -> [a] -> (a, [a])
removeAt 1 (x:xs) = (x, xs)
removeAt c (x:xs) = (\(r, xs) -> (r, (x:xs))) $ removeAt (c - 1) xs
