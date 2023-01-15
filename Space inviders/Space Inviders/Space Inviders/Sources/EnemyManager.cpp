/*****************************************************************//**
 * \file EnemyMenager.cpp
 * \brief
 * Plik z implementacja klasy EnemyMenager
 *********************************************************************/


#define NOMINMAX
#include <iostream>
#include <windows.h>
#include <array>
#include <chrono>
#include <random>
#include <SFML/Graphics.hpp>

#include "../Includes/Animation.hpp"
#include "../Includes/Global.hpp"
#include "../Includes/Enemy.hpp"
#include "../Includes/EnemyManager.hpp"
//#include "../Includes/Player.hpp"

/// <summary>
/// Jest to konstruktor klasy. Tworzy obiekt klasy i inicjalizuje wartosci poczatkowe. Inicjalizowane sa zmienne i obiekty klasy, takie jak obiekt klasy "Animation" i obiekt "sf::Sprite" odpowiedzialny za rysowanie pociskow wrogow.
/// </summary>
EnemyManager::EnemyManager ():
	shoot_distribution (0, ENEMY_SHOOT_CHANCE) {
	reset (0);

	enemy_bullet_texture.loadFromFile ("Images/EnemyBullet.png");

	enemy_bullet_sprite.setTexture (enemy_bullet_texture);

	for( unsigned char a = 0; a < ENEMY_TYPES; a++ ) {
		enemy_animations.push_back (Animation (1 + move_pause, BASE_SIZE, "Images/Enemy" + std::to_string (static_cast<unsigned short>( a )) + ".png"));
	}
}

/// <summary>
/// Ta funkcja sprawdza czy przeciwnicy dotarli do gracza. Funkcja przyjmuje pozycje gracza jako argument i zwraca true, jesli przeciwnicy dotarli do gracza lub false jesli nie dotarli.
/// </summary>
/// <param name="i_player_y">Pozycja gracza</param>
/// <returns>True jesli przeciwnicy dotarli do gracza</returns>
bool EnemyManager::reached_player (unsigned short i_player_y) const {
	for( const Enemy& enemy : enemies ) {
		if( enemy.get_y () > i_player_y - 0.5f * BASE_SIZE ) {
			//Jesli przeciwnicy dotkna gracza gra zostaje przerwana
			return 1;
		}
	}

	return 0;
}


/// <summary>
/// Funkcja jest odpowiedzialna za rysowanie wszystkich pociskow i przeciwnikow w grze. W petli jest przegladana tablica enemy_bullets i kazdy pocisk jest rysowany na ekranie. Nastepnie jest przegladana tablica enemies, a kazdy przeciwnik jest rysowany na ekranie. Jesli przeciwnik zostanie trafiony, zostanie wyswietlony na bialo przez okreslony czas. Kolory przeciwnikow sa rowniez ustawiane w zaleznosci od ich typu. 
/// </summary>
/// <param name="i_window">Okno na ktorym rysujemy</param>
void EnemyManager::draw (sf::RenderWindow& i_window) {
	for( const Bullet& bullet : enemy_bullets ) {
		//Rysowanie pociskow
		enemy_bullet_sprite.setPosition (bullet.x, bullet.y);
		enemy_bullet_sprite.setTextureRect (sf::IntRect (BASE_SIZE * bullet.previous_x.size (), 0, BASE_SIZE, BASE_SIZE));

		i_window.draw (enemy_bullet_sprite);
	}

	for( Enemy& enemy : enemies ) {
		//Kiedy przeciwnik dostanie pociskiem staje sie na kilka klatek bialy
		sf::Color enemy_color = sf::Color (255, 255, 255);

		

		if( 0 == enemy.get_hit_timer () ) {
			switch( enemy.get_type () ) {
				case 0:
				{
					enemy_color = sf::Color (255, 68, 13);

					break;
				}
				case 1:
				{
					enemy_color = sf::Color (255, 168, 25);

					break;
				}
				case 2:
				{
					enemy_color = sf::Color (183, 0, 255);

					break;
				}

				case 3:
				{
					enemy_color = sf::Color (0, 255, 184);

					break;
				}

				case 4:
				{
					enemy_color = sf::Color (0, 132, 255);

				}
			}
		}

		enemy_animations[enemy.get_type ()].draw (enemy.get_x (), enemy.get_y (), i_window, enemy_color);
	}
}

