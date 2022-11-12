# Web Shop Angular aplikacija
## Basic Authentication
Postoje dve vrste korisnika: **administratori** i **kupci**.
Administratori mogu da vide artikle, dodaju nove i menjaju postojeće. Dostupne su im rute: */artikli*, */add* i */edit/:id*.
Kupci mogu da vide artikli i da ih dodaju u kopru (radi jednostavnosti nema odabira količine niti uklanjanja artikala iz korpe). Svaki korisnik vidi samo artikle iz svoje korpe. Dostupne su im rute: */artikli* i */korpa*.

### Back-end
Pre prvog pokretanja back-end-a obrisati bazu.
Inicijalno su dodati sledeći korisnici:
  - username: admin@admin.com, password: admin, uloga: administrator
  - username: aaa@aaa.com, password: aaaaaa, uloga: kupac
  - username: bbb@bbb.com, password: bbbbbb, uloga: kupac
  - username: ccc@ccc.com, password: cccccc, uloga: kupac

### Frond-end
Obratiti pažnju na:
  - **AuthService**, čuvanje kredencijala u *localStorage*-u i generisanje *HttpHeader*-a za sve HTTP zahteve.
  - **AppRoutingModule**, postavljanje uslova pod kojima se mogu pojedine rute ativirati.
