package com.example.webshop.models;

import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.ManyToOne;

@Entity
public class KupljeniArtikal {

	@Id
	@GeneratedValue(strategy = GenerationType.AUTO)
	private Long id;
	private Long kolicina;
	
	@ManyToOne
	private User kupac;
	
	@ManyToOne
	private Artikal artikal;

	public Long getId() {
		return id;
	}

	public void setId(Long id) {
		this.id = id;
	}

	public Long getKolicina() {
		return kolicina;
	}

	public void setKolicina(Long kolicina) {
		this.kolicina = kolicina;
	}

	public User getKupac() {
		return kupac;
	}

	public void setKupac(User kupac) {
		this.kupac = kupac;
	}

	public Artikal getArtikal() {
		return artikal;
	}

	public void setArtikal(Artikal artikal) {
		this.artikal = artikal;
	}
	
}
