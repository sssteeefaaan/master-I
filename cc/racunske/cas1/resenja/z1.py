def primesInRange(lower, upper):
    for i in range(lower, upper + 1):
        if isPrime(i):
            print(i)

def isPrime(n):
    # Is 1 a prime number
    if n < 1:
        return False
    if n < 4:
        return True
    if n % 2 == 0 or n % 3 == 0:
        return False
    j = 5
    while j * j <= n:
        if n % j == 0:
            return False
        j += 1
    return True

def main():
    lower, upper = 0, 21
    primesInRange(lower, upper)

if __name__ == "__main__":
    main()