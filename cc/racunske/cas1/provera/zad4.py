
from datetime import date

class Dog:
    def __init__(self, name, breed, color, dateOfBirth):
        self.name = name
        self.breed = breed
        self.color = color
        self.dateOfBirth = dateOfBirth

    def calculateDogYears(self):
        dogAge = (date.today().year - self.dateOfBirth.year) * 7
        print(self.name + " is " + str(dogAge) + " years old in dog age.")

dog = Dog("Max", "Golden retriever", "black", date(2015,2,17))
dog.calculateDogYears()