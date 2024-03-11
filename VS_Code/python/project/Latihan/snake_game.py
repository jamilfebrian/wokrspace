from tkinter import *
import random

GAME_WIDHT = 1000
GAME_HEIGHT = 600
SPEED = 120
SPACESIZE = 25
BODY_PART = 3
SNAKE_COLOR = 'green'
FOOD_COLOR = 'red'
BACKGROUND = 'black'

class Snake:
    def __init__(self):
        self.body_size = BODY_PART
        self.coordinates = []
        self.squares = []
        for i in range (0, BODY_PART):
            self.coordinates.append([0,0])
        for x,y in self.coordinates:
            square = canvas.create_rectangle(x,y,x+SPACESIZE,y+SPACESIZE, fill=SNAKE_COLOR, tag="Snake")
            self.squares.append(square)
class Food:
    def __init__(self):
        x=random.randint(0, GAME_WIDHT/SPACESIZE-1)*SPACESIZE
        y=random.randint(0, GAME_HEIGHT/SPACESIZE-1)*SPACESIZE
        self.coordinates = [x,y]
        canvas.create_oval(x,y,x+SPACESIZE,y+SPACESIZE, fill=FOOD_COLOR,tags='food')
def next_turn(snake, food):
    x,y = snake.coordinates[0]
    if direction=='up':
        y-=SPACESIZE
    elif direction == 'down':
        y+=SPACESIZE
    elif direction == 'left':
        x-=SPACESIZE
    elif direction == 'right':
        x+=SPACESIZE
    snake.coordinates.insert(0,(x,y))
    square = canvas.create_rectangle(x,y,x+SPACESIZE,y+SPACESIZE, fill=SNAKE_COLOR)
    snake.squares.insert(0,square)

    if x == food.coordinates[0]and y==food.coordinates[1]:
        global score
        score += 1
        label.config(text="Score:{}".format(score))
        canvas.delete("food")
        food = Food()
    else :
        del snake.coordinates[-1]
        canvas.delete(snake.squares[-1])
        del snake.squares[-1]

    if check_collisions(snake):
        game_over()
    else:
        window.after(SPEED, next_turn,snake,food)
    

def change_direction(new_direction):
    global direction
    if new_direction == 'left':
        if direction != 'right':
            direction = new_direction
    elif new_direction == 'right':
        if direction != 'left':
            direction = new_direction
    if new_direction == 'up':
        if direction != 'down':
            direction = new_direction
    if new_direction == 'down':
        if direction != 'up':
            direction = new_direction
def check_collisions(snake):
    x,y = snake.coordinates[0]
    if x<0 or x>=GAME_WIDHT:
        return True
    elif y<0 or y>=GAME_HEIGHT:
        return True
    for body_part in snake.coordinates[1:]:
        if x==body_part[0] and y== body_part[1]:
            return True
    return False

def game_over():
    canvas.delete(ALL)
    canvas.create_text(canvas.winfo_width()/2,canvas.winfo_height()/2,
                    font=('Consolas',70),text="HAHAHA! Pakak", fill='red', tag="gameover")


window = Tk()
window.title("Main Ula")
window.resizable(False,False)

score = 0
direction = 'down'

label = Label(window,text='Score:{}'.format(score),font=('Consolas', 40))
label.pack()

canvas = Canvas(window, bg=BACKGROUND, height=GAME_HEIGHT, width=GAME_WIDHT)
canvas.pack()
window.update()
window_width = window.winfo_width()
window_height = window.winfo_height()
schreen_width = window.winfo_screenwidth()
schreen_height = window.winfo_screenheight()

x = int((schreen_width/2)-(window_width/2))
y = int((schreen_height/2)-(window_height/2))

window.geometry(f"{window_width}x{window_height}+{x}+{y}")

window.bind('<Left>', lambda event: change_direction('left'))
window.bind('<Right>', lambda event: change_direction('right'))
window.bind('<Up>', lambda event: change_direction('up'))
window.bind('<Down>', lambda event: change_direction('down'))

snake = Snake()
food = Food()
next_turn(snake,food)

window.mainloop()
