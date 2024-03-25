#include "hangman.hpp"
#include "SFML/Config.hpp"
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

Hangman::Hangman() {
    this->eventHandlerThread_ = std::thread(&Hangman::eventHandlerLoop, this);
    this->uiHandlerThread_ = std::thread(&Hangman::TaskQueueLoop, this);

    std::string bg_img_path = "/Users/harshitgarg/CPP-projects/hangman/assets/hangman3.png";
    if(!this->startup_image_.loadFromFile(bg_img_path)) {
        std::cerr<<"Failed to load the image: "<<bg_img_path<<std::endl; 
        //TODO add a simple_background_image for loading the background in this->run() function
    }

    this->startup_sprite_.setTexture(startup_image_);
   
    // TODO edit this to take random words from a file
    this->correct_word_ = getWord();
    for(int i = 0; i<correct_word_.size(); i++) {
        this->display_map_.push_back(0);
    }

    // Setting up UI text
    font_.loadFromFile("/Users/harshitgarg/CPP-projects/hangman/assets/Arial.ttf");
    this->game_ui_text_box_.setFont(font_);

    std::string my_text_box;
    for(int i = 0; i<this->correct_word_.size(); i++) {
        my_text_box += "|   ";
    }
    my_text_box += "|";
    game_ui_text_box_.setString(my_text_box);
    game_ui_text_box_.setCharacterSize(70);
    game_ui_text_box_.setFillColor(sf::Color::Red);
    game_ui_text_box_.setStyle(sf::Text::Style::Underlined);

    game_ui_text_box_.setPosition(50, 400);

    game_finish_text_.setFont(font_);
    game_finish_text_.setPosition(450, 100);
    game_finish_text_.setCharacterSize(50);
    game_finish_text_.setFillColor(sf::Color::Red);
}

Hangman::~Hangman() {
    eventHandlerThread_.join();
    uiHandlerThread_.join();
}


void Hangman::eventHandlerLoop() {
    int count = 1;
    this->handleHangmanImages(count);
    count++;
    while(!exit_requested_) {
        if(!this->charQueue_.empty()) {
            char text = this->charQueue_[0];
            // TODO: handle events like enter and backspace;
            this->charQueue_.erase(this->charQueue_.begin());
            sf::Texture texture;
            sf::Sprite sprite;
            if(checkInput(text)) {
                this->handleUI(this->game_ui_text_box_);
                if(this->game_finishe_)
                {
                    count = 2;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    this->reset();

                    game_finishe_ = false;
                }

            }
            else {
                this->handleHangmanImages(count);
                if(count == 7) {
                    game_finish_text_.setString("It is "+ correct_word_);
                    game_finishe_ = true;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    game_finishe_ = false;
                    this->reset();
                    count = 1;
                }
                count++;
                std::cout<<"Letter is wrong"<<std::endl;
            }
            std::cout<<"Erased "<<text<<" from the queue"<<std::endl;
        }
        else{
            std::this_thread::sleep_for(std::chrono::seconds(1));
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
        if(background_running_) {
            window_.draw(this->startup_sprite_);
            window_.draw(this->startup_text_);
        }
        else {
            window_.draw(this->game_ui_sprite_);
            window_.draw(this->game_ui_text_box_);
        }

        if(this->game_finishe_){
            window_.draw(this->game_finish_text_);
        }

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
                    if(!background_running_){
                        std::cout<<"Text entered event"<<std::endl;
                        handleTextEvent(event_.text.unicode);
                    }
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
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        count++;
    }
    this->startup_text_.setFont(font_);
    this->startup_text_.setString("Hangman (for Purvaja)");
    this->startup_text_.setFillColor(sf::Color::Red);
    this->startup_text_.setCharacterSize(60);
    this->startup_text_.setPosition(50, 300);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    this->background_running_ = false;

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

void Hangman::handleUI(sf::Text& ui_text) {
    std::string current_word;
    std::string my_text_box;
    for(int i = 0; i<this->correct_word_.size(); i++) {
        if(this->display_map_[i] == 0){
            my_text_box += "|   ";
        }
        else {
            my_text_box += "| " + correct_word_.substr(i,1) + " ";
            current_word += correct_word_.substr(i, 1);
        }
    }
    if(current_word == correct_word_) {
        this->game_finishe_ = true;
        this->game_finish_text_.setString("You WON");
    }
    my_text_box += "|";
    ui_text.setString(my_text_box);
    ui_text.setCharacterSize(70);
    ui_text.setFillColor(sf::Color::Red);
    ui_text.setStyle(sf::Text::Style::Underlined);

    ui_text.setPosition(50, 400);
}


bool Hangman::checkInput(char letter) {
    bool status = false;
    for(int i = 0; i<display_map_.size(); i++) {
       if(letter == correct_word_[i]) {
            display_map_[i] = 1;
            status = true;
       }
    }
    return status;
}

void Hangman::handleHangmanImages(int count) {
    std::string file_path = "/Users/harshitgarg/CPP-projects/hangman/assets/hangmanpart";
    file_path += std::to_string(count) + ".png";
    if(!this->game_ui_hangman_.loadFromFile(file_path)) {
        std::cout<<"Failed to load the file: "<<file_path<<std::endl;
    }
    this->game_ui_sprite_.setTexture(this->game_ui_hangman_);
}

void Hangman::reset() {
    // TODO load a random word from the a list
    correct_word_ = getWord();
    display_map_.clear();
    for(int i = 0; i<correct_word_.size(); i++) {
        display_map_.push_back(0);
    }
    game_finish_text_.setString("");
    handleUI(game_ui_text_box_);
    handleHangmanImages(1); 
}

std::string Hangman::getWord() {
    std::vector<std::string> word = {"APPLE", "BANANA", "BOMB", "BEACH", "BRAIN", "FLUTE", "RED", "GREEN"};
    int i = rand()%8;
    return word[i];
}









