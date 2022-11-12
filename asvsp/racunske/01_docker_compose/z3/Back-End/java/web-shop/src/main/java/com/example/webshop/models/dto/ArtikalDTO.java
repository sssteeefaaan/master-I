package com.example.webshop.models.dto;

import com.example.webshop.models.Artikal;

public class ArtikalDTO {

	private Long id;
	private String sifra;
	private String naziv;
	private Double cena;
	private String opis;
	private String slikaUrl;
	private String tipArtikla;

	public ArtikalDTO() {
	}
	

	public ArtikalDTO(Long id, String sifra, String naziv, Double cena, String opis, String slikaUrl,
			String tipArtikla) {
		super();
		this.id = id;
		this.sifra = sifra;
		this.naziv = naziv;
		this.cena = cena;
		this.opis = opis;
		this.slikaUrl = slikaUrl;
		this.tipArtikla = tipArtikla;
	}

	public ArtikalDTO(Artikal artikal) {
		this.id = artikal.getId();
		this.sifra = artikal.getSifra();
		this.naziv = artikal.getNaziv();
		this.cena = artikal.getCena();
		this.opis = artikal.getOpis();
		this.slikaUrl = artikal.getSlikaUrl();
		this.tipArtikla = artikal.getTipArtikla() == null ? null : artikal.getTipArtikla().getNaziv();
	}

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

	public String getTipArtikla() {
		return tipArtikla;
	}

	public void setTipArtikla(String tipArtikla) {
		this.tipArtikla = tipArtikla;
	}

	public Artikal convert() {
		Artikal artikal = new Artikal();

		artikal.setNaziv(this.getNaziv());
		artikal.setOpis(this.getOpis());
		artikal.setSifra(this.getSifra());
		artikal.setCena(this.getCena());
		artikal.setSlikaUrl(this.getSlikaUrl());

		return artikal;
	}

}
