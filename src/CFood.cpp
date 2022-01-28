#include "../headers/CFood.hpp"

using namespace sf;
using namespace std;

//food constructor
CFood::CFood(Difficulty difficulty, CNode* head, vector<CFood>& food_vec): 
m_difficulty(difficulty),
m_resource(CResources::getInstance()),
m_settings_values(CSettingsValues::getInstance())
{
    //set attributes for crunch sound
    m_crunch_sound.setBuffer(m_resource.m_crunch_buffer);
    m_crunch_sound.setVolume(*m_settings_values.getVolumeEat());
    //set attributes of food sprite
    m_food_sprite.setTexture(m_resource.m_food_texture);
    //food gets spawned at random position in window
    setRandCoordinates(head, food_vec);
}

CFood::~CFood()
{}


//simple draw function
void CFood::draw(RenderTarget& target, RenderStates states) const
{   
    //draws counter and food
    target.draw(m_food_sprite);
};

//returns food bounds for collition check
FloatRect CFood::getFoodBounds()
{
    return m_food_sprite.getGlobalBounds();
}

//function is called when food gets eaten
void CFood::gotEaten(CNode* head, vector<CFood>& food_vec)
{
    //crunch sound
    m_crunch_sound.play();
    //varibales for itteration and collition check
    CNode *temp = head;
    bool collition;

    setRandCoordinates(head, food_vec);
}

//set coordinates for new food
void CFood::setRandCoordinates(CNode* head, vector<CFood>& food_vec)
{
    //food bound of new food location and body bound for snake
    FloatRect body_bound;
    bool collition;
    CNode* temp;
    
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

        //temp pointer starts on head node, collition is set to false
        temp = head;
        collition = false;
        //while loop itterates through whole snake
        while(temp != nullptr)
        {
            //body bound gets set to current node bound
            body_bound = temp->m_node.getGlobalBounds();
            //when new food and node collide variable is set to true, new coordinates are set and while loop breaks
            if(getFoodBounds().intersects(body_bound)) 
            {
                collition = true;
                break;
            }    
            //pointer points to next node
            temp = temp->next;
        }
        int col_count = 0;
        for_each(food_vec.begin(), food_vec.end(), 
            [&](CFood& food)
            {
                if(getFoodBounds().intersects(food.getFoodBounds()))
                col_count += 1;
            });
        
        if(col_count >= 2)
            collition = true;
    //loop itterates as long as collition is true        
    }while(collition == true);  
}

