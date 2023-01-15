#pragma once

/*****************************************************************//**
 * \file DrawText.hpp
 * \brief
 * Plik naglowkowy funkcji DrawText
 *********************************************************************/

/// <summary>
/// Deklaracja funkcji o nazwie "draw_text". Funkcja przyjmuje 5 argumentow.
/// </summary>
/// <param name="i_x">Wspolrzedna x, gdzie tekst powinien zostac narysowany.</param>
/// <param name="i_y"> Wspolrzedna y, gdzie tekst powinien zostac narysowany.</param>
/// <param name="i_text">Tekst, ktory ma zostac narysowany.</param>
/// <param name="i_window"> Obiekt klasy sf::RenderWindow, czyli okno, na ktorym ma zostac narysowany tekst.</param> 
/// <param name="i_font_texture">Obiekt klasy sf::Texture, czyli tekstura zawierajaca czcionke, ktora ma zostac uzyta do narysowania tekstu.</param>

void draw_text(short i_x, short i_y, const std::string& i_text, sf::RenderWindow& i_window, const sf::Texture& i_font_texture);