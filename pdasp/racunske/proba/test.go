package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"os"
	"regexp"
	"strconv"
)

func korisnickiUnos(poruka string) string {
	var ret string
	fmt.Printf(poruka)
	fmt.Scanln(&ret)
	return ret
}

func ucitajMapu(filePath string) (map[interface{}]interface{}, error) {
	var ret map[interface{}]interface{}

	file, err := os.OpenFile(filePath, os.O_RDONLY, 0777)
	defer file.Close()

	if err != nil {
		fmt.Printf("Greska pri otvaranju fajla (%s): '%s'", filePath, err.Error())
		return nil, err
	}

	readData, err := ioutil.ReadAll(file)
	if err != nil {
		fmt.Printf("Greska pri citanju fajla (%s): '%s'!\n", filePath, err.Error())
		return nil, err
	}

	err = json.Unmarshal(readData, &ret)
	if err != nil {
		fmt.Printf("Greska pri parsiranju json-a (%s): '%s'!\n", filePath, err.Error())
		return nil, err
	}

	return ret, nil
}

func unesiteVrednostKljuca() interface{} {
	for {
		fmt.Printf("Unesite vrednost kljuca\n")
		fmt.Printf("1 - Bool")
		fmt.Printf("2 - Int")
		fmt.Printf("3 - Float")
		fmt.Printf("4 - String")
		fmt.Printf("5 - Mapa")

		unos := korisnickiUnos("Odaberite opciju (1-5): ")

		switch unos {
		case "1":
			vrednost := korisnickiUnos("Unesite vrednost true/false: ")
			val, err := strconv.ParseBool(vrednost)
			if err != nil {
				fmt.Printf("Greska pri parsiranju bool vrednosti (%s) '%s'!\n", vrednost, err.Error())
				continue
			}
			return val
		case "2":
			vrednost := korisnickiUnos("Unesite celobrojnu vrednost: ")
			val, err := strconv.ParseInt(unos, 10, 64)
			if err != nil {
				fmt.Printf("Greska pri parsiranju int vrednosti (%s) '%s'!\n", vrednost, err.Error())
				continue
			}
			return val
		case "3":
			vrednost := korisnickiUnos("Unesite realni broj: ")
			val, err := strconv.ParseFloat(unos, 64)
			if err != nil {
				fmt.Printf("Greska pri parsiranju bool vrednosti (%s) '%s'!\n", vrednost, err.Error())
				continue
			}
			return val
		case "4":
			return korisnickiUnos("Unesite string vrednost: ")
		case "5":
			return unesiMapu()
		default:
			fmt.Printf("Uneli ste pogresnu opciju '%s'\n", unos)
		}
	}
}

func unesiMapu() map[interface{}]interface{} {
	var ret = make(map[interface{}]interface{})
	for {
		fmt.Printf("1 - Novi kljuc\n")
		fmt.Printf("2 - Gotovo\n")

		unos := korisnickiUnos("Unesite opciju (1/2): ")
		switch unos {
		case "1":
			kljuc := korisnickiUnos("Unesite kljuc mape: ")
			matched, _ := regexp.MatchString("[A-Za-z_]+[A-Za-z0-9_]*", kljuc)
			if !matched {
				fmt.Printf("Ime mape mora sadrzati iskljucivo alfanumericke znake i mora pocinjati slovom ili '_'!\n")
				continue
			}
			_, ok := ret[kljuc]
			if ok {
				fmt.Printf("Mapa vec sadrzi prosledjeni kljuc '%s'!\n", kljuc)
				continue
			}
			ret[kljuc] = unesiteVrednostKljuca()
		case "2":
			return ret
		default:
			fmt.Printf("Uneli ste pogresnu opciju!\n")
		}
	}
}

func sacuvajMapu(filePath string, mapa map[interface{}]interface{}) error {
	file, err := os.OpenFile(filePath, os.O_CREATE|os.O_WRONLY|os.O_TRUNC, 0777)
	defer file.Close()

	if err != nil {
		fmt.Printf("Nije moguce otvoriti fajl (%s) '%s'!\n", filePath, err.Error())
		return err
	}

	bytes, err := json.Marshal(mapa)
	if err != nil {
		fmt.Printf("Greska pri parsiranju vrednosti (%s) '%s'!\n", filePath, err.Error())
		return err
	}

	_, err = file.Write(bytes)
	if err != nil {
		fmt.Printf("Greska pri upisivanju vrednosti u fajl (%s) '%s'\n", filePath, err.Error())
		return err
	}

	return nil
}

func prikazMape(mape map[interface{}]interface{}) {
	for {
		fmt.Printf("Prikazi mapu menu!\n")
		fmt.Printf("1 - Unesi kljuc mape\n")
		fmt.Printf("2 - Nazad\n")

		unos := korisnickiUnos("Unesite opciju (1/2): ")
		switch unos {
		case "1":
			filePath := korisnickiUnos("Unesite kljuc mape: ")
			matched, _ := regexp.MatchString("[A-Za-z_]+[A-Za-z0-9_]*", filePath)
			if !matched {
				fmt.Printf("Ime mape mora sadrzati iskljucivo alfanumericke znake i mora pocinjati slovom ili '_'!\n")
				continue
			}

			mapa, err := ucitajMapu(fmt.Sprintf("%s.json", filePath))
			if err != nil {
				fmt.Printf("Ne postoji mapa sa prosledjenim kljucem!\n")
				continue
			}
			mape[filePath] = mapa
			fmt.Printf("%v\n", mapa)
		case "2":
			return
		default:
			fmt.Printf("Uneli ste pogresnu opciju!\n")
		}
	}
}

func unosMape(mape map[interface{}]interface{}) {
	var kljuc string
	var mapa map[interface{}]interface{}
	for {
		kljuc = korisnickiUnos("Unesite ime mape: ")
		matched, _ := regexp.MatchString("[[A-Za-z_]+[A-Za-z0-9_]*", kljuc)
		if !matched {
			fmt.Printf("Ime mape mora sadrzati iskljucivo alfanumericke znake i mora pocinjati slovom ili '_'!\n")
			continue
		}
		_, ok := mape[kljuc]
		if ok {
			fmt.Printf("Vec postoji mapa sa imenom '%s'!\n", kljuc)
			continue
		}
		mapa = unesiMapu()
		break
	}
	mape[kljuc] = &mapa
	sacuvajMapu(fmt.Sprintf("%s.json", kljuc), mapa)
}

func mainMenu() {
	mape := make(map[interface{}]interface{})
	for {
		fmt.Printf("Glavni menu!\n")
		fmt.Printf("1 - Unesi mapu\n")
		fmt.Printf("2 - Prikazi mapu\n")
		fmt.Printf("3 - Kraj\n")

		unos := korisnickiUnos("Unesite opciju (1-3): ")
		switch unos {
		case "1":
			unosMape(mape)
		case "2":
			prikazMape(mape)
		case "3":
			fmt.Printf(("Kraj"))
			return
		default:
			fmt.Printf("Uneli ste pogresnu opciju!")
		}
	}
}
func main() {
	mainMenu()
}
