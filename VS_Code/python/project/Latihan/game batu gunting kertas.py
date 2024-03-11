import random
from fungsi_game import *

while True :
   suit =["batu", "gunting", "kertas"]
   pc = random.choice(suit)
   player= None
   while player not in suit :
      player = input("batu, gunting, atau kertas : ").lower()


   if player==pc :
      a(pc, player)
   elif player=="batu":
      if pc == "kertas":
         b(pc, player)
      elif pc == "gunting" :
         c(pc, player)
   elif player=="gunting":
      if pc == "batu":
         b(pc, player)
      elif pc == "kertas" :
         c(pc, player)
   elif player=="kertas":
      if pc == "gunting":
         b(pc, player)
      elif pc == "batu" :
         c(pc, player)

   play = None
   while play != "Y" :
      pilih = ["Y", "N"]
      play = input("Main liak? (Y/N) \t: ").upper()
      if play != pilih :
         while play not in pilih:
            play = input("mohon pilih (Y/N) \t: ").upper()

      if play == "N":
         stop = input("Anda Yakin? (Y/N) \t: ").upper()
         if stop != pilih :
            while stop not in pilih:
               stop = input("mohon pilih (Y/N) \t: ").upper()

         if stop == "N":
            continue
         elif stop == "Y":
            break
   if play == "Y" :
      continue
   if stop == "Y":
      break
print("Terima Kasih Telah Bermain!")