data Tree a = Nil | Node { value :: a
                         , left :: Tree a
                         , right :: Tree a
                         , size :: Int
                         , height :: Int
                         } deriving Show

-- Helpers
sizeOf :: Tree a -> Int
sizeOf Nil = 0
sizeOf tree = size tree

heightOf :: Tree a -> Int
heightOf Nil = 0
heightOf tree = height tree

balanceOf :: Tree a -> Int
balanceOf tree = heightOf (left tree) - heightOf (right tree)

update :: Tree a -> Tree a
update tree = tree
    { size   = (sizeOf $ left tree) + (sizeOf $ right tree) + 1
    , height = max (heightOf $ left tree) (heightOf $ right tree) + 1
    }

updateL :: Tree a -> Tree a -> Tree a
updateL tree leftTree = update tree { left = leftTree }

updateR :: Tree a -> Tree a -> Tree a
updateR tree rightTree = update tree { right = rightTree }

-- Basic stuff
single :: a -> Tree a
single x = Node { value = x
                , left = Nil
                , right = Nil
                , size = 1
                , height = 1
                }

toList :: Tree a -> [a]
toList Nil = []
toList tree = toList (left tree) ++ [value tree] ++ toList (right tree)

fromList :: (Ord a) => [a] -> Tree a
fromList = foldl insert Nil

-- Tree operations
insert :: (Ord a) => Tree a -> a -> Tree a
insert Nil x = single x
insert tree x
    | x < value tree = balanceL $ tree `updateL` insert (left tree) x
    | x > value tree = balanceR $ tree `updateR` insert (right tree) x
    | otherwise      = tree

erase :: (Ord a) => Tree a -> a -> Tree a
erase Nil _ = Nil
erase tree x
    | x < value tree = balanceR $ tree `updateL` erase (left tree) x
    | x > value tree = balanceL $ tree `updateR` erase (right tree) x
    | otherwise      = case right tree of
                         Nil -> left tree
                         _   -> let (rtree, inorder) = erase' $ right tree
                                in balanceL $ (inorder `updateL` left tree) `updateR` rtree
                       where erase' tree = case left tree of
                                             Nil -> (right tree, tree)
                                             _   -> let (ltree, inorder) = erase' $ left tree
                                                    in (balanceR $ tree `updateL` ltree, inorder)

contains :: (Ord a) => Tree a -> a -> Bool
Nil `contains` _ = False
tree `contains` x
    | x < value tree = left tree `contains` x
    | x > value tree = right tree `contains` x
    | otherwise      = True

indexOf :: (Ord a) => Tree a -> a -> Int
tree `indexOf` x
    | x < value tree = left tree `indexOf` x
    | x > value tree = sizeOf (left tree) + 1 + right tree `indexOf` x
    | otherwise      = sizeOf $ left tree

atIndex :: (Ord a) => Tree a -> Int -> a
tree `atIndex` ind
    | ind < currInd = left tree `atIndex` ind
    | ind > currInd = right tree `atIndex` (ind - currInd - 1)
    | otherwise     = value tree
    where currInd = sizeOf $ left tree

-- Rotations
rotateL :: Tree a -> Tree a
rotateL tree = let root = right tree
                   leftTree = tree `updateR` left root
               in root `updateL` leftTree

rotateR :: Tree a -> Tree a
rotateR tree = let root = left tree
                   rightTree = tree `updateL` right root
               in root `updateR` rightTree

-- Balance - left is higher
balanceL :: Tree a -> Tree a
balanceL tree
    | imbal     = rotateR $ if imbalL
                            then tree `updateL` (rotateL $ left tree)
                            else tree
    | otherwise = tree
    where imbal = balanceOf tree > 1
          imbalL = balanceOf (left tree) < 0

-- Balance - right is higher
balanceR :: Tree a -> Tree a
balanceR tree
    | imbal     = rotateL $ if imbalR
                            then tree `updateR` (rotateR $ right tree)
                            else tree
    | otherwise = tree
    where imbal = balanceOf tree < -1
          imbalR = balanceOf (right tree) > 0
