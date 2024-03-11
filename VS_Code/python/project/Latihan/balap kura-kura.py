from turtle import *
from random import randint

speed(10)
penup()
goto(-140, 140)
for step in range(15):
  write(step)
  right(90)
  forward(10)
  pendown()
  forward(150)
  penup()
  backward(160)
  left(90)
  forward(20)

x = Turtle()
x.color('red')
x.shape('turtle')
x.penup()
x.goto(-160,100)
x.pendown()

a = Turtle()
a.color('blue')
a.shape('turtle')
a.penup()
a.goto(-160,70)
a.pendown()

b = Turtle()
b.color('green')
b.shape('turtle')
b.penup()
b.goto(-160,40)
b.pendown()

c = Turtle()
c.color('yellow')
c.shape('turtle')
c.penup()
c.goto(-160,10)
c.pendown()

for turn in range(100):
    x.forward(randint(1,5))
    a.forward(randint(1,5))
    b.forward(randint(1,5))
    c.forward(randint(1,5))