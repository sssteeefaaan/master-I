#!/bin/bash
#Skripta za pokretanje resenja zadatka 3

# Komanda pomocu koje se vrsi ispis svake komand pre pokretanja
#set -o xtrace

# Zaustavljanje postojecih kontejnera i brisanje resursa (radi ponovnog pokretanja skripte)
# docker stop $(docker ps -a -q)
# docker rm $(docker ps -a -q)

docker network create webnet

docker build -t zadatak3:1.0 .
docker run -d --net webnet --name zadatak3 -p 8085:80 zadatak3:1.0

docker run -d --net webnet -v="/home/docker/data:/data" --name redis -p 6379:6379 redis:6.2.5-alpine

#set +o xtrace
