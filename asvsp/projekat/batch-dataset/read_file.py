import os
from sys import argv

def read_file(path):
    with open(path) as f:
        headers = f.readline()
        print(headers)
        err = "No errors"
        for l in f:
            print(l)
            if l == headers:
                err = "Duplicated headers!"
        print(err)

def main():
    workdir = argv[2] if len(argv) > 2 else os.curdir
    output = argv[1] if len(argv) > 1 else "output.csv"
    read_file(os.path.join(workdir, output))

if __name__ == "__main__":
    main()