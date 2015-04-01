# CMPS 112 WINTER 2015 FINAL PROJECT
# AUTHORS: Evan Hughes, Carl Eadler, Hadley Black
# IML from English interpreter for Synthtactic Sugar
# Part of speech, note number, beats example 
# noun   :: 1,5   
# pro    :: 1,5               "I, you, he, she" 
# adj    :: 2,6,7  
# verb   :: 3
# adv    :: 5                 describes a verb
# prep   :: 6      :: 1/2     linking nouns to other words "to, at, after, on, but"
# conj   :: 7      :: 1/2     "and, but, when"
# int    :: 8                 "oh!, ouch!, hi!"

# asumed note form: (POS, s_count, last_char)
# IML    note form: ([p1, p2 ...]/[d1, d2 ...])
 
# --- music settings ---  
# key: C, C#, D, D#, E, F, F#, G, G#, A, A#, B
# scale: "major", "minor"
# tempo: bpm value  


import sys, fileinput, string, random

# helper function for shifting notes from C major to correct key
def keyChange(key, scale, notes):

   if  key == "C#": 
       for x in notes: notes[x] += 1
   elif key == "D":
       for x in notes: notes[x] += 2
   elif key == "D#":
       for x in notes: notes[x] += 3
   elif key == "E":
       for x in notes: notes[x] += 4
   elif key == "F":
       for x in notes: notes[x] += 5
   elif key == "F#":
       for x in notes: notes[x] += 6
   elif key == "G":
       for x in notes: notes[x] -= 5
   elif key == "G#":
       for x in notes: notes[x] -= 4
   elif key == "A":
       for x in notes: notes[x] -= 3
   elif key == "A#":
       for x in notes: notes[x] -= 2
   elif key == "B":
       for x in notes: notes[x] -= 1

   if scale == "harmonic-minor":
       notes[3] -= 1
       notes[6] -= 1
   if scale == "natural-minor" or scale == "minor":
       notes[3] -= 1
       notes[6] -= 1
       notes[7] -= 1
   if scale == "melodic-minor":
       notes[3] -= 1
      
 
   return notes

def getPitch(pos, n):
    return {
       "noun"             : random.choice([n[1], n[3], n[5], n[8]]),
       "pronoun"          : random.choice([n[1], n[8], n[5]]),
       "adjective"        : random.choice([n[2], n[6], n[7]]),
       "verb"             : random.choice([n[3], n[5]]),
       "adverb "          : random.choice([n[2], n[6]]),
       "preposition"      : random.choice([n[2], n[4], n[6]]),
       "conjunction"      : random.choice([n[2], n[4], n[6]]),
       "interjection"     : random.choice([n[5], n[8]]),
       "definite article" : random.choice([n[2], n[6]])
    }.get(pos, n[1])

def getDuration(pos, l_char, tempo):
    #tempo *= 2
    dur = 1
    if (l_char == "."): dur *= 4
    if (l_char == ","): dur *= 2
    if (pos == "conjunction" or pos == "preposition"): dur = float(dur)/2
    dur = (float((dur * 60000)) / tempo)
    #print dur
    return int(dur) 

def toIML(text, key, scale, tempo, voices):
  
  # initialize notes to C major at octave 3 
  notes = {1: 48, 2: 50, 3: 52, 4: 53, 5: 55, 6: 57, 7: 59, 8: 60} 
  # shift to correct key
  notes = keyChange(key, scale, notes)

  # IML note list
  IML = ""

  for i in range(0, int(voices)):
    IML_filename = "IML" + str(i) + ".txt"
    open(IML_filename, 'w').close()
    IML_file = open(IML_filename, "a")

    # make IML file
    for word in text:
      #print word[0]
      dur = ""
      note  = "(["
      note += (str(getPitch(word[1], notes)) + "]/[")
      if word[0] == 0: 
        dur = str( (getDuration(word[1], word[2], float(tempo)))/(word[0] + 1))
      else: 
        dur = str( (getDuration(word[1], word[2], float(tempo)))/word[0] )
      
      if word[0] == 0: 
        for i in range (0, word[0] + 1): note += (dur + ":")
      else: 
        for i in range (0, word[0]): note += (dur + ":")
      note = note[:-1] + "])"
      IML += (note + ",")


    IML = IML[:-1]
    IML_file.write(IML)
    IML = ""


