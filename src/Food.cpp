#include "Food.hpp"

using namespace sf;
using namespace std;

//food constructor
Food::Food(std::list<Node>& nodes, vector<Food>& food_vec):
m_resource(Resources::getInstance()),
m_settings_values(SettingsValues::getInstance())
{
    //set attributes for crunch sound
    m_crunch_sound.setBuffer(m_resource.getSound("crunch_buffer"));
    m_crunch_sound.setVolume(*m_settings_values.getVolumeEat());
    //set attributes of food sprite
    m_food_sprite.setTexture(m_resource.getTexture("food_texture"));
    //food gets spawned at random position in window
    setRandCoordinates(nodes, food_vec);
}

Food::~Food()
{}


//simple draw function
void Food::draw(RenderTarget& target, RenderStates states) const
{   
    //draws counter and food
    target.draw(m_food_sprite);
};

//returns food bounds for collition check
FloatRect Food::getFoodBounds()
{
    return m_food_sprite.getGlobalBounds();
}

//function is called when food gets eaten
void Food::gotEaten(list<Node>& nodes, vector<Food>& food_vec)
{
    //crunch sound
    m_crunch_sound.play();

    setRandCoordinates(nodes, food_vec);
}

//set coordinates for new food
void Food::setRandCoordinates(list<Node>& nodes, vector<Food>& food_vec)
{
    //food bound of new food location and body bound for snake
    FloatRect body_bound;
    bool collition;
    
    //do while checks once if new food coordinates collide with snake(if yes while loop until not)
    do
    {   
        //random coordinates get chosen
        m_pos_x = (rand() % 941 + 39);
        m_pos_y = (rand() % 941 + 39);
        //coordinates need to be increment of 40 to fit on grid
        m_pos_x = m_pos_x - (m_pos_x % 40);
        m_pos_y = m_pos_y - (m_pos_y % 40);
        //coordinates get passed to sprite
        m_food_sprite.setPosition(m_pos_x, m_pos_y);

        collition = false;
        //while loop itterates through whole snake

        for_each(nodes.begin(), nodes.end(), 
            [&](Node& node){if(getFoodBounds().intersects(node.m_node.getGlobalBounds())) collition = true;});

        int col_count = 0;
        for_each(food_vec.begin(), food_vec.end(), 
            [&](Food& food)
            {
                if(getFoodBounds().intersects(food.getFoodBounds()))
                col_count += 1;
            });
        
        if(col_count >= 2)
            collition = true;
    //loop itterates as long as collition is true        
    }while(collition);  
}

