{------------------------------------------------
-- CMPS112 Winter 2015 Homework 2 --
Program: hw2.hs
Authors: 
Evan Hughes [eshughes@ucsc.edu]

NOTE: 
I pair programmed with:
Justin Wong [jujwong@ucsc.edu]

Solely on problem 11, on which we
worked together for 3 hours.
-------------------------------------------------}

--(1)
myFoldl :: (a -> b -> a) -> a -> [b] -> a
myFoldl f x [] = x
myFoldl f x (y:ys) = myFoldl f (f x y) ys

--(2)
list_concat :: [a] -> a -> [a]
list_concat x y = [y] ++ x

myReverse :: [a] -> [a]
myReverse xs = foldl (list_concat) [] xs


--(3)
myFoldr :: (a-> b -> b) -> b -> [a] -> b
myFoldr f x [] = x
myFoldr f x ys = foldl (flip f) x (reverse ys)

--(4)
myFoldl2 :: (a -> b -> a) -> a -> [b] -> a
myFoldl2 f x [] = x
myFoldl2 f x ys = foldr (flip f) x (reverse ys)

-- (5)
isUpper :: Char -> Bool
isUpper c = elem c ['A'..'Z']

--(6)
onlyCapitals1 :: String -> String
onlyCapitals1 s = filter (isUpper) s

--(7)
onlyCapitals2 :: String -> String
onlyCapitals2 xs = [x|x<-xs, isUpper x]

--(8)
onlyCapitals3 :: String -> String
onlyCapitals3 [] = []
onlyCapitals3 (x:xs) 
    | isUpper x = x : onlyCapitals3 xs
    | otherwise = onlyCapitals3 xs

--(9)
divRemainder :: Int -> Int -> (Int,Int)
divRemainder dividend divisor = (quotient, remainder)
    where quotient = div dividend divisor
          remainder = mod dividend divisor

--(10)
toNum :: String -> [Int]
toNum [] = []
toNum (c:cs) = (read [c] + 0) : toNum cs

digitSum :: Int -> Int
digitSum num = sum digit_list
    where digit_list = toNum(breakdown)
          breakdown = show num

--(11)
{---------------------------------------
ones is a list of strings containing
the english word for the numbers
in the ones place in the decimal system.
----------------------------------------}
ones        = ["","one","two",
               "three","four","five",
               "six","seven",
               "eight","nine"]
{-----------------------------------------
tens is a list of strings containing
the english words for multiples of ten
from ten to ninety in the decimal system.
-----------------------------------------}
tens        = ["","ten","twenty","thirty",
               "forty","fifty","sixty",
               "seventy","eighty","ninety"]
{------------------------------------
addZ takes two ints and a string.
len is the length of the string,
and dlen is the desired length
after prepending 0's to the string.

(Realizing now that this could be done
with passing only two parameters...)
-----------------------------------}
addZ :: Int -> Int -> String -> String
addZ len dlen "" = ""
addZ len dlen cs 
  | len == dlen = cs
  | otherwise = addZ (len+1) dlen ("0" ++ cs)

{-----------------------------------------------
divStr was originally written as a mathematical 
function with which we would divide a number
passed in String form by 1000 in order to chop the 
top 3 digits off of a large number.
Then we realized this could be done simply by 
deleting the front 3 elements, or removing 
continuous zeroes.

divStr removes all leading zeroes from a
string of digits.
-------------------------------------------------}
divStr :: String -> String
divStr [] = []
divStr (a:b:c:d:e:f:cs) 
  | ( d == '0' && e == '0' && f == '0') = divStr(cs)
  | otherwise = divStr (d:e:f:cs)
divStr (c:cs)
  | c == '0' = divStr(cs)
  | otherwise = (c:cs)

{-
sayHundred takes a String of digits,
and based upon the leading 3 digits,
outputs the english word for what number
that might be.
i.e. sayHundred "123400" ->
     one hundred twenty three

This function counts on adding zeroes (addZ)
to the front prior to calling it for 
numbers like "1234", as in the number
should be passed as "001234" so that 
it does not return one hundred twenty three
instead of one for the thousandths place.
-}
sayHundred :: String -> String
sayHundred x
  | h == "" && t == "" = o
  | (h /= "" && t == "" && o == "") = h
  | (h /= "" && t == "" && o /= "") = h ++ " " ++ o
  | (h == "" && o /= "") = t ++ " " ++ o
  | (h == "" && t /= "" && o == "") = t
  | otherwise = h ++ " " ++ t ++ " " ++ o
  where h = if (read [(x !! 0)] + 0) /= 0
            then (ones !! (read [(x !! 0)] + 0)) ++ " " ++ "hundred"
            else ""
        t = tens !! (read [(x !! 1)] + 0)
        o = ones !! (read [(x !! 2)] + 0)

