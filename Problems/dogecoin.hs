import Data.Array.IO
import qualified Data.ByteString.Lazy as B
import qualified Data.ByteString.Lazy.Char8 as C8
import Data.Int (Int32)

main = do
    input <- B.getContents
    let (n:m:k:rest) = map parseNumber $ C8.words input
        coins = groupCoords rest
    dp <- newArray ((0, 0), (n-1, m-1)) 0 :: IO (IOUArray (Int32, Int32) Int32)
    mapM_ (\ind -> addIOArray dp ind 1) coins
    mapM_ (\j -> readArray dp (0, j-1) >>= addIOArray dp (0, j)) [1..m-1]
    mapM_ (dpRow dp m) [1..n-1]
    result <- readArray dp (n-1, m-1)
    print result

groupCoords [] = []
groupCoords (x:y:xs) = (x, y) : groupCoords xs

addIOArray arr ind x = do
    v <- readArray arr ind
    writeArray arr ind (v + x)

dpRow arr m i = do
    x <- readArray arr (i - 1, 0)
    addIOArray arr (i, 0) x
    mapM_ (\j -> dpCell arr (i, j)) [1..m-1]

dpCell arr ind@(i, j) = do
    x <- readArray arr (i - 1, j)
    y <- readArray arr (i, j - 1)
    addIOArray arr ind $ max x y

parseNumber :: B.ByteString -> Int32
parseNumber = parse 0
  where parse n str
          | B.null str = n
          | otherwise  = let d = fromIntegral (B.head str - 48)
                         in parse (n * 10 + d) $ B.tail str
