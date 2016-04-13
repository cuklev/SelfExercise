-- TODO: remove repeating code?

main = getLine >>= print . evaluate

evaluate = (\(x, "") -> x) . evalSum

evalSum str = foldSum $ evalProd str
evalProd str = foldProd $ evalPow str
evalPow str = foldPow $ evalBracket str

evalBracket ('(':xs) = let (value, (')':str)) = evalSum xs
                       in (value, str)
evalBracket x = evalNumber x

evalNumber str = reads str !! 0

foldSum (left, ('+':xs)) = foldSum $ apply (+) left xs evalProd
foldSum (left, ('-':xs)) = foldSum $ apply (-) left xs evalProd
foldSum x = x

foldProd (left, ('*':xs)) = foldProd $ apply (*) left xs evalPow
foldProd (left, ('/':xs)) = foldProd $ apply (/) left xs evalPow
foldProd x = x

foldPow (left, ('^':xs)) = apply (**) left xs $ foldPow . evalBracket
foldPow x = x

apply f left xs op = let (right, str) = op xs
                     in (f left right, str)
