from datetime import datetime

class Pas:
    def __init__(self, boja, ime, datum_rodjenja : datetime):
        self.boja = boja
        self.ime = ime
        self.datum_rodjenja = datum_rodjenja

    def starost(self):
        return (datetime.now().year - self.datum_rodjenja.year) * 7

    def info(self):
        godine = self.starost()
        normalizovano = godine if godine < 21 else godine % 10
        padez = 'godina' if normalizovano == 0 or normalizovano >= 5 else 'godina' if normalizovano != 1 else 'godinu'
        return f"{ self.ime } je { self.boja } boje i trenutno ima { godine } { padez }!"

def main():
    psi = list()
    psi.append(Pas("braon", "Ben", datetime(2019, 1, 1)))
    psi.append(Pas("žute", "Kivi", datetime(2012, 1, 1)))
    psi.append(Pas("crne", "Džeki", datetime(2021, 1, 1)))
    psi.append(Pas("belo-žute", "Miki", datetime(2020, 1, 1)))
    for p in psi:
        print(p.info())

if __name__ == "__main__":
    main()