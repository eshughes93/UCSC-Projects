# CMPS 112 WINTER 2015 FINAL PROJECT
# AUTHORS: Evan Hughes, Carl Eadler, Hadley Black
# IML to Pd interpreter for SYNTHTACTIC SUGAR

import sys, fileinput, string

for i in range(0, 9):
   open(sys.argv[1] + str(i), 'w').close()

num_v = int(sys.argv[2])

for j in range(0, num_v):
   target_filename = sys.argv[1] + str(j)
   open(target_filename, 'w').close()

   IML_filename = "IML" + str(j) + ".txt"

   IML_file = open(IML_filename, "r")
   PD_file  = open(target_filename, "a")

   with IML_file as file:
      data = file.readlines()

   notes = data[0].split(",")

   i = 0
   time = 0 

   for note in notes:
   
      note = note.replace("(","")
      note = note.replace(")","")
   
      pd = note.split("/")
   
      pd[0] = pd[0].replace("[","")
      pd[0] = pd[0].replace("]","")
      pd[1] = pd[1].replace("[","")
      pd[1] = pd[1].replace("]","")
   
      pitches   = pd[0].split(":")
      durations = pd[1].split(":")

      for dur in durations:	
    
         voice = 0
         voice += j
         for pitch in pitches:                   
            PD_file.write(str(time) + " voice" + str(voice) + " " + str(pitch) + ";\n")
            voice += 1  
            time = 0

         time = int(dur) 
