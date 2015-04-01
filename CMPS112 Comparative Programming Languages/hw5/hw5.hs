{------------------------------------------------
-- CMPS112 Winter 2015 Homework 5 --
Program: hw5.hs
Author: 
Evan Hughes [eshughes@ucsc.edu]

Partner: Hadley Black [hablack@ucsc.edu]
Time programming together: 12 hours
Individual Time: ~1 hour
-------------------------------------------------}
-- Necessary imports
import Control.Applicative ((<$>),liftA,liftA2)
import Data.Map
import Text.Parsec
import Text.Parsec.Expr
import Text.Parsec.Language (emptyDef)
import Text.Parsec.String (Parser)
import qualified Text.Parsec.Token as P
import Data.Maybe

--------- AST Nodes ---------

-- Variables are identified by their name as string
type Variable = String

-- Values are either integers or booleans
data Value = IntVal Int       -- Integer value
           | BoolVal Bool     -- Boolean value

-- Expressions are variables, literal values, unary and binary operations
data Expression = Var Variable                    -- e.g. x
                | Val Value                       -- e.g. 2
                | BinOp Op Expression Expression  -- e.g. x + 3
                | Assignment Variable Expression  -- e.g. x = 3

-- Statements are expressions, conditionals, while loops and sequences
data Statement = Expr Expression                   -- e.g. x = 23
               | If Expression Statement Statement -- if e then s1 else s2 end
               | While Expression Statement        -- while e do s end
               | Sequence Statement Statement      -- s1; s2
               | Skip                              -- no-op
               | For Variable Expression Expression Statement
-- All binary operations
data Op = Plus         --  +  :: Int -> Int -> Int
        | Minus        --  -  :: Int -> Int -> Int
        | Times        --  *  :: Int -> Int -> Int
        | GreaterThan  --  >  :: Int -> Int -> Bool
        | Equals       --  == :: Int -> Int -> Bool
        | LessThan     --  <  :: Int -> Int -> Bool

-- The `Store` is an associative map from `Variable` to `Value` representing the memory
type Store = Map Variable Value

--------- Parser ---------

-- The Lexer

lexer = P.makeTokenParser (emptyDef {
  P.identStart = letter,
  P.identLetter = alphaNum,
  P.reservedOpNames = ["+", "-", "*", "!", ">", "=", "==", "<"],
  P.reservedNames = ["true", "false", "if", "in", "then", "else", "while", "end", "to", "do", "for"]
})

-- The Parser

-- Number literals
numberParser :: Parser Value
numberParser = (IntVal . fromIntegral) <$> P.natural lexer

-- Boolean literals
boolParser :: Parser Value
boolParser =  (P.reserved lexer "true" >> return (BoolVal True))
          <|> (P.reserved lexer "false" >> return (BoolVal False))

-- Literals and Variables
valueParser :: Parser Expression
valueParser =  Val <$> (numberParser <|> boolParser)
           <|> Var <$> P.identifier lexer

-- -- Expressions
exprParser :: Parser Expression
exprParser = liftA2 Assignment
                    (try (P.identifier lexer >>= (\v ->
                          P.reservedOp lexer "=" >> return v)))
                    exprParser
          <|> buildExpressionParser table valueParser
    where table = [[Infix (op "*" (BinOp Times)) AssocLeft]
                  ,[Infix (op "+" (BinOp Plus)) AssocLeft]
                  ,[Infix (op "-" (BinOp Minus)) AssocLeft]
                  ,[Infix (op ">" (BinOp GreaterThan)) AssocLeft]
                  ,[Infix (op "==" (BinOp Equals)) AssocLeft]
                  ,[Infix (op "<" (BinOp LessThan)) AssocLeft]]
          op name node = (P.reservedOp lexer name) >> return node

-- Sequence of statements
stmtParser :: Parser Statement
stmtParser = stmtParser1 `chainl1` (P.semi lexer >> return Sequence)

-- Single statements
stmtParser1 :: Parser Statement
stmtParser1 = (Expr <$> exprParser)
          <|> do
              P.reserved lexer "if"
              cond <- exprParser
              P.reserved lexer "then"
              the <- stmtParser
              P.reserved lexer "else"
              els <- stmtParser
              P.reserved lexer "end"
              return (If cond the els)
          <|> do
              P.reserved lexer "while"
              cond <- exprParser
              P.reserved lexer "do"
              body <- stmtParser
              P.reserved lexer "end"
              return (While cond body)
           <|> do
              P.reserved lexer "for"
              var <- P.identifier lexer
              P.reserved lexer "in"
              low <- exprParser
              P.reserved lexer "to"
              up <- exprParser
              P.reserved lexer "do"
              body <- stmtParser
              P.reserved lexer "end"
              return (For var low up body)
