from utils import *
import os

INPUT_FILE = "_input"

def get_score(number:int,list_:list):
    score = 0
    for n in list_:
        if(n==number):
            score+=1
    return score

def main() -> int:
    input_ = get_input()
    
    response = 0
    for n in input_[0]:
        response += n * get_score(n,input_[1])

    print(response)
    return 0

if __name__ == "__main__":
    os._exit(main())