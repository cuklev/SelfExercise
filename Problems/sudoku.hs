{-# LANGUAGE FlexibleContexts #-}
import Control.Monad (unless)
import Data.Array.IO
import Data.Char (ord, chr)

main = do
    inputs <- concat <$> mapM (\_ -> map parseDigit <$> getLine) [1..9]
    table <- newListArray ((0, 0), (8, 8)) inputs :: IO (IOUArray (Int, Int) Int)
    let allIndices = (,) <$> [0..8] <*> [0..8]
        emptyIndices = [ fst x | x <- zip allIndices inputs, snd x == 0]
    solvable <- backtrack table emptyIndices
    result <- getElems table
    putStr . unlines . splitEvery 9 $ map (\d -> chr (d + ord '0')) result

splitEvery _ [] = []
splitEvery n xs = let (row, rest) = splitAt n xs
                  in row : splitEvery n rest

parseDigit :: Char -> Int
parseDigit '-' = 0
parseDigit x   = ord x - ord '0'

backtrack _     []       = return True
backtrack table (ix:ixs) = do
    solved <- lazyIOAnd $ map tryDigit [1..9]
    unless solved $ writeArray table ix 0
    return solved

  where
    tryDigit d = do
        f1 <- not <$> checkCell d
        if f1 then do writeArray table ix d
                      backtrack table ixs
              else return False

    checkCell d = lazyIOAnd $ map (fmap (== d) . readArray table) indices

    indices = let (i, j) = ix
                  i' = i - i `mod` 3
                  j' = j - j `mod` 3
                  row = map (\k -> (i, k)) [0..8]
                  col = map (\k -> (k, j)) [0..8]
                  square = (,) <$> [i'..i'+2] <*> [j'..j'+2]
              in row ++ col ++ square

lazyIOAnd :: [IO Bool] -> IO Bool
lazyIOAnd [] = return False
lazyIOAnd (x:xs) = x >>= \v -> if v then return True
                                    else lazyIOAnd xs
