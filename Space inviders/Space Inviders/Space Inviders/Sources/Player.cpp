/*****************************************************************//**
 * \file Player.cpp
 * \brief
 * Plik z implementacja klasy Player
 *********************************************************************/

#include <array>
#include <chrono>
#include <random>
#include <SFML/Graphics.hpp>

#include "../Includes/Animation.hpp"
#include "../Includes/Global.hpp"
#include "../Includes/Enemy.hpp"
#include "../Includes/Ufo.hpp"
#include "../Includes/Player.hpp"

/// <summary>
/// Konstruktor klasy Player inicjalizuje zmienne, ktore odpowiadaja za stan gracza oraz zaladowanie tekstur dla pociskow i gracza. Tworzona jest zmienna explosion, ktora jest obiektem klasy Animation i jest odpowiedzialna za animacje eksplozji. Wywolywana jest funkcja reset, ktora ustawia poczatkowe wartosci zmiennych odpowiedzialnych za stan gracza. Zaladowywane sa tekstury dla pociskow i gracza z plikow o nazwach "Images/PlayerBullet.png" i "Images/Player.png" odpowiednio. Ustawiana jest tekstura dla obiektow bullet_sprite i sprite. Funkcja reset () ustawia poczatkowe wartosci zmiennych odpowiedzialnych za stan gracza, takie jak pozycja na ekranie, licznik reloadu, timer powerupow itp. Funkcja ta jest wywolywana przy tworzeniu obiektu klasy Player oraz przy resetowaniu rozgrywki.
/// </summary>

Player::Player ():
	explosion (EXPLOSION_ANIMATION_SPEED, BASE_SIZE, "Images/Explosion.png") {
	reset ();

	bullet_texture.loadFromFile ("Images/PlayerBullet.png");
	texture.loadFromFile ("Images/Player.png");
		
	bullet_sprite.setTexture (bullet_texture);
	sprite.setTexture (texture);
}

/// <summary>
/// Funkcja jest getterem klasy Player, ktory zwraca aktualny stan "dead" obiektu Player.
/// </summary>
/// <returns>true jesli gracz jest martwy</returns>
/// <returns>false jesli gracz nie jest martwy</returns>
bool Player::get_dead () const {
	return dead;
}

/// <summary>
/// Funkcja jest getterem klasy Player, ktory zwraca aktualny stan obiektu Player.
/// </summary>
/// <returns>true jesli animacja zostala zakonczona</returns>
/// <returns>false jesli animacja nie zostala zakonczona</returns>
bool Player::get_dead_animation_over () const {
	return dead_animation_over;
}


/// <summary>
/// Funkcja jest getterem klasy Player, ktoy zwraca aktualny poziom power-upu
/// </summary>
/// <returns>0 - brak bonusu </returns>
/// <returns>1 - tarcza </returns>
/// <returns>2 - wielostrzal </returns>
/// <returns>3 - szynkie przeladowanie </returns>
unsigned char Player::get_current_power () const {
	return current_power;
}

/// <summary>
/// Funkcja ta zwraca aktualny stan timera dla powerupa. Timer ten jest uzywany do okreslenia, jak dlugo dany powerup jest aktywny w grze. 
/// </summary>
unsigned short Player::get_power_timer () const {
	return power_timer;
}

/// <summary>
/// Funkcja ta zwraca aktualna pozycje y gracza na ekranie. Wartosc ta jest uzywana do okreslenia pozycji gracza podczas rysowania i kolizji z innymi elementami gry.
/// </summary>
/// <returns>Pozucja y gracza</returns>
unsigned short Player::get_y () const {
	return y;
}

/// <summary>
/// Funkcja "die()" ustawia zmienna "dead" na 1, oznaczajaca, ze gracz jest martwy.
/// </summary>
void Player::die () {
	dead = 1;
}

