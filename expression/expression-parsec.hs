{-# LANGUAGE FlexibleContexts #-}


surroundedWithSpaces x = do
    spaces
    r <- x
    spaces
    return r

operator = surroundedWithSpaces . char

numberLiteral = fmap read $ many1 digit

parens = between (operator '(') (operator ')')

term = parens expression
     <|> surroundedWithSpaces numberLiteral
     <?> "simple expression"

table = [ [prefix '+' id, prefix '-' negate]
        , [binary '^' (^) AssocRight]
        , [binary '*' (*) AssocLeft, binary '/' div AssocLeft]
        , [binary '+' (+) AssocLeft, binary '-' (-) AssocLeft]
        ]
        where
            binary  name f assoc = Infix (reg name f) assoc
            prefix  name = Prefix . reg name
            postfix name = Postfix . reg name
            reg name f = operator name >> return f


parseExpression = parse expression "expression"
expression = do
    r <- buildExpressionParser table term <?> "expression"
    eof
    return r

main = do
    expressions <- fmap lines getContents
    flip mapM expressions $ \x -> case parseExpression x of
            Right result -> print result
            Left err -> print err
