def get_input(path:str):
    list0 = []
    list1 = []
    with open(path, "r") as f:
        for line in f.read().split("\n"):
            if(len(line)>0):
                c = line.split()
                list0.append(int(c[0]))
                list1.append(int(c[1]))

    return list0, list1

def print_input(input_,range_:int = -1):
    print("group1    group2")
    for i in range(len(input_[0])):
        print(f"{input_[0][i]}    {input_[1][i]}")
        if(range_-1 == i):
            print("......")
            return