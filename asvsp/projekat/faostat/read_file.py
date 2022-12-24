import os
from sys import argv

def read_file(path):
    with open(path) as f:
        [print(line) for line in f]

def main():
    workdir = argv[2] if len(argv) > 2 else os.curdir
    output = argv[1] if len(argv) > 1 else "output.csv"
    read_file(os.path.join(workdir, output))

if __name__ == "__main__":
    main()