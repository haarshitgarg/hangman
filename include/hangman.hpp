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
        sf::Text game_finish_text_;
        sf::Text game_ui_text_box_;
        sf::Texture game_ui_hangman_;
        sf::Sprite game_ui_sprite_;

        sf::Font font_;
        std::queue<std::function<void()>> function_stack_;

        std::vector<int> display_map_;
        bool exit_requested_ = false;
        bool background_running_ = true;
        bool game_finishe_= false;
        
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
        
        // Handling the ui
        void handleUI(sf::Text& ui_text);

        void handleHangmanImages(int count);


        // Correct_word
        std::string correct_word_;

        bool checkInput(char letter);

        void reset();

        std::string getWord();
};

#endif //HANGMAN_HPP
