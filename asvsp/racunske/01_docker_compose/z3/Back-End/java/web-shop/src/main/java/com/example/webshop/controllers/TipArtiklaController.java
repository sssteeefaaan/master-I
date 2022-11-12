package com.example.webshop.controllers;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.example.webshop.models.TipArtikla;
import com.example.webshop.services.TipArtiklaService;

@RestController
@RequestMapping("api/tipArtikla")
public class TipArtiklaController {
	
	private TipArtiklaService service;

	@Autowired
	public TipArtiklaController(TipArtiklaService service) {
		super();
		this.service = service;
	}

	@GetMapping
	public ResponseEntity<List<TipArtikla>> get() {
		return new ResponseEntity<List<TipArtikla>>(service.findAll(), HttpStatus.OK);
	}
	
	@GetMapping(value = "{id}")
	public ResponseEntity<TipArtikla> get(@PathVariable Long id) {
		TipArtikla tipArtikla = service.findOne(id);
		if (tipArtikla == null) {
			return new ResponseEntity<TipArtikla>(HttpStatus.NOT_FOUND);
		}
		return new ResponseEntity<TipArtikla>(tipArtikla, HttpStatus.OK);
	}

}
