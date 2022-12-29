<h1>Pokretanje okruzenja</h1>
<ul>
    <li>Izvršiti naredbu <code>sudo sysctl -w vm.max_map_count=262144<code></li>
    <li>Pokrenuti skriptu <code>./download_dataset.sh<code></li>
    <li>Izvrsiti naredbu <code>docker-compose up</code></li>
    <li>Izvrsiti naredbu <code>docker exec -it namenode /home/projekat/batch-dataset/load.sh</code>
</ul>
