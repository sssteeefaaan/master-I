package com.example.webshop.repositories;

import java.util.List;

import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

import com.example.webshop.models.KupljeniArtikal;
import com.example.webshop.models.User;

@Repository
public interface KupljeniArtikalRepository extends CrudRepository<KupljeniArtikal, Long> {

	List<KupljeniArtikal> findByKupac(User kupac);
}
