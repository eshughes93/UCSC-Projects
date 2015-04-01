isSorted :: (Ord a) => [a] -> Bool
isSorted [] = True
isSorted [x] = True
isSorted (x:y:xs) 
    | x > y = False
    | otherwise = isSorted (y:xs)