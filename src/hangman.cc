#include "hangman.hpp"
#include "SFML/Config.hpp"
#include <iostream>

Hangman::Hangman() {
    this->eventHandlerThread_ = std::thread(&Hangman::eventHandlerLoop, this);
}

Hangman::~Hangman() {
    eventHandlerThread_.join();
}


void Hangman::eventHandlerLoop() {
    while(!exit_requested_) {
        if(!this->charQueue_.empty()) {
            char text = this->charQueue_[0];
            // TODO: handle events like enter and backspace;
            this->charQueue_.erase(this->charQueue_.begin());
            std::cout<<"Erased "<<text<<" from the queue"<<std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
}

void Hangman::run() {
    //Creating a window
    window_.create(sf::VideoMode(800,600), "Hangman");

    while (window_.isOpen())
    {
        while (window_.pollEvent(event_))
        {
            switch (event_.type) {
                case sf::Event::Closed:
                    std::cout<<"Closing window..."<<std::endl;
                    exit_requested_ = true;
                    window_.close();
                    break;

                case sf::Event::TextEntered:
                    std::cout<<"Text entered event"<<std::endl;
                    handleTextEvent(event_.text.unicode);
                    break;

                default:
                    break;
            }
        }
    }
}

void Hangman::handleTextEvent(sf::Uint32 text) {
    if(text < 128)
    {
        char charText = static_cast<char>(text);
        std::cout<<"Key pressed: "<<charText<<std::endl;
        this->charQueue_.push_back(charText);
    }
}

