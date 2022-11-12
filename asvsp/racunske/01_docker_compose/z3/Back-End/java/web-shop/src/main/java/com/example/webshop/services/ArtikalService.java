package com.example.webshop.services;

import java.util.List;
import java.util.stream.Collectors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.example.webshop.models.Artikal;
import com.example.webshop.models.TipArtikla;
import com.example.webshop.models.dto.ArtikalDTO;
import com.example.webshop.repositories.ArtikalRepository;
import com.example.webshop.repositories.TipArtiklaRepository;

@Service
public class ArtikalService {

	private ArtikalRepository artikalRepository;
	private TipArtiklaRepository tipArtiklaRepository;

	@Autowired
	public ArtikalService(ArtikalRepository artikalRepository, TipArtiklaRepository tipArtiklaRepository) {
		super();
		this.artikalRepository = artikalRepository;
		this.tipArtiklaRepository = tipArtiklaRepository;
	}

	public ArtikalDTO save(ArtikalDTO artikalDTO) {
		TipArtikla tipArtikla = tipArtiklaRepository.findByNaziv(artikalDTO.getTipArtikla());
		Artikal artikal = artikalDTO.convert();
		artikal.setTipArtikla(tipArtikla);

		return new ArtikalDTO(artikalRepository.save(artikal));
	}

	public ArtikalDTO update(ArtikalDTO artikalDTO, Long id) {
		if (artikalRepository.findOne(id) == null) {
			return null;
		}

		TipArtikla tipArtikla = tipArtiklaRepository.findByNaziv(artikalDTO.getTipArtikla());
		Artikal artikal = artikalDTO.convert();
		artikal.setId(id);
		artikal.setTipArtikla(tipArtikla);

		return new ArtikalDTO(artikalRepository.save(artikal));
	}

	public List<ArtikalDTO> findAll() {
		return ((List<Artikal>) artikalRepository.findAll()).stream().map(artikal -> new ArtikalDTO(artikal))
				.collect(Collectors.toList());
	}

	public ArtikalDTO delete(Long id) {
		Artikal artikal = artikalRepository.findOne(id);

		if (artikal == null) {
			artikalRepository.delete(id);
			return new ArtikalDTO(artikal);
		}

		return null;
	}

	public ArtikalDTO findOne(Long id) {
		Artikal artikal = artikalRepository.findOne(id);
		return artikal != null ? new ArtikalDTO(artikal) : null;
	}

	public List<ArtikalDTO> findByNaziv(String naziv) {
		return artikalRepository.searchByNaziv(naziv).stream().map(artikal -> new ArtikalDTO(artikal))
				.collect(Collectors.toList());
	}

}
