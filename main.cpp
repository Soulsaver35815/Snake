#include <SFML/Graphics.hpp>
#include <iostream>
#include "Window.hpp"
#include "Resources.hpp"

#define TODO_LIST_

#ifdef TODO_LIST
#warning TODO Music Player
#endif


#define WIN_SIZE 1000

int main(int argc, char* argv[])
{   
    //Resources& resource = Resources::getInstance();
    //initialize time 
    srand(time(NULL));
    //create window with VSync
    sf::RenderWindow render_window(sf::VideoMode(WIN_SIZE, WIN_SIZE), "Snake", sf::Style::Titlebar | sf::Style::Close);
    //render_window.setIcon(resource.m_snake_icon.getSize().x, resource.m_snake_icon.getSize().y, resource.m_snake_icon.getPixelsPtr());
    render_window.setVerticalSyncEnabled(true);
    //render_window.setFramerateLimit(60);
    render_window.setKeyRepeatEnabled(false); 
    //create window object
    Window window;
    while(render_window.isOpen())
    {
        //game tick from window object
        window.windowTick(render_window);
        
        //clear, draw and display window object
        render_window.clear(sf::Color(169,169,169));
        render_window.draw(window);
        render_window.display();
    }    
}
