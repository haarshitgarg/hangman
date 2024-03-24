#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/VideoMode.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800,600), "test_window");
    sf::Text text;
    sf::Font font;
    if(!font.loadFromFile("/Users/harshitgarg/CPP-projects/hangman/assets/Arial.ttf")) {
        std::cout<<"Failed to load the .ttf file"<<std::endl;
    }
    text.setFont(font);
    text.setString("Hello World");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    while(window.isOpen()) {
        window.draw(text);
        window.display();
        sf::Event event;
        while(window.pollEvent(event)) {
            // Event handling;

            if(event.type == sf::Event::Closed){
                std::cout<<"Closing the window";
                window.close();
            }

        }
        window.clear();
    }
}
