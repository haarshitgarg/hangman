#ifndef HANGMAN_HPP
#define HANGMAN_HPP

#include "SFML/Graphics/Text.hpp"
#include <thread>
#include <functional>
#include <queue>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Hangman {
	public:
        Hangman();
        ~Hangman();
		void run();

	private:
		sf::RenderWindow window_;
        sf::Event event_;
        sf::Texture startup_image_;
        sf::Sprite startup_sprite_;
        sf::Text startup_text_;

        sf::Font font_;
        std::queue<std::function<void()>> function_stack_;


        bool exit_requested_ = false;
        
        std::vector<char> charQueue_;

        std::thread eventHandlerThread_;

        // I can create a task manager to handle UI but right now I can only think of one thread
        // I can use a task manager for all the task I want to post on a thread;
        std::thread uiHandlerThread_;
    
        // Handles the text events for the game
        void handleTextEvent(sf::Uint32 text);
       
        // Event loop that perfoms the operation in an event handler thread
        void eventHandlerLoop();

        // Setup the background sprite
        void setupSpriteBackground();
 
        // TaskManager function
        void TaskQueueLoop();

        // Add a void() function to the stack
        void AddTaskToQueue(const std::function<void()>& func);
};

#endif //HANGMAN_HPP
