#!/bin/bash
#Skripta pokretanje resenja zadatka 2

# Komanda pomocu koje se vrsi ispis svake komand pre pokretanja
#set -o xtrace

# Zaustavljanje postojecih kontejnera i brisanje resursa (radi ponovnog pokretanja skripte)
# docker stop $(docker ps -a -q)
# docker rm $(docker ps -a -q)

docker build -t zadatak2:1.0 .
docker run -d --name zadatak2 -p 8085:80 zadatak2:1.0

#set +o xtrace
