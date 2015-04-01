# CMPS 112 WINTER 2015 FINAL PROJECT
# AUTHORS: Evan Hughes, Carl Eadler, Hadley Black
# Shell Script for running Synthactic Sugar 
# $1 --- Source Text
# $2 --- Key (C, C#, D, ...)
# $3 --- Scale (major/minor)
# $4 --- Tempo in bpm
# $5 --- Number of voices [1,2]

echo Generating Pure Data: PD.
echo From source text: $1.

python parser.py $1 $2 $3 $4 $5
python IMLtoPD.py PD $5

echo Melody in $2 $3 at $4 bpm with $5 voices.

echo "0 tempo" $4 ";" > tempo_file