/// <summary>
/// Funkcja ta odpowiada za rysowanie gracza. Sprawdza, czy gracz jest martwy, jesli nie, to ustawia pozycje i teksture dla gracza, rysuje wszystkie pociski gracza oraz rysuje gracza. Jesli gracz jest martwy, to sprawdza, czy animacja smierci jest skonczona, jesli nie, to rysuje animacje smierci.
/// </summary>
/// <param name="i_window">Okno</param>
void Player::draw (sf::RenderWindow& i_window) {
	if( 0 == dead ) {
		sprite.setPosition (x, y);
		sprite.setTextureRect (sf::IntRect (BASE_SIZE * current_power, 0, BASE_SIZE, BASE_SIZE));

		for( const Bullet& bullet : bullets ) {
			bullet_sprite.setPosition (bullet.x, bullet.y);

			i_window.draw (bullet_sprite);
		}

		i_window.draw (sprite);

		if( 0 == shield_animation_over ) {
			//Kiedy dostaniemy pociskiem a mamy tarcze, tarcza zniknie w niebieskim wybuchu
			explosion.draw (x, y, i_window, sf::Color (0, 109, 255));
		}
	}
	else if( 0 == dead_animation_over ) {
		explosion.draw (x, y, i_window, sf::Color (255, 36, 0));
	}
}

/// <summary>
/// Funkcja ta przywraca gracza do stanu poczatkowego. Ustawia zmienna "dead" na 0, co oznacza, ze gracz nie jest martwy. Ustawia zmienna "dead_animation_over" na 0, co oznacza, ze animacja smierci gracza nie jest jeszcze zakonczona. Ustawia zmienna "shield_animation_over" na 1, co oznacza, ze animacja tarczy gracza jest zakonczona.
/// Ustawia "current_power" na 0, co oznacza, ze gracz nie ma zadnej specjalnej mocy.Ustawia "reload_timer" na 0, co oznacza, ze gracz moze natychmiast wystrzelic pociski.
/// Ustawia "power_timer" na 0, co oznacza, ze czas dzialania specjalnej mocy gracza jest zerowy.Ustawia "x" na polowe szerokosci ekranu minus polowa rozmiaru gracza, co oznacza, ze gracz jest na srodku ekranu poziomo.Ustawia "y" na wysokosc ekranu minus dwa rozmiary gracza, co oznacza, ze gracz jest na dole ekranu.
/// Czysci wektor "bullets", czyli pociski gracza.I resetuje animacje eksplozji.
/// </summary>
void Player::reset () {
	dead = 0;
	dead_animation_over = 0;
	shield_animation_over = 1;

	current_power = 0;
	reload_timer = 0;

	power_timer = 0;
	x = 0.5f * ( SCREEN_WIDTH - BASE_SIZE );
	y = SCREEN_HEIGHT - 2 * BASE_SIZE;

	bullets.clear ();

	explosion.reset ();
}

