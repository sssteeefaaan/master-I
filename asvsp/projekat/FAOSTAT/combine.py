import os
from sys import argv

def main():
    output_filename = argv[1] if len(argv) > 1 else "output.csv"
    workdir = argv[2] if len(argv) > 2 else os.curdir
    skip = 0
    filenames = [f for f in os.listdir(workdir) if f.endswith(".csv")]
    with open(os.path.join(workdir, output_filename), "a+") as output_file:
        for filename in filenames:
            with open(os.path.join(workdir, filename), "r") as file:
                output_file.writelines(file.readlines()[skip:])
            skip = 1

if __name__ == "__main__":
    main()