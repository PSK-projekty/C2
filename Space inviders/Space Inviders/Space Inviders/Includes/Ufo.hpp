#pragma once

/*****************************************************************//**
 * \file Ufo.hpp
 * \brief
 * Plik naglowkowy klasy Ufo
 *********************************************************************/

/// <summary>
/// Klasa zawiera kilka zmiennych skladowych, takich jak „dead”, ktore wskazuje, czy UFO jest obecnie martwe, „dead_animation_over”, ktore wskazuje, czy animacja smierci UFO zostala zakonczona, „explosion_x” i „x”, ktore sa wspolrzednymi x odpowiednio dla eksplozji i UFO, „timer” i „y”, ktore sa licznikiem czasu i wspolrzedna y dla UFO, „powerup_distribution” i „timer_distribution”, ktore sa obiektami klasy „std::uniform_int_distribution” uzywanej do generowania liczb losowych , „powerup_animations” i „powerups”, ktore sa wektorami do przechowywania odpowiednio animacji i ulepszen UFO, „animation” i „explosion”, ktore sa obiektami klasy „Animation”, ktore sa uzywane do animowania UFO i jego eksplozji .
/// </summary>
class Ufo{
	bool dead;
	bool dead_animation_over;

	short explosion_x;
	short x;

	unsigned short timer;
	unsigned short y;

	std::uniform_int_distribution<unsigned short> powerup_distribution;
	std::uniform_int_distribution<unsigned short> timer_distribution;

	std::vector<Animation> powerup_animations;

	std::vector<Powerup> powerups;

	Animation animation;
	Animation explosion;
public:
	/// <summary>
	/// Konstruktor klasy Ufo
	/// </summary>
	/// <param name="i_random_engine">Generator liczb pseudolosowych</param>
	Ufo(std::mt19937_64& i_random_engine);

	/// <summary>
	/// Sprawdza, czy pocisk trafil w UFO
	/// </summary>
	/// <param name="i_random_engine">Generator liczb pseudolosowych</param>
	/// <param name="i_bullet_hitbox">Hitbox pocisku</param>
	bool check_bullet_collision(std::mt19937_64& i_random_engine, const sf::IntRect& i_bullet_hitbox);

	/// <summary>
	/// Sprawdza, gracz zlapal bonus
	/// </summary>
	/// <param name="i_player_hitbox">Hitbox gracza</param>
	unsigned char check_powerup_collision(const sf::IntRect& i_player_hitbox);

	/// <summary>
	/// Funkcja rysuje Ufo
	/// </summary>
	/// <param name="i_window">Okno</param>
	void draw(sf::RenderWindow& i_window);

	/// <summary>
	/// Funkcja odpowiada za resetowanie stanu Ufo. 
	/// </summary>
	/// <param name="i_dead">Zmienna przechowujaca informacje czy Ufo zyje</param>
	/// <param name="i_random_engine">Generator liczb pseudolosowych</param>
	void reset(bool i_dead, std::mt19937_64& i_random_engine);

	/// <summary>
	/// Aktualizowanie stanu Ufo
	/// </summary>
	/// <param name="i_random_engine">Generator liczb pseudolosowych</param>
	void update(std::mt19937_64& i_random_engine);

	/// <summary>
	/// Zwraca Hitbox Ufo
	/// </summary>
	/// <returns>Hibox Ufo</returns>
	sf::IntRect get_hitbox() const;
};