-------- Helper functions --------

-- Lift primitive operations on IntVal and BoolVal valuest
liftIII :: (Int -> Int -> Int) -> Value -> Value -> Value
liftIII f (IntVal x) (IntVal y) = IntVal $ f x y
liftIIB :: (Int -> Int -> Bool) -> Value -> Value -> Value
liftIIB f (IntVal x) (IntVal y) = BoolVal $ f x y

-- Apply the correct primitive operator for the given Op value
applyOp :: Op -> Value -> Value -> Value
applyOp Plus        = liftIII (+)
applyOp Minus       = liftIII (-)
applyOp Times       = liftIII (*)
applyOp GreaterThan = liftIIB (>)
applyOp Equals      = liftIIB (==)
applyOp LessThan    = liftIIB (<)

-- Parse and print (pp) the given WHILE programs
pp :: String -> IO ()
pp input = case (parse stmtParser "" input) of
    Left err -> print err
    Right x  -> print x

-- Parse and run the given WHILE programs
run :: (Show v) => (Parser n) -> String -> (n -> Store -> v) -> IO ()
run parser input eval = case (parse parser "" input) of
    Left err -> print err
    Right x  -> print (eval x empty)

--  Uncomment the following function for question #5 and #6

-- Parse and run the given WHILE programs using monads
runMonad :: String -> Maybe Store
runMonad input = proc (parse stmtParser "" input)
    where proc (Right x) = snd `fmap` runImperative (evalS_monad x) empty
          proc _         = Nothing

-- Question 1 --
instance Show Value where
  show (IntVal x) = show x
  show (BoolVal b) = if b then "true" else "false"

instance Show Op where
  show Plus = "+"
  show Minus = "-"
  show Times = "*"
  show GreaterThan = ">"
  show Equals = "=="
  show LessThan = "<"

instance Show Expression where
  show (Var s) = s
  show (Val v) = show v
  show (BinOp op e1 e2) = show e1 ++ " " ++ show op ++ " " ++ show e2 
  show (Assignment var expr) = var ++ " = " ++ show expr

instance Show Statement where
  show (Expr expr) = show expr
  show (If e s1 s2) = "if " ++ show e ++ " then " ++ show s1 ++ " else " ++ show s2 ++ " end"
  show (While e s) = "while " ++ show e ++ " do " ++ show s ++ " end"
  show (Sequence s1 s2) = show s1 ++ "; " ++ show s2
  show Skip = ""
  show (For v e1 e2 s) = "for " ++ v ++ " in " ++ show e1 ++ " to " ++ show e2 ++ " do " ++ show s ++ " end"