/// <summary>
/// Funkcja odpowiedzialna za ustawienie wszystkich zmiennych i elementow na poczatkowe wartosci. Jest wywolywana na poczatku gry lub po przejsciu na kolejny poziom. Ta funkcja resetuje poziom gry a takze odpowiada za rozklad przeciwnikow na kolejnych poziomach.
/// </summary>
/// <param name="i_level">Numer poziomu</param>

void EnemyManager::reset (unsigned short i_level) {
	unsigned char enemy_x = 0;
	unsigned char enemy_y = 0;

	std::string level_sketch = "";

	move_pause = std::max<short> (ENEMY_MOVE_PAUSE_START_MIN, ENEMY_MOVE_PAUSE_START - ENEMY_MOVE_PAUSE_DECREASE * i_level);
	move_timer = move_pause;

	shoot_distribution = std::uniform_int_distribution<unsigned short> (0, std::max<short> (ENEMY_SHOOT_CHANCE_MIN, ENEMY_SHOOT_CHANCE - ENEMY_SHOOT_CHANCE_INCREASE * i_level));

	for( Animation& enemy_animation : enemy_animations ) {
		enemy_animation.reset ();
	}

	enemy_bullets.clear ();

	enemies.clear ();

	switch( i_level ) {

		case 0:
		{
			level_sketch = "0 0 0 0 0 0 0 0\n"
				           "0             0\n"
				           "0 0 0 0 0 0 0 0";
			break;
		}
		case 1:
		{
			level_sketch = "0 0 0 0 0 0 0 0 \n"
				            "0 0 0 0 0 0 0 0\n"
				           "0 0 0 0 0 0 0 0 \n" 
				            "0 0 0 0 0 0 0 0";
			break;
		}
		case 2:
		{
			level_sketch = "0000000000000000\n"
				           "0000000000000000\n"
				           "0000000000000000\n"
				           "0000000000000000";
			break;
		}

		case 3:
		{
			level_sketch = "1010101010101010\n"
				           "0000000000000000\n"
				           "0000000000000000\n"
				           "0000000000000000";
			break;
		}

		case 4:
		{
			level_sketch = "1010101010101010\n"
						   "0101010101010101\n"
				           "1010101010101010\n"
				           "0101010101010101";
			break;
		}

		case 5:
		{
			level_sketch = "1111111111111111\n"
				           "0101010101010101\n"
				           "1111111111111111\n"
				           "0101010101010101";
			break;
		}

		case 6:
		{
			level_sketch = "1111111111111111\n"
				           "1111111111111111\n"
				           "1111111111111111\n"
				           "1111111111111111";
			break;
		}

		case 7:
		{
			level_sketch = "2020202020202020\n"
				           "1111111111111111\n"
				           "1010101010101010\n"
				           "0000000000000000";
			break;
		}

		case 8:
		{
			level_sketch = "2222222222222222\n"
				           "1121212121212122\n"
				           "1010101010101010\n"
				           "0101010101010101";
			break;
		}

		case 9:
		{
			level_sketch = "0000000000000000\n"
				           "2222222222222222\n"
				           "1111111111111111\n"
				           "1111111111111111";
			break;
		}

		case 10:
		{
			level_sketch = "1212121212121212\n"
				           "1111111111111111\n"
				           "1212121212121212\n"
				           "1111111111111111";
			break;
		}

		case 11:
		{
			level_sketch = "2222222222222222\n"
				           "2222222222222222\n"
				           "2222222222222222\n"
				           "2222222222222222";
		}

		case 12:
		{
			level_sketch = "3030303030303030\n"
				           "0120120120120120\n"
				           "1201201201201202\n"
				           "1201201201201201";
		}

		case 13:
		{
			level_sketch = "3333333333333333\n"
				           "3232323232323232\n"
				           "2222222222222222\n"
				           "1111111111111111";
		}

		case 14:
		{
			level_sketch = "3333333333333333\n"
				           "2222222222222222\n"
				           "3333333333333333\n"
				           "3333333333333333";
		}

		case 15:
		{
			level_sketch = "3333333333333333\n"
				           "3333333333333333\n"
				           "3333333333333333\n"
				           "3333333333333333";
		}

		case 16:
		{
			level_sketch = "4040404040404040\n"
				           "2222222222222222\n"
				           "1212121212121212\n"
				           "0101010101010101";
		}

		case 17:
		{
			level_sketch = "4444444444444444\n"
				           "2323232323232323\n"
				           "1212121212121212\n"
				           "1111111111111111";
		}

		case 18:
		{
			level_sketch = "4444444444444444\n"
				           "3333333333333333\n"
				           "2121121212121212\n"
				           "1111111111111111";
		}

		case 19:
		{
			level_sketch = "1111111111111111\n"
				           "4343434343434343\n"
				           "3232132323232322\n"
				           "4343434343434343";
		}

		case 20:
		{
			level_sketch = "4444444444444444\n"
				           "4444444444444444\n"
				           "4444444444444444\n"
				           "4444444444444444";
		}
	}

	//Tu zamieniamy znaki na przeciwnikow
	for( char sketch_character : level_sketch ) {
		enemy_x++;

		switch( sketch_character ) {
			case '\n':
			{
				enemy_x = 0;
				enemy_y++;

				break;
			}
			case '0':
			{
				enemies.push_back (Enemy (0, BASE_SIZE * ( 1 + enemy_x ), BASE_SIZE * ( 2 + enemy_y )));

				break;
			}
			case '1':
			{
				enemies.push_back (Enemy (1, BASE_SIZE * ( 1 + enemy_x ), BASE_SIZE * ( 2 + enemy_y )));

				break;
			}
			case '2':
			{
				enemies.push_back (Enemy (2, BASE_SIZE * ( 1 + enemy_x ), BASE_SIZE * ( 2 + enemy_y )));

				break;
			}

			case '3':
			{
				enemies.push_back (Enemy (3, BASE_SIZE * ( 1 + enemy_x ), BASE_SIZE * ( 2 + enemy_y )));

				break;

			}

			case '4':
			{
				enemies.push_back (Enemy (4, BASE_SIZE * ( 1 + enemy_x ), BASE_SIZE * ( 2 + enemy_y )));
			}
		}
	}
}

