from random import randint

class Game:
    def setup(self):
        ok = False
        while not ok:
            lower = input("Input the lower bound: ")
            try:
                lower = int(lower)
                ok = True
            except:
                print("Please input whole numbers only!")
        ok = False
        while not ok:
            upper = input("Input the upper bound: ")
            try:
                upper = int(upper)
                ok = upper > lower
                if not ok:
                    print(f"Upper bound must be greater than { lower }!")
            except Exception as e:
                print("Please input whole numbers only!")
        self.lower = lower
        self.upper = upper
        self.number = randint(self.lower, self.upper)
        self.won = False
            
    def guess(self, number):
        return self.number - number

    def play(self):
        self.won = False
        self.guesses = 0
        while not self.won:
            try:
                user_guess = input(f"Enter a number in range[{ self.lower }, { self.upper }]: ")
                try:
                    user_guess = int(user_guess)
                except:
                    raise Exception("Please input whole numbers only!")
                if user_guess < self.lower:
                    raise Exception(f"Number cannot be lower than { self.lower }!")
                elif user_guess > self.upper:
                    raise Exception(f"Number cannot be higher than { self.upper }!")
                range = self.guess(user_guess)
                self.guesses += 1
                if range < 0:
                    print("Too high!")
                elif range > 0:
                    print("Too low!")
                else:
                    print("Exactly right!")
                    print(f"You took { self.guesses } { 'guess' if self.guesses == 1 else 'guesses' }!")
                    self.won = True
            except BaseException as e:
                print(e)
            

def main():
    g = Game()
    g.setup()
    g.play()

if __name__ == "__main__":
    main()