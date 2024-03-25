#include "hangman.hpp"
#include "SFML/Config.hpp"
#include <functional>
#include <iostream>

Hangman::Hangman() {
    this->eventHandlerThread_ = std::thread(&Hangman::eventHandlerLoop, this);
    this->uiHandlerThread_ = std::thread(&Hangman::TaskQueueLoop, this);

    std::string bg_img_path = "/Users/harshitgarg/CPP-projects/hangman/assets/hangman3.png";
    if(!this->startup_image_.loadFromFile(bg_img_path)) {
        std::cerr<<"Failed to load the image: "<<bg_img_path<<std::endl; 
        //TODO add a simple_background_image for loading the background in this->run() function
    }

    this->startup_sprite_.setTexture(startup_image_);

    font_.loadFromFile("/Users/harshitgarg/CPP-projects/hangman/assets/Arial.ttf");
}

Hangman::~Hangman() {
    eventHandlerThread_.join();
    uiHandlerThread_.join();
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
    window_.setVerticalSyncEnabled(true);
     
    this->AddTaskToQueue(std::bind(&Hangman::setupSpriteBackground, this));
    int count = 0;
    while (window_.isOpen())
    {

        window_.draw(this->startup_sprite_);
        window_.draw(this->startup_text_);
        window_.display();
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
        window_.clear(sf::Color::White);
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


void Hangman::setupSpriteBackground() {
    // I need to set the size
    // I need to animate the background
    int count = 0;
    while(this->window_.isOpen() && count < 8) {
        std::cout<<"Here with count: "<<count<<std::endl;
        this->startup_sprite_.scale(1.1, 1.1);    
        std::this_thread::sleep_for(std::chrono::seconds(1));
        count++;
    }
    this->startup_text_.setFont(font_);
    this->startup_text_.setString("HANGMAN");
    this->startup_text_.setFillColor(sf::Color::Red);
    this->startup_text_.setCharacterSize(100);
    this->startup_text_.setPosition(50, 300);

}

void Hangman::TaskQueueLoop() {
   while(!this->exit_requested_) {
        std::function<void()> task;
        if(this->function_stack_.empty()) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        else {
            task = this->function_stack_.front();
            this->function_stack_.pop();
            task();
        }
   }

}

void Hangman::AddTaskToQueue(const std::function<void()>& func) {
    this->function_stack_.push(func);
}
