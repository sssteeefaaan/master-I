package com.example.webshop.services;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.example.webshop.models.TipArtikla;
import com.example.webshop.repositories.TipArtiklaRepository;

@Service
public class TipArtiklaService {

	private TipArtiklaRepository repository;

	@Autowired
	public TipArtiklaService(TipArtiklaRepository repository) {
		super();
		this.repository = repository;
	}

	public List<TipArtikla> findAll() {
		return (List<TipArtikla>) repository.findAll();
	}

	public TipArtikla findOne(Long id) {
		return repository.findOne(id);
	}

}