/// <summary>
/// Ta funkcja aktualizuje pozycje przeciwnikow i pociskow. Sprawdza takze, czy gracz zostal trafiony przez pocisk przeciwnika.
/// </summary>
/// <param name="i_random_engine">Silnik losowosci</param>

void EnemyManager::update (std::mt19937_64& i_random_engine) {
	std::vector<Enemy>::iterator dead_enemies_start;

	if( 0 == move_timer ) {
		move_timer = move_pause;

		for( Enemy& enemy : enemies ) {
			enemy.move ();
		}

		for( Animation& enemy_animation : enemy_animations ) {
			//Zmiana klatki przeciwnika po kazdym ruchu
			enemy_animation.change_current_frame ();
		}
	}
	else {
		move_timer--;
	}

	for( Enemy& enemy : enemies ) {
		enemy.update ();

		if( 0 == shoot_distribution (i_random_engine) ) {
			enemy.shoot (enemy_bullets);
		}
	}

	dead_enemies_start = remove_if (enemies.begin (), enemies.end (), [] (const Enemy& i_enemy) {
		return 0 == i_enemy.get_health ();
									});

	//Im wiecej przeciwnikow zniszczymy tym szybciej reszta bedzie sie poruszac
	move_pause = std::max<int> (ENEMY_MOVE_PAUSE_MIN, move_pause - ENEMY_MOVE_PAUSE_DECREASE * ( enemies.end () - dead_enemies_start ));

	enemies.erase (dead_enemies_start, enemies.end ());

	for( Bullet& enemy_bullet : enemy_bullets ) {
		enemy_bullet.update ();
	}

	enemy_bullets.erase (remove_if (enemy_bullets.begin (), enemy_bullets.end (), [] (const Bullet& i_bullet) {
		return 1 == i_bullet.dead;
	}), enemy_bullets.end ());
}

/// <summary>
/// Funkcja zwracajaca referencje do wektora przechowujacego pociski przeciwnikow, aby miec dostep do niego w innych klasach.
/// </summary>
/// <returns>Referencja do wektora pociskow przeciwnikow</returns>
std::vector<Bullet>& EnemyManager::get_enemy_bullets () {
	return enemy_bullets;
}

/// <summary>
/// Runkcja zwracajaca referencje do wektora przechowujacego przeciwnikow, aby miec dostep do niego w innych klasach.
/// </summary>
/// <returns>Referencja do przeciwnikow</returns>
std::vector<Enemy>& EnemyManager::get_enemies () {
	return enemies;
}