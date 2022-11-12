package com.example.webshop.services;

import java.util.List;
import java.util.stream.Collectors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.example.webshop.models.Artikal;
import com.example.webshop.models.KupljeniArtikal;
import com.example.webshop.models.User;
import com.example.webshop.models.dto.ArtikalDTO;
import com.example.webshop.repositories.ArtikalRepository;
import com.example.webshop.repositories.KupljeniArtikalRepository;
import com.example.webshop.repositories.UserRepository;

@Service
public class KupljeniArtikalService {

	private KupljeniArtikalRepository repository;
	private UserRepository userRepository;
	private ArtikalRepository artikalRepository;

	@Autowired
	public KupljeniArtikalService(KupljeniArtikalRepository repository, UserRepository userRepository, ArtikalRepository artikalRepository) {
		super();
		this.repository = repository;
		this.userRepository = userRepository;
		this.artikalRepository = artikalRepository;
	}

	public KupljeniArtikal dodajUKorpu(String username, ArtikalDTO artikalDto) {
		User user = userRepository.findByEmail(username);
		Artikal artikal = artikalRepository.findOne(artikalDto.getId());

		KupljeniArtikal kupljeniArtikal = new KupljeniArtikal();
		kupljeniArtikal.setArtikal(artikal);
		kupljeniArtikal.setKupac(user);
		kupljeniArtikal.setKolicina(1L);

		return repository.save(kupljeniArtikal);
	}

	public List<ArtikalDTO> findAll(String username) {
		User user = userRepository.findByEmail(username);
		return repository.findByKupac(user).stream()
				.map(kupljeniArtikal -> new ArtikalDTO(kupljeniArtikal.getArtikal())).collect(Collectors.toList());
	}
}
