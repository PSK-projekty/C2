/*****************************************************************//**
 * \file DrawText.cpp
 * \brief
 * Plik z implementacja funkcji DrawText
 *********************************************************************/


#include <SFML/Graphics.hpp>

#include "../Includes/DrawText.hpp"

/// <summary>
/// Funkcja uzywa wlasciwosci tekstury fontu, aby okreslic wymiary pojedynczego znaku, a nastepnie ustawia odpowiednie prostokatne obszary tekstury dla kazdego znaku w ciagu tekstu, aby pobrac odpowiedni obraz znaku z tekstury. Nastepnie ustawia pozycje sprite'a na pozycje x i y, gdzie dany znak ma zostac wyswietlony i rysuje go na ekranie za pomoca okna renderujacego. Po wyswietleniu kazdego znaku, pozycja x jest zwiekszana o szerokosc jednego znaku, aby umozliwic wyswietlenie kolejnego znaku obok poprzedniego. Funkcja konczy dzialanie po przejsciu przez caly ciag tekstu i wyswietleniu wszystkich znakow.
/// </summary>
/// <param name="i_x">Wspolrzedna x, gdzie tekst powinien zostac narysowany.</param>
/// <param name="i_y"> Wspolrzedna y, gdzie tekst powinien zostac narysowany.</param>
/// <param name="i_text">Tekst, ktory ma zostac narysowany.</param>
/// <param name="i_window"> Obiekt klasy sf::RenderWindow, czyli okno, na ktorym ma zostac narysowany tekst.</param> 
/// <param name="i_font_texture">Obiekt klasy sf::Texture, czyli tekstura zawierajaca czcionke, ktora ma zostac uzyta do narysowania tekstu.</param>

void draw_text (short i_x, short i_y, const std::string& i_text, sf::RenderWindow& i_window, const sf::Texture& i_font_texture) {
	short character_x = i_x;
	short character_y = i_y;

	unsigned char character_height = i_font_texture.getSize ().y;
	unsigned char character_width = i_font_texture.getSize ().x / 96;

	sf::Sprite character_sprite (i_font_texture);

	for( std::string::const_iterator a = i_text.begin (); a != i_text.end (); a++ ) {
		if( '\n' == *a ) {
			character_x = i_x;
			character_y += character_height;

			continue;
		}

		character_sprite.setPosition (character_x, character_y);
		character_sprite.setTextureRect (sf::IntRect (character_width * ( *a - 32 ), 0, character_width, character_height));

		character_x += character_width;

		i_window.draw (character_sprite);
	}
}