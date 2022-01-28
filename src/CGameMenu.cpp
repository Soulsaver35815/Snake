#include "../headers/CGameMenu.hpp"

#define POSITION_Y 500

using namespace sf;
using namespace std;

CGameMenu::CGameMenu(CGame* game, Texture& menu_texture, Texture& button_one_texture, Texture& button_two_texture, int button_one_y, int button_two_y, int menu_y) : 
m_game_instance(game),
m_resource(CResources::getInstance()),
m_settings_values(CSettingsValues::getInstance())
{   
    //initialise all 3 textures and get bounds from buttons
    initButton(m_game_menu_sprite, menu_texture, Vector2f(POSITION_Y, menu_y));
    m_button_one_bound = initButton(m_button_one_sprite, button_one_texture, Vector2f(POSITION_Y, button_one_y));
    m_button_two_bound = initButton(m_button_two_sprite, button_two_texture, Vector2f(POSITION_Y, button_two_y));
    
    //initialise click sound
    m_click_sound.setBuffer(m_resource.m_click_buffer);
    m_click_sound.setVolume(*m_settings_values.getVolumeClick());  
}

CGameMenu::~CGameMenu()
{}

WindowInstance CGameMenu::gameMenuTick(RenderWindow& renderWindow)
{
    //wait Event to prevent unnecessary memory and cpu usage
    Event event;
    while(renderWindow.waitEvent(event))
    {
        switch(event.type)
        {
            case Event::Closed:
                renderWindow.close(); break;
            
            //can navigate window with buttons
            case Event::KeyPressed:
                switch(event.key.code)
                {
                    case Keyboard::Escape: return Menu; break;
                    case Keyboard::Space: return Game; break;
                    default: break;
                }
                break;
            
            //mouse press event
            case Event::MouseButtonPressed:
                if(event.mouseButton.button == Mouse::Left)
                {
                    //get coordinates of mouse press
                    Vector2i cords = Mouse::getPosition(renderWindow);
                    //check collition between mouse coordinates and resumeBound
                    if(m_button_one_bound.contains(cords.x, cords.y)) 
                    {
                        //play click sound, wait and return true(game gest started)
                        m_click_sound.play(); 
                        sleep(milliseconds(300)); 
                        return buttonOneAction();
                    }
                    //check collition between mouse coordinates and menuBound
                    else if(m_button_two_bound.contains(cords.x, cords.y)) 
                    {
                        //play click sound wait and close window
                        m_click_sound.play(); 
                        sleep(milliseconds(300)); 
                        return buttonTwoAction(); 
                    }  
                }
                break;
            default: break;
        }
    }
    //default returns false so game doesn't start
    return BreakMenu;
}

//used to initialise a button with a texture, position and origin
FloatRect CGameMenu::initButton(Sprite& sprite, Texture& texture, Vector2f pos)
{
    sprite.setTexture(texture);
    sprite.setOrigin(int(sprite.getGlobalBounds().width/2), int(sprite.getGlobalBounds().height/2));
    sprite.setPosition(pos);

    //returns global bounds for button click-detection
    return sprite.getGlobalBounds();
}

//contains the action that is performed on button press
WindowInstance CGameMenu::buttonOneAction()
{
    return Game;
}

//contains the action that is performed on button press
WindowInstance CGameMenu::buttonTwoAction()
{
    return Menu;
}

void CGameMenu::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(*this->m_game_instance); 
    target.draw(this->m_game_menu_sprite);  
    target.draw(this->m_button_one_sprite);
    target.draw(this->m_button_two_sprite);
}
