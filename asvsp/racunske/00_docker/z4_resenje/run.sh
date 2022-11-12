#!/usr/bin/env bash
#Skripta sa resenjima za zadatak 4

# Komanda pomocu koje se vrsi ispis svake komand pre pokretanja
#set -o xtrace

# Zaustavljanje postojecih kontejnera i brisanje resursa (radi ponovnog pokretanja skripte)
#docker stop $(docker ps -a -q)
#docker rm $(docker ps -a -q)
#docker network rm back-tier
#docker network rm front-tier
#docker volume prune

# Kreiranje mreza potrebnih za uspostavljanje veze medju kontejnerima
docker network create back-tier
docker network create front-tier

# Pokretanje redis kontejnera, uz povezivanje na 'back-tier' mrezu i mapiranje portova
docker run --net back-tier -d --name redis -p 6379:6379 redis:5.0-alpine3.10

# Pokretanje postgres kontejnera, uz povezivanje na 'back-tier' mrezu i dodelu skladista podataka
docker run -d -v="db-data:/var/lib/postgresql/data" -e POSTGRES_USER="postgres" -e POSTGRES_PASSWORD="postgres" --net back-tier --name db postgres:9.4

# Izgradnja i pokretanje worker kontejnera, uz povezivanje na 'back-tier' mrezu i mapiranje portova
docker build -t worker:1.0 ./worker
docker run -d --net back-tier --name worker worker:1.0

# Izgradnja i pokretanje result kontejnera, uz povezivanje na 'back-tier' i 'front-tier' mrezu i mapiranje portova
docker build -t result:1.0 ./result
# TODO: Promeniti apsolutnu putanju!
docker run -d --net front-tier --name result -p 5000:80 -p 5858:5858 -v="/media/todorovic/KINGSTON/ASVSP_Kontejnerizacija/01_Kontejnerizacija_Resenja/01_Docker_Resenja/Zadatak4_Resenje/result:/app" result:1.0
docker network connect back-tier result

# Izgradnja i pokretanje vote kontejnera, uz povezivanje na 'back-tier' i 'front-tier' mrezu i mapiranje portova
docker build -t vote:1.0 ./vote
# TODO: Promeniti apsolutnu putanju!
docker run -d --net front-tier --name vote -p 5001:80 -v="/media/todorovic/KINGSTON/ASVSP_Kontejnerizacija/01_Kontejnerizacija_Resenja/01_Docker_Resenja/Zadatak4_Resenje/vote:/app" vote:1.0
docker network connect back-tier vote

#set +o xtrace






