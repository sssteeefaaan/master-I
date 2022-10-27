def reverse(sentence):
    words = sentence.split(" ")
    #words.reverse()
    reversed = list()
    for w in words:
        reversed = [w] + reversed
    return (" ").join(reversed)

def main():
    sentence = "Python is a popular programming language."
    while not sentence:
        sentence = input("Enter a sentence: ")
    print(reverse(sentence))

if __name__ == "__main__":
    main()