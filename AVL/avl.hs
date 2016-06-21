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

-- Tree operations
push :: (Ord a) => Tree a -> a -> Tree a
push Nil x = single x
push tree x
    | x < value tree = balanceL $ tree `updateL` push (left tree) x
    | otherwise      = balanceR $ tree `updateR` push (right tree) x

contains :: (Ord a) => Tree a -> a -> Bool
Nil `contains` _ = False
tree `contains` x
    | x < value tree = left tree `contains` x
    | x > value tree = right tree `contains` x
    | otherwise      = True

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
