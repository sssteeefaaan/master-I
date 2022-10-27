def fibonacci(n):
    if n < 2:
        return 1
    return fibonacci(n - 1) + fibonacci(n - 2)

def fibonacciSequence(n):
    for i in range(n):
        print(fibonacci(i), end=" ")
    print()


def main():
    range = -1
    while range < 1:
        range = int(input("How many fibonacci numbers do you want: "))
    fibonacciSequence(range)

if __name__ == "__main__":
    main()
    
