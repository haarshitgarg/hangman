#ifndef HANGMAN_HPP
#define HANGMAN_HPP

#include <thread>
#include <memory>
#include <SFML/Window.hpp>

class Hangman {
	public:
        Hangman();
        ~Hangman();
		void run();

	private:
		sf::Window window_;
        sf::Event event_;
        bool exit_requested_ = false;
        
        std::vector<char> charQueue_;

        std::thread eventHandlerThread_;

        void handleTextEvent(sf::Uint32 text);
        
        void eventHandlerLoop();
};

#endif //HANGMAN_HPP
