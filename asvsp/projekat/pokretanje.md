<h1>Pokretanje okruženja</h1>
<ol>
    <li>Izvršiti naredbu</br><code>sudo sysctl -w vm.max_map_count=262144</code></li>
    <li>Pokrenuti skriptu</br><code>./download_dataset.sh</code></li>
    <li>Izvršiti naredbu</br><code>docker-compose up</code></li>
    <li>Izvršiti naredbu</br><code>docker exec -it namenode /home/projekat/batch-dataset/load.sh</code></li>
</ol>
