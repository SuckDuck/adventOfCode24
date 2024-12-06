from utils import *
import os

INPUT_FILE = "_input"

def main() -> int:
    input_ = get_input()
    input_[0].sort()
    input_[1].sort()

    response = 0
    for i in range(len(input_[0])):
        response += abs(input_[0][i] - input_[1][i])

    print_input(input_,5)
    print(response)
    return 0

if __name__ == "__main__":
    os._exit(main())