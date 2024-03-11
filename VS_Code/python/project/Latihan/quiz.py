def new_game():
    guesses = []
    correct_guesses = 0
    question_num = 0

    for x in question:
        print("---------------------------------------")
        print(x)
        for i in option[question_num]:
            print(i)
        guess = input("Jawaban Anda (A,B,C) : ").upper()
        guesses.append(guess)

        correct_guesses += check_answer(question.get(x),guess)
        question_num += 1

    display_score(correct_guesses, guesses)

def check_answer(answer, guess):
    if answer == guess :
        print("Correct!")
        return 1
    else :
        print("Wrong!")
        return 0

def display_score(correct_guesses, guesses):
    print("---------------------------------------")
    print("results")
    print("---------------------------------------")
    print("Anwers : ", end=" ")
    for i in question : 
        print(question.get(i), end=" ")
    print()

    print("Guesses: ", end=" ")
    for i in guesses :
        print(i, end=" ")
    print()

    score = int((correct_guesses/len(question))*100)
    print("your score is : "+str(score)+ "%")
    


def play_again():
    reponse = input("Do you want to play again? (Y/N) : ").upper()
    if reponse == "Y":
        return True
    else :
        return False

question = { 
    "1. who created python? ": "A",
    "2. what year was python created? ":"B",
    "3. python is tributed to which comedy group? ":"C",
    "4. is the earth round? ":"A"
}
option = [["A. guido van Rossum", "B. Alan Walker", "C. Bill Gates"],
        ["A. 1980", "B. 1991", "C. 2000"],
        ["A. Lonely island", "B. Smosh", "C. Monty python"],
        ["A. True", "B. False", "C. sometimes"]]

new_game()
while play_again() :
    new_game()

print ("thank you for answer!")