-- problem 1
myLast :: [a] -> a
myLast [x] = x
myLast (x:xs) = myLast xs

-- problem 2
myButLast :: [a] -> a
myButLast [x, _] = x
myButLast (x:y:ys) = myButLast (y:ys)

-- problem 3
elementAt :: [a] -> Int -> a
elementAt (x:_) 0 = x
elementAt (x:xs) index = elementAt xs $ index - 1

-- problem 4
myLength :: [a] -> Int
myLength [] = 0
myLength (_:xs) = 1 + myLength xs

-- problem 5
myReverse :: [a] -> [a]
myReverse x = myReverse' x []
    where myReverse' [] ys = ys
          myReverse' (x:xs) ys = myReverse' xs (x:ys)

-- problem 6
isPalindrome :: Eq a => [a] -> Bool
isPalindrome x = x == myReverse x

-- problem 7
data NestedList a = Elem a | List [NestedList a]

flatten :: NestedList a -> [a]
flatten (Elem x) = [x]
flatten (List []) = []
flatten (List (Elem x:xs)) = x : flatten (List xs)
flatten (List (x:xs)) = flatten x ++ flatten (List xs)

flatten' :: NestedList a -> [a]
flatten' (Elem x) = [x]
flatten' (List xs) = xs >>= flatten'

-- problem 8
compress :: Eq a => [a] -> [a]
compress [] = []
compress [x] = [x]
compress (x:y:ys) | x == y    = compress (y:ys)
                  | otherwise = x : compress (y:ys)

-- problem 9
pack :: Eq a => [a] -> [[a]]
pack [] = []
pack [x] = [[x]]
pack (x:y:ys) | x == y    = (\(g:gs) -> (x:g):gs) $ pack (y:ys)
              | otherwise = [x] : pack (y:ys)

-- problem 10
encode :: Eq a => [a] -> [(Int, a)]
encode [] = []
encode [x] = [(1, x)]
encode (x:y:ys) | x == y    = (\((f, s):xs) -> ((f + 1, s):xs)) $ encode (y:ys)
                | otherwise = (1, x) : encode (y:ys)
