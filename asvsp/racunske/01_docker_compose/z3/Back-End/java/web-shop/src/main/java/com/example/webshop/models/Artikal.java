package com.example.webshop.models;

import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.ManyToOne;

@Entity
public class Artikal {

	@Id
	@GeneratedValue(strategy = GenerationType.AUTO)
	private Long id;
	private String sifra;
	private String naziv;
	private Double cena;
	private String opis;
	private String slikaUrl;

	@ManyToOne
	private TipArtikla tipArtikla;

	public Long getId() {
		return id;
	}

	public void setId(Long id) {
		this.id = id;
	}

	public String getSifra() {
		return sifra;
	}

	public void setSifra(String sifra) {
		this.sifra = sifra;
	}

	public String getNaziv() {
		return naziv;
	}

	public void setNaziv(String naziv) {
		this.naziv = naziv;
	}

	public Double getCena() {
		return cena;
	}

	public void setCena(Double cena) {
		this.cena = cena;
	}

	public String getOpis() {
		return opis;
	}

	public void setOpis(String opis) {
		this.opis = opis;
	}

	public String getSlikaUrl() {
		return slikaUrl;
	}

	public void setSlikaUrl(String slikaUrl) {
		this.slikaUrl = slikaUrl;
	}

	public TipArtikla getTipArtikla() {
		return tipArtikla;
	}

	public void setTipArtikla(TipArtikla tipArtikla) {
		this.tipArtikla = tipArtikla;
	}

}