{------------------------------------
sayNum' was originally just sayNum,
but we had to add the apostrophe so 
that in our actual sayNum function
we could chop off the trailing piece
of whitespace resulting from the
sayNum' function.

We know its not pretty but after
spending 3 hours on it we decided
to forsake making the function 
cleaner and more concise in the 
interest of being done.
-------------------------------------}

sayNum' :: String -> String
sayNum' "" = ""
sayNum' x 
  | length x < 4 = sayHundred(addZ (length x) 3 x)
  | length x < 7 = sayHundred(addZ (length x) 6 x) ++ " " ++ 
    "thousand" ++ " " ++ sayNum' (divStr(addZ (length x) 6 x))
  | length x < 10 = sayHundred(addZ (length x) 9 x) ++ " " ++
    "million" ++ " " ++ sayNum'(divStr(addZ (length x) 9 x)) 
  | length x < 13 = sayHundred(addZ (length x) 12 x) ++ " " ++
    "billion" ++ " " ++ sayNum'(divStr(addZ (length x) 12 x))
  | length x < 16 = sayHundred(addZ (length x) 15 x) ++ " " ++
    "trillion" ++ " " ++ sayNum'(divStr(addZ (length x) 15 x))
  | length x < 19 = sayHundred(addZ (length x) 18 x) ++ " " ++
    "quadrillion" ++ " " ++ sayNum'(divStr(addZ (length x) 18 x))
  | length x < 22 = sayHundred(addZ (length x) 21 x) ++ " " ++
    "quintillion" ++ " " ++ sayNum'(divStr(addZ (length x) 21 x))
  | length x < 25 = sayHundred(addZ (length x) 24 x) ++ " " ++
    "sextillion" ++ " " ++ sayNum'(divStr(addZ (length x) 24 x))
  | length x < 28 = sayHundred(addZ (length x) 27 x) ++ " " ++
    "septillion" ++ " " ++ sayNum'(divStr(addZ (length x) 27 x))
  | length x < 31 = sayHundred(addZ (length x) 30 x) ++ " " ++
    "octillion" ++ " " ++ sayNum'(divStr(addZ (length x) 30 x))
  | length x < 34 = sayHundred(addZ (length x) 33 x) ++ " " ++
    "nonillion" ++ " " ++ sayNum'(divStr(addZ (length x) 33 x))
  | length x < 37 = sayHundred(addZ (length x) 36 x) ++ " " ++
    "decillion" ++ " " ++ sayNum'(divStr(addZ (length x) 36 x))
  | length x < 40 = sayHundred(addZ (length x) 39 x) ++ " " ++
    "undecillion" ++ " " ++ sayNum'(divStr(addZ (length x) 39 x))
  | length x < 43 = sayHundred(addZ (length x) 42 x) ++ " " ++
    "duodecillion" ++ " " ++ sayNum'(divStr(addZ (length x) 42 x))
  | length x < 46 = sayHundred(addZ (length x) 45 x) ++ " " ++
    "tredecillion" ++ " " ++ sayNum'(divStr(addZ (length x) 45 x))
  | length x < 49 = sayHundred(addZ (length x) 48 x) ++ " " ++
    "quattuordecillion" ++ " " ++ sayNum'(divStr(addZ (length x) 48 x))
  | length x < 52 = sayHundred(addZ (length x) 51 x) ++ " " ++
    "quindecillion" ++ " " ++ sayNum'(divStr(addZ (length x) 51 x))
  | length x < 55 = sayHundred(addZ (length x) 54 x) ++ " " ++
    "sexdecillion" ++ " " ++ sayNum'(divStr(addZ (length x) 54 x))
  | length x < 58 = sayHundred(addZ (length x) 57 x) ++ " " ++
    "septendecillion" ++ " " ++ sayNum'(divStr(addZ (length x) 57 x))
  | length x < 61 = sayHundred(addZ (length x) 60 x) ++ " " ++
    "octodecillion" ++ " " ++ sayNum'(divStr(addZ (length x) 60 x))
  | length x < 64 = sayHundred(addZ (length x) 63 x) ++ " " ++
    "novemdecillion" ++ " " ++ sayNum'(divStr(addZ (length x) 63 x))
  | length x == 64 = sayHundred(addZ (length x) 66 x) ++ " " ++
    "vigintillion" ++ " " ++ sayNum'(divStr(addZ (length x) 66 x))
  | length x > 64 = "Error: input smaller number."
  | otherwise = ""

sayNum :: String -> String
sayNum x 
  | last num == ' ' = init num
  | otherwise = num
  where num = sayNum' x