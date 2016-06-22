module AvlMap
( fromList
, toList
, insert
--, change
, erase
, hasKey
, indexOf
, atIndex
, sizeOf
, heightOf
) where

import qualified Avl

data KeyValue k v = KeyValue k v

toPair :: KeyValue k v -> (k, v)
toPair (KeyValue k v) = (k, v)

instance (Eq k) => Eq (KeyValue k v) where
    (KeyValue k1 _) == (KeyValue k2 _) = k1 == k2

instance (Eq k, Ord k) => Ord (KeyValue k v) where
    compare (KeyValue k1 _) (KeyValue k2 _) = compare k1 k2

type Map k v = Avl.Tree (KeyValue k v)


sizeOf :: Map k v -> Int
sizeOf = Avl.sizeOf

heightOf :: Map k v -> Int
heightOf = Avl.heightOf

toList :: (Ord k) => Map k v -> [(k, v)]
toList = map toPair . Avl.toList

fromList :: (Ord k) => [(k, v)] -> Map k v
fromList = foldl (\m (k, v) -> insert m k v) Avl.Nil

insert :: (Ord k) => Map k v -> k -> v -> Map k v
insert m k v = Avl.insert m $ KeyValue k v

--change :: (Ord k) => Map k v -> k -> v -> Map k v

erase :: (Ord k) => Map k v -> k -> Map k v
erase m k = Avl.erase m $ KeyValue k undefined

hasKey :: (Ord k) => Map k v -> k -> Bool
hasKey m k = m `Avl.contains` KeyValue k undefined

indexOf :: (Ord k) => Map k v -> k -> Int
indexOf m k = m `Avl.indexOf` KeyValue k undefined

atIndex :: (Ord k) => Map k v -> Int -> (k, v)
m `atIndex` ind = toPair $ m `Avl.atIndex` ind
