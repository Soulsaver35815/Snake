#include "Counter.hpp"
#include <iostream>

//Counter constructor initializes 2 colors and score with parameter
Counter::Counter(unsigned int init_score, int x, int y, bool high_bool): 
sf::Drawable(), 
m_black(0, 0, 0, 200), 
m_green(140, 198, 63, 200),
m_red(198, 100, 63, 200),
m_score(init_score),
m_pos_x(x),
m_pos_y(y),
m_highscore_counter(high_bool),
m_resource(Resources::getInstance())
{   
    //load score Font and set score text to initial value
    m_score_text.setString(std::to_string(m_score));
    
    //counter attributes and position are initialized
    setCounterAttributes();
    setCounterPosition();
}

//increses score by given amount
void Counter::increaseScore(unsigned int amount)
{
    m_score += amount;
    //score text gets updated
    m_score_text.setString(std::to_string(m_score));
    //score position gets updated
    setCounterPosition();
}

//draw function draws score text
void Counter::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_score_text);
}

//sets position of score counter
void Counter::setCounterPosition()
{
    m_score_text.getGlobalBounds();
    //origin is in the top right corner so score stays at the same position despite 2 digits
    if(m_highscore_counter == true)
    {
        m_score_text.setOrigin(0, 0);
    }
    else
    {
        m_score_text.setOrigin(m_score_text.getGlobalBounds().width, 0);
    }
    m_score_text.setPosition(m_pos_x, m_pos_y);
}

//attributes of counter are set
void Counter::setCounterAttributes()
{
    m_score_text.setFont(m_resource.getFont("game_font"));
    
    m_score_text.setCharacterSize(80);
    m_score_text.setOutlineThickness(2);
    //own colors being used
    if(m_highscore_counter == true)
    {
        m_score_text.setFillColor(m_red);
    }
    else 
    {
        m_score_text.setFillColor(m_green);
    }
    
    m_score_text.setOutlineColor(m_black);
}

unsigned int Counter::getScore()
{
    return m_score;
}