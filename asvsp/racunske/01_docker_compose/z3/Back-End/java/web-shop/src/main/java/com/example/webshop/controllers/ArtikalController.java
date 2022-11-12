package com.example.webshop.controllers;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.security.access.annotation.Secured;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

import com.example.webshop.models.dto.ArtikalDTO;
import com.example.webshop.services.ArtikalService;

@RestController
@RequestMapping("api/artikal")
public class ArtikalController {

	private ArtikalService service;

	@Autowired
	public ArtikalController(ArtikalService service) {
		super();
		this.service = service;
	}

	@GetMapping
	public ResponseEntity<List<ArtikalDTO>> get(@RequestParam(required = false) String naziv) {
		if (naziv == null) {
			return new ResponseEntity<List<ArtikalDTO>>(service.findAll(), HttpStatus.OK);
		} else {
			return new ResponseEntity<List<ArtikalDTO>>(service.findByNaziv(naziv), HttpStatus.OK);
		}
	}

	@GetMapping(value = "{id}")
	public ResponseEntity<ArtikalDTO> get(@PathVariable Long id) {
		ArtikalDTO dto = service.findOne(id);
		if (dto == null) {
			return new ResponseEntity<ArtikalDTO>(HttpStatus.NOT_FOUND);
		}
		return new ResponseEntity<ArtikalDTO>(dto, HttpStatus.OK);
	}

	@PostMapping
	@Secured("ROLE_ADMIN")
	public ResponseEntity<ArtikalDTO> post(@RequestBody ArtikalDTO artikal) {
		return new ResponseEntity<ArtikalDTO>(service.save(artikal), HttpStatus.CREATED);
	}

	@PutMapping(value = "{id}")
	@Secured("ROLE_ADMIN")
	public ResponseEntity<ArtikalDTO> put(@RequestBody ArtikalDTO artikal, @PathVariable Long id) {
		ArtikalDTO dto = service.update(artikal, id);
		if (dto == null) {
			return new ResponseEntity<ArtikalDTO>(HttpStatus.NOT_FOUND);
		}
		return new ResponseEntity<ArtikalDTO>(dto, HttpStatus.OK);
	}

	@DeleteMapping(value = "{id}")
	@Secured("ROLE_ADMIN")
	@ResponseBody
	public ResponseEntity<ArtikalDTO> delete(@PathVariable Long id) {
		ArtikalDTO dto = service.delete(id);
		if (dto == null) {
			return new ResponseEntity<ArtikalDTO>(HttpStatus.NOT_FOUND);
		}
		return new ResponseEntity<ArtikalDTO>(dto, HttpStatus.OK);
	}

}
