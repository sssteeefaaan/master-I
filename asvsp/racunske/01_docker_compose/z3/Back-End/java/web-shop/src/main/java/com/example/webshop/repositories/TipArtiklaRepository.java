package com.example.webshop.repositories;

import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

import com.example.webshop.models.TipArtikla;

@Repository
public interface TipArtiklaRepository extends CrudRepository<TipArtikla, Long> {

	TipArtikla findByNaziv(String tipArtikla);

}
