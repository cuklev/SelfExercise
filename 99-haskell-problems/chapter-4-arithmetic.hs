-- problem 31
isPrime :: Integral a => a -> Bool
isPrime n = and $ map (\d -> n `mod` d /= 0) $ takeWhile (\x -> x * x <= n) [2..]

-- problem 32
--myGCD :: Integral a => a -> a -> a
myGCD a 0 | a < 0     = -a
          | otherwise = a
myGCD a b = myGCD b (a `mod` b)

-- problem 33
--coprime :: Integral a => a -> a -> Bool
coprime x = (==1) . myGCD x

-- problem 34
totient :: Int -> Int
totient 1 = 1
totient n = myLength $ [() | x <- [1..n-1], coprime x n]

-- problem 35
primeFactors :: Integral a => a -> [a]
primeFactors n = primeFactors' n 2
    where primeFactors' n p | p * p > n      = [n]
          primeFactors' n 2 | even n         = 2 : primeFactors' (n `div` 2) 2
                            | otherwise      = primeFactors' n 3
          primeFactors' n p | n `mod` p == 0 = p : primeFactors' (n `div` p) p
                            | otherwise      = primeFactors' n $ p + 2

-- TODO: problem 36 - 41
