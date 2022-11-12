package com.example.webshop.controllers;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.security.access.annotation.Secured;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.example.webshop.models.KupljeniArtikal;
import com.example.webshop.models.dto.ArtikalDTO;
import com.example.webshop.services.KupljeniArtikalService;

@RestController
@RequestMapping("api/korpa")
public class KorpaController {
	
	private KupljeniArtikalService service;
	
	@Autowired
	public KorpaController(KupljeniArtikalService service) {
		super();
		this.service = service;
	}

	@GetMapping
	@Secured("ROLE_KUPAC")
	public ResponseEntity<List<ArtikalDTO>> get() {
		String username = SecurityContextHolder.getContext().getAuthentication().getName();
		return new ResponseEntity<>(service.findAll(username), HttpStatus.OK);
	}
	
	@PostMapping
	@Secured("ROLE_KUPAC")
	public ResponseEntity<KupljeniArtikal> post(@RequestBody ArtikalDTO artikalDto) {
		String username = SecurityContextHolder.getContext().getAuthentication().getName();
		return new ResponseEntity<>(service.dodajUKorpu(username, artikalDto), HttpStatus.OK);
	}
}
