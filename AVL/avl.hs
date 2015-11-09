data Tree = Nil | Node Tree Tree Int

add :: Tree -> Int -> Tree
add Nil value = Node Nil Nil value
add (Node left right x) value
            | value < x = rebalance $ Node (add left value) right x
            | otherwise = rebalance $ Node left (add right value) x

contains :: Tree -> Int -> Bool
contains Nil _ = False
contains (Node left right x) value
            | value < x = contains left value
            | value > x = contains right value
            | otherwise = True

each :: Tree -> [Int]
each Nil = []
each (Node left right value) = (each left) ++ (value : each right)





sizeof :: Tree -> Int
sizeof Nil = 0
sizeof (Node left right _) = sizeof left + sizeof right + 1

heightof :: Tree -> Int
heightof Nil = 0
heightof (Node left right _) = max (heightof left) (heightof right) + 1

rebalance :: Tree -> Tree
rebalance Nil = Nil
rebalance node@(Node left right value)
            | balance node < -1 = rotateLeft (if balance left > 0 then (Node left (rotateRight right) value) else node)
            | balance node >  1 = rotateRight (if balance right < 0 then (Node (rotateLeft left) right value) else node)
            | otherwise = node

balance :: Tree -> Int
balance Nil = 0
balance (Node left right _) = heightof left - heightof right

rotateLeft :: Tree -> Tree
rotateLeft Nil = Nil
rotateLeft (Node left (Node rl rr rv) value) = Node (Node left rl value) rr rv

rotateRight :: Tree -> Tree
rotateRight Nil = Nil
rotateRight (Node (Node ll lr lv) right value) = Node ll (Node lr right value) lv


main = do
    let tree = foldl add Nil [1..10000]
    putStrLn $ "Size of tree: " ++ show (sizeof tree)
    putStrLn $ "Height of tree: " ++ show (heightof tree)
    --print $ each tree
