/*****************************************************************//**
 * \file Animation.cpp
 * \brief
 * Plik z implementacja klasy Animation
 *********************************************************************/


#include <SFML/Graphics.hpp>

#include "../Includes/Animation.hpp"

/// <summary>
/// Konstruktor klasy Animation przyjmuje parametry predkosci animacji, szerokosci klatki oraz lokalizacji tekstury. Wczytuje teksture z pliku i oblicza liczbe klatek na podstawie szerokosci tekstury i szerokosci klatki.
/// </summary>
/// <param name="i_animation_speed">predkosc animacji</param>
/// <param name="i_frame_width">szerokosc klatki</param>
/// <param name="i_texture_location">lokalizacja tekstury</param>
Animation::Animation (unsigned short i_animation_speed, unsigned short i_frame_width, const std::string& i_texture_location):
	animation_iterator (0),
	animation_speed (std::max<unsigned short> (1, i_animation_speed)),
	current_frame (0),
	frame_width (i_frame_width) {
	texture.loadFromFile (i_texture_location);

	total_frames = texture.getSize ().x / frame_width;
}


/// <summary>
/// Metoda change_current_frame() zwieksza aktualnie wyswietlana klatke o 1. Jesli aktualna klatka jest ostatnia klatka animacji to zmienia ja na pierwsza i zwraca true, w przeciwnym wypadku zwraca false
/// </summary>
/// <returns>true - animacja zakonczona</returns>
/// <returns>return false - animacja nie zakonczona</returns>
bool Animation::change_current_frame () {
	current_frame++;

	if( current_frame == total_frames ) {
		current_frame = 0;

		return 1;
	}

	return 0;
}


/// <summary>
/// Metoda update() aktualizuje animacje, zwiekszajac iterator o 1. Jesli iterator przekroczy predkosc animacji, zwieksza aktualnie wyswietlana klatke o 1. Jesli aktualna klatka jest ostatnia klatka animacji to zmienia ja na pierwsza i zwraca true, w przeciwnym wypadku zwraca false
/// </summary>
/// <returns>return true - animacja zakonczona</returns>
/// <returns>return false - animacja nie zakonczona</returns>
bool Animation::update () {
	bool output = 0;

	animation_iterator++;

	while( animation_iterator >= animation_speed ) {
		animation_iterator -= animation_speed;
		current_frame++;

		if( current_frame == total_frames ) {
			output = 1;

			current_frame = 0;
		}
	}

	return output;
}


/// <summary>
/// Metoda draw() rysuje aktualnie wyswietlana klatke na ekranie. Ustawia pozycje, kolor i klatke tekstury sprite'a na podstawie aktualnej klatki. Rysuje sprite w podanym oknie renderowania.
/// </summary>
/// <param name="i_x">pozycja x na ekranie</param>
/// <param name="i_y">pozycja y na ekranie</param>
/// <param name="i_window">okno renderowania</param>
/// <param name="i_color">kolor klatki</param>
void Animation::draw (short i_x, short i_y, sf::RenderWindow& i_window, const sf::Color& i_color) {
	sprite.setColor (i_color);
	sprite.setPosition (i_x, i_y);
	sprite.setTexture (texture);
	sprite.setTextureRect (sf::IntRect (current_frame * frame_width, 0, frame_width, texture.getSize ().y));

	i_window.draw (sprite);
}

/// <summary>
/// Metoda reset() resetuje animacje do stanu poczatkowego, ustawiajac iterator na 0 oraz aktualna klatke na 0.
/// </summary>
void Animation::reset () {
	animation_iterator = 0;
	current_frame = 0;
}