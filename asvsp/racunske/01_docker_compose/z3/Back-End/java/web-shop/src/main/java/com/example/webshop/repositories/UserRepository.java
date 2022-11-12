package com.example.webshop.repositories;

import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

import com.example.webshop.models.User;

@Repository
public interface UserRepository extends CrudRepository<User, Integer> {

	public User findByEmailAndPassword(String email, String pword);
	public User findByEmail(String email);
}
