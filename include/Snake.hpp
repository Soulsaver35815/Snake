#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <list>
#include "Node.hpp"
#include "headerEnum.hpp"
#include "Food.hpp"
#include "SettingsValues.hpp"


//snake class
class Snake : public sf::Drawable
{
    //functions to add/remove head/tail of snake to make it move
    public:
        Snake(int, int, unsigned int, unsigned int);
        ~Snake();
        bool addHead(WindowInstance&);
        void removeTail();
        void setState(SnakeState);
        void setChangedThisTick(bool);
        bool getChangedThisTick();
        SnakeState getState();
        Node& getHead();
    private:
        SnakeState m_snake_state = W;
        bool m_changed_this_tick_b = false;
        std::vector<Food> m_food_vec;

        SettingsValues& m_settings_values;

        std::list<Node> m_nodes;   
        
        bool checkFoodBounds(sf::FloatRect&);

        void initSnake(const unsigned int);
        void initFood(unsigned int);
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};