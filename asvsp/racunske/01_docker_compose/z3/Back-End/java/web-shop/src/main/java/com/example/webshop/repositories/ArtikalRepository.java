package com.example.webshop.repositories;

import java.util.List;

import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import com.example.webshop.models.Artikal;

@Repository
public interface ArtikalRepository extends CrudRepository<Artikal, Long> {

	@Query("SELECT a FROM Artikal a where lower(a.naziv) like lower(concat('%', :naziv,'%'))")
	List<Artikal> searchByNaziv(@Param(value = "naziv") String naziv);
	
}