-- Question 2 -- 
evalE :: Expression -> Store -> (Value, Store)
evalE (BinOp o a b) s = (applyOp o a' b', s'')
  where (a', s') = evalE a s
        (b', s'') = evalE b s'

evalE (Var x) s = (x', s)
  where x' = fromMaybe (error "Store not found") (Data.Map.lookup x s)

evalE (Val v) s = (v, s)

evalE (Assignment x e) s = (x'', s'')
  where (x', s') = evalE e s 
        (x'', s'') = evalE e (Data.Map.insert x x' s)

-- Question 3 --
evalS :: Statement -> Store -> Store
evalS w@(While e s1) s = case (evalE e s) of
                          (BoolVal True,s')  -> let s'' = evalS s1 s' in evalS w s''
                          (BoolVal False,s') -> s'
                          _                  -> error "Condition must be a BoolVal"

evalS Skip s = s

evalS (Expr e) s = snd(evalE e s)

evalS (Sequence s1 s2) s = s''
  where s' = evalS s1 s
        s'' = evalS s2 s'

evalS (If e s1 s2) s = case (evalE e s) of
                        (BoolVal True,s')  -> evalS s1 s'
                        (BoolVal False,s') -> evalS s2 s'
                        _                  -> error "Condition must be a BoolVal"

-- Q6 --
evalS (For v e1 e2 s1) s = case (evalE e s') of
                          (BoolVal True, s3) -> s3
                          (BoolVal False, s3)  -> let s4 = evalS s1 s3 in evalS f s4
                                where inc = (BinOp Plus (Val (fst (evalE (Val v') s'))) (Val (IntVal 1)))
                                      (e'',s'') = (evalE (Assignment v inc) s')
                                      f = (For v (Val e'') e2 s1)
                          _                    -> error "Condition must be a BoolVal"
  where (v',s') = evalE e1 (Data.Map.insert v (fst(evalE e1 s)) s)
        e = (BinOp GreaterThan (Val (fst(evalE (Val v') s'))) (Val (fst(evalE e2 s'))))
        
-- Question 4 --
evalE_maybe :: Expression -> Store -> Maybe (Value, Store)
evalE_maybe (BinOp o a b) s = do (a',s') <- evalE_maybe a s
                                 (b',s'') <- evalE_maybe b s'
                                 return (applyOp o a' b', s'')

evalE_maybe (Var x) s = do x' <- Data.Map.lookup x s
                           return (x', s)

evalE_maybe (Val v) s = Just(v, s)

evalE_maybe (Assignment x e) s = do (x',s') <- evalE_maybe e s
                                    t' <- evalE_maybe e (Data.Map.insert x x' s')
                                    return t'

evalS_maybe :: Statement -> Store -> Maybe Store
evalS_maybe w@(While e s1) s = case (evalE_maybe e s) of
                               Just(BoolVal True,s')  -> let s'' = fromJust(evalS_maybe s1 s') in evalS_maybe w s''
                               Just(BoolVal False,s') -> Just s'
                               _                  -> Nothing

evalS_maybe Skip s = Just s

evalS_maybe (Sequence s1 s2) s = do s'<- evalS_maybe s1 s
                                    s'' <- evalS_maybe s2 s'
                                    return s''

evalS_maybe (Expr e) s = do (x,s') <- evalE_maybe e s
                            return s'

evalS_maybe (If e s1 s2) s = case (evalE_maybe e s) of
                              Just(BoolVal True,s')  -> evalS_maybe s1 s'
                              Just(BoolVal False,s') -> evalS_maybe s2 s'
                              _                  -> Nothing

 -- Q6 --
evalS_maybe (For v e1 e2 s1) s = do (v', s') <- evalE_maybe (Assignment v e1) s
                                    (e1',s'') <- evalE_maybe (Val v') s'
                                    (e2',s3) <- evalE_maybe e2 s''
                                    e  <- evalE_maybe (BinOp GreaterThan (Val e1') (Val e2')) s3
                                    case e of
                                      (BoolVal True, s4)  -> Just s4
                                      (BoolVal False, s4) -> do s5 <- evalS_maybe s1 s4
                                                                evalS_maybe (For v e' e2 s1) s5
                                                                  where e' = BinOp Plus (Val v') (Val (IntVal 1))
                                      _                   -> Nothing
 
-- Question 5 --
getVar :: Variable -> Imperative Value
getVar var = Imperative (\store -> ((Data.Map.lookup var store) >>= (\v -> Just (v,store))))

setVar :: Variable -> Value -> Imperative Value
setVar var val = Imperative (\store -> Just (val, Data.Map.insert var val store))

newtype Imperative a = Imperative {
    runImperative :: Store -> Maybe (a, Store)
}

instance Monad Imperative where
    return a = Imperative (\s -> Just (a,s))
    b >>= f = Imperative (\s -> do (v1,s1) <- (runImperative b) s
                                   runImperative (f v1) s1)
    fail _ = Imperative (\s -> Nothing)


evalE_monad :: Expression -> Imperative Value
evalE_monad (BinOp o a b)     = do a' <- evalE_monad a
                                   b' <- evalE_monad b
                                   return (applyOp o a' b')

evalE_monad (Var x)           = getVar x
evalE_monad (Val v)           = Imperative (\s -> Just(v,s))
evalE_monad (Assignment x e)  = do e' <- evalE_monad e
                                   setVar x e'


evalS_monad :: Statement -> Imperative ()
evalS_monad w@(While e s1)     = do e' <- evalE_monad e
                                    case (e') of
                                      (BoolVal True)  -> do evalS_monad s1 
                                                            evalS_monad w
                                      (BoolVal False) -> return ()
                                      _               -> return ()

evalS_monad Skip             = return ()
evalS_monad (Sequence s1 s2) = do evalS_monad s1
                                  evalS_monad s2
evalS_monad (Expr e)         = do evalE_monad e
                                  return ()
evalS_monad (If e s1 s2)     = do e' <- evalE_monad e
                                  case (e') of
                                    (BoolVal True)  -> evalS_monad s1
                                    (BoolVal False) -> evalS_monad s2
                                    _               -> return () 
-- Q6 --
evalS_monad (For v e1 e2 s1) = do v' <- evalE_monad (Assignment v e1)
                                  b  <- evalE_monad (BinOp GreaterThan (Val v') (e2))
                                  case b of
                                     (BoolVal True)  -> return ()
                                     (BoolVal False) -> do evalS_monad s1
                                                           evalS_monad (For v e' e2 s1)
                                                             where e' = BinOp Plus (Val v') (Val (IntVal 1))
                                     _               -> return ()
