#include "Snake.hpp"

#define TILE_SIZE 40
#define GRID_SIZE 25
#define MAX_INIT_LENGTH ((12 * 13) + 1)
#define MAX_FOOD_COUNT 10
using namespace sf;
using namespace std;

//constructor takes new head position as argument 
//and initializes new node as head, snakeState(direction) and changedState
Snake::Snake(int init_x, int init_y, unsigned int length, unsigned int food_count): 
m_settings_values(SettingsValues::getInstance()),
m_nodes({Node(init_y, init_x, W)})
{  
    //tail = head (only 1 node exists)
    initSnake(length);
    initFood(food_count);
}

Snake::~Snake()
{}

//function to add head for movement/growth food pointer as arguments
bool Snake::addHead(WindowInstance& new_instance)
{
    //change texture of old head to normal body texture

    m_nodes.front().changeTexture(m_snake_state, false);

    int temp_y = m_nodes.front().m_node_pos_y;
    int temp_x = m_nodes.front().m_node_pos_x;
    switch(m_snake_state)
    {
        //position of new head gets calculated using old heads position and moving direction
        case W: temp_y -= TILE_SIZE; break;
        case A: temp_x -= TILE_SIZE; break;
        case S: temp_y += TILE_SIZE; break;
        case D: temp_x += TILE_SIZE; break;
        default: break;
    }

    //** CRASH WHEN HITTING WALL **//
    //if new head is created outside the window(player crashed into border) true is returned
    if(*m_settings_values.getWallCrash() == true)
    {
        if(temp_x < 0|
        temp_x >= 1000|
        temp_y < 0|
        temp_y >= 1000) new_instance = EGameOverMenu;
    }
    //** DON'T CRASH WHEN HITTING WALL **//
    else
    {
        if(temp_x < 0) temp_x += 1000;
        else if(temp_x >= 1000) temp_x -= 1000;
        else if(temp_y < 0) temp_y += 1000;
        else if(temp_y >= 1000) temp_y -= 1000;
    }

    m_nodes.front().setPrevState(m_snake_state);
    //new head gets created at calculated coordinates. All pointers are set to link the head to the list
    m_nodes.push_front({temp_y, temp_x, m_snake_state});
    
    //bounds for head and body to check for collition when creating the new head
    FloatRect head_bound = m_nodes.front().m_node.getGlobalBounds();
    FloatRect body_bound;

    //if the head collides with the food 
    if(checkFoodBounds(head_bound))
    {
        m_nodes.back().changeTexture(m_snake_state, true);
        return true;
    }

    //tail gets removed only when no food is eaten (snake grows)
    removeTail();
    if(m_nodes.size() > 1) m_nodes.back().changeTexture(m_snake_state, true);

    //** GAME OVER WHEN SNAKE HITS SNAKE **//
    if(*m_settings_values.getSelfCrash())
    {
        for(auto iter = std::next(m_nodes.begin()); iter != m_nodes.end(); iter++) 
            if(head_bound.intersects(iter->m_node.getGlobalBounds()))
                new_instance = EGameOverMenu;
    }      
    return false;
}

bool Snake::checkFoodBounds(FloatRect& head_bound)
{
    bool return_value = false;
    for_each(m_food_vec.begin(), m_food_vec.end(), 
        [&](Food& food) 
        {
            if(food.getFoodBounds().intersects(head_bound))
            {
                food.gotEaten(m_nodes, m_food_vec);
                return_value = true;
            }
        });
    return return_value;
}

void Snake::initSnake(unsigned int length)
{   
    unsigned int tail_length = length - 1;
    int delta_y, delta_x;
    SnakeState state;

    if(tail_length > MAX_INIT_LENGTH - 1) tail_length = MAX_INIT_LENGTH - 1;
    else if(tail_length < 1) tail_length = 1;

    for(int i = 0; i < tail_length; i++)
    {   
        delta_x = static_cast<int>(i / 12);

        if((delta_x % 2) == 1) 
        {
            delta_y = 12 - ( (i % 12));
            if(delta_y == 1) state = A;
            else state = S;
        }       
        else 
        {
            delta_y = (1 + (i % 12));
            if(delta_y == 12) state = A;
            else state = W;
        }

        SnakeState& old_state = m_nodes.back().m_node_state;
        m_nodes.push_back({m_nodes.front().m_node_pos_y + delta_y * TILE_SIZE, m_nodes.front().m_node_pos_x + delta_x * TILE_SIZE, state});
        m_nodes.back().changeTexture(old_state, false);
    }
    m_nodes.back().changeTexture(W, true);
}

void Snake::initFood(unsigned int food_count)
{
    if(food_count < 1) food_count = 1;
    else if(food_count > MAX_FOOD_COUNT) food_count = MAX_FOOD_COUNT;
    for(int i = 0; i < food_count; i++)
    {
        m_food_vec.push_back({m_nodes, m_food_vec});
    }
}

//function that removes the tail node to make it seem like the snake is moving
void Snake::removeTail()
{
    m_nodes.pop_back();
}

//draw function so snake can be drawn in window. Itterates through list and draws every node
void Snake::draw(RenderTarget& target, RenderStates states) const
{   
    for_each(m_nodes.begin(), m_nodes.end(), [&](Node node) {target.draw(node.m_node);});
    for_each(m_food_vec.begin(), m_food_vec.end(), [&](Food food) {target.draw(food);});
}

//returns state(direction) of snake
SnakeState Snake::getState()
{
    return m_snake_state;
}

//returns whether snake changed already in current tick
bool Snake::getChangedThisTick()
{
    return m_changed_this_tick_b;
}

//sets new state(new direction) and sets changedState to true
void Snake::setState(SnakeState new_state)
{
    m_snake_state = new_state;
    m_changed_this_tick_b = true;
}

//sets changedState to parameter
void Snake::setChangedThisTick(bool changed)
{
    m_changed_this_tick_b = changed;
}

Node& Snake::getHead()
{
    return m_nodes.front();
}


