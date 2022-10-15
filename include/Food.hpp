#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <fstream>
#include <list>
#include "Counter.hpp"
#include "Node.hpp"
#include "Resources.hpp"
#include "SettingsValues.hpp"

//food class to create food
class Food : public sf::Drawable
{
    public:
        Food(std::list<Node>& nodes, std::vector<Food>&);
        ~Food();
        //gotEaten to relocate food
        void gotEaten(std::list<Node>& nodes, std::vector<Food>&);
        //returnRect to get bounds of food
        sf::FloatRect getFoodBounds();
    private:
        int m_pos_x;
        int m_pos_y;
        
        Resources& m_resource;    
        SettingsValues& m_settings_values;

        sf::Sprite m_food_sprite;
        sf::Sound m_crunch_sound;
        
        void setRandCoordinates(std::list<Node>& nodes, std::vector<Food>&);
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};