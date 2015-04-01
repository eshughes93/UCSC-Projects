{---------------------
Evan Hughes
CMPS112 HW3 - Haskell
Due Feb 4, 2015
---------------------}
import Data.List

data BST k v = Empty | Node k v (BST k v) (BST k v)

--(1)
val :: BST k v -> Maybe v
val (Empty) = Nothing
val (Node k v l r) = Just v

--(2)
size :: BST k v -> Int
size (Empty) = 0
size (Node k v l r) = size l + size r + 1

--(3)
ins :: (Ord k) => k -> v -> BST k v -> BST k v
ins k v Empty = Node k v Empty Empty
ins k v (Node nk nv l r)
    | k == nk = Node k v l r
    | k < nk = Node nk nv (ins k v l) r
    | k > nk = Node nk nv l (ins k v r)
    | otherwise = Node nk nv l r

--(4)
instance (Show v) => Show (BST k v) where
    show Empty = ""
    show (Node k v l r) = "(" ++ (show l) ++ (show v) ++ (show r) ++ ")"


----------------------------------
data JSON = JStr String
          | JNum Double
          | JArr [JSON]
          | JObj [(String, JSON)]
----------------------------------
--(5)
{---------------------------------------------------------
showJObj recursively prints a String for a list of tuples
consisting of (String,JSON), giving back a list of Strings
containing the JSON representation of the JObj.
---------------------------------------------------------}
showJObj :: [(String,JSON)] -> [String]
showJObj [] = []
showJObj ((s,j):js) = ("\"" ++ s ++ "\"" ++ ":" ++ show j) : showJObj js

instance Show JSON where
    show (JStr s) = "\"" ++ s ++ "\""
    show (JNum d) = show d
    show (JArr xs) = show xs
    show (JObj js) = "{" ++ (intercalate "," (showJObj js)) ++ "}"


--(6)
class Json a where
  toJson :: a -> JSON
  fromJson :: JSON -> a

instance Json Double where
  toJson d = JNum d
  fromJson (JNum d) = d

instance (Json a) => Json [a] where
  toJson arr = JArr (map toJson arr)
  fromJson (JArr arr) = map fromJson arr