/// <summary>
/// Funkcja update sprawdza, czy gracz jest martwy lub nie za pomoca zmiennej "dead", jesli nie jest martwy, to sprawdza czy gracz przesuwa sie w lewo lub prawo za pomoca klawiszy A lub D i ustawia odpowiednia pozycje gracza. Nastepnie sprawdza czy gracz moze strzelac i czy zostal nacisniety odpowiedni klawisz, jesli tak, to tworzy nowy pocisk i ustawia odpowiedni timer do przeladowania broni.
/// Funkcja update kontynuuje sprawdzanie kolizji pociskow wrogow z graczem, jesli kolizja zostanie wykryta i gracz nie ma aktywnego power - upu, to jego zmienna "dead" zostanie ustawiona na 1, co oznacza, ze ​​gracz zginal.Jesli gracz ma aktywny power - up, to jego poziom zostanie zmniejszony.
/// Funkcja update sprawdza rowniez kolizje power - upow z graczem i jesli zostanie wykryta, to ustawia odpowiedni power - up dla gracza i ustawia odpowiedni timer dla power - upu.
/// Na koncu funkcja update aktualizuje pozycje pociskow gracza i sprawdza ich kolizje z wrogami.
/// </summary>
/// <param name="i_random_engine">Generator liczb pseudolosowych</param>
/// <param name="i_enemy_bullets">Wektor pociskow przeciwnikow</param>
/// <param name="i_enemies">Wektor przeciwnikow</param>
/// <param name="i_ufo">Obiekt reprezentujacy UFO</param>
/// 
void Player::update (std::mt19937_64& i_random_engine, std::vector<Bullet>& i_enemy_bullets, std::vector<Enemy>& i_enemies, Ufo& i_ufo) {
	if( 0 == dead ) {
		unsigned char powerup_type;

		if( 1 == sf::Keyboard::isKeyPressed (sf::Keyboard::Left) || sf::Keyboard::isKeyPressed (sf::Keyboard::A) ) {


			x = std::max<int> (x - PLAYER_MOVE_SPEED, BASE_SIZE);

		}

		if( 1 == sf::Keyboard::isKeyPressed (sf::Keyboard::Right) || sf::Keyboard::isKeyPressed (sf::Keyboard::D) ) {


			x = std::min<int> (PLAYER_MOVE_SPEED + x, SCREEN_WIDTH - 2 * BASE_SIZE);

		}

		if( 0 == reload_timer ) {
			if( 1 == sf::Keyboard::isKeyPressed (sf::Keyboard::Z) || sf::Keyboard::isKeyPressed (sf::Keyboard::Space) ) {
				if( 2 == current_power ) {
					reload_timer = FAST_RELOAD_DURATION;
				}
				else {
					reload_timer = RELOAD_DURATION;
				}

				bullets.push_back (Bullet (0, -PLAYER_BULLET_SPEED, x, y));

				if( 3 == current_power ) {
					bullets.push_back (Bullet (0, -PLAYER_BULLET_SPEED, x - 0.375f * BASE_SIZE, y));
					bullets.push_back (Bullet (0, -PLAYER_BULLET_SPEED, x + 0.375f * BASE_SIZE, y));
				}
			}
		}
		else {
			reload_timer--;
		}

		for( Bullet& enemy_bullet : i_enemy_bullets ) {
			if( 1 == get_hitbox ().intersects (enemy_bullet.get_hitbox ()) ) {
				if( 1 == current_power ) {
					current_power = 0;

					shield_animation_over = 0;
				}
				else {
					dead = 1;
				}

				enemy_bullet.dead = 1;

				break;
			}
		}

		powerup_type = i_ufo.check_powerup_collision (get_hitbox ());

		if( 0 < powerup_type ) {
			current_power = powerup_type;

			power_timer = POWERUP_DURATION;
		}

		if( 0 == power_timer ) {
			current_power = 0;
		}
		else {
			power_timer--;
		}

		if( 0 == shield_animation_over ) {
			shield_animation_over = explosion.update ();
		}
	}
	else if( 0 == dead_animation_over ) {
		dead_animation_over = explosion.update ();
	}

	for( Bullet& bullet : bullets ) {
		bullet.update ();

		if( 0 == bullet.dead ) {
			if( 1 == i_ufo.check_bullet_collision (i_random_engine, bullet.get_hitbox ()) ) {
				bullet.dead = 1;
			}
		}
	}

	for( Enemy& enemy : i_enemies ) {
		for( Bullet& bullet : bullets ) {
			if( 0 == bullet.dead && 0 < enemy.get_health () && 1 == enemy.get_hitbox ().intersects (bullet.get_hitbox ()) ) {
				bullet.dead = 1;

				enemy.hit ();

				break;
			}
		}
	}

	bullets.erase (remove_if (bullets.begin (), bullets.end (), [] (const Bullet& i_bullet) {
		return 1 == i_bullet.dead;
	}), bullets.end ());
}

/// <summary>
/// Ta funkcja zwraca obiekt klasy sf::IntRect, ktory jest prostokatem zdefiniowanym przez jego lewy gorny rog (x, y) oraz rozmiar w pikselach.
/// Funkcja oblicza wspolrzedne x i y lewego gornego rogu prostokata, dodajac procent wartosci BASE_SIZE do wspolrzednych x i y gracza.Nastepnie ustawia szerokosc i wysokosc prostokata jako wartosc procentowa BASE_SIZE.
/// Zwrocony hitbox sluzy do wykrywania kolizji miedzy graczem a innymi obiektami w grze, takimi jak pociski wroga i ulepszenia.
/// </summary>
/// <returns>Hitbox gracza</returns>
sf::IntRect Player::get_hitbox () const {
	return sf::IntRect (x + 0.125f * BASE_SIZE, y + 0.125f * BASE_SIZE, 0.75f * BASE_SIZE, 0.75f * BASE_SIZE);
}