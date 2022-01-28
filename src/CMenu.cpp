#include "../headers/CMenu.hpp"

#define BUTTON_X 500

using namespace sf;
//constructor initializes 2 colors and all the textures
CMenu::CMenu():
m_my_black(0, 0, 0, 200), 
m_my_green(0, 255, 0, 200),
m_resource(CResources::getInstance()),
m_settings_values(CSettingsValues::getInstance())
{
    initTextures();
}

//menuTick function
WindowInstance CMenu::menuTick(RenderWindow& renderWindow)
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
                    case Keyboard::Space: return Settings; break;
                    case Keyboard::Escape:  renderWindow.close(); break;
                    default: break;
                }
                break;
            
            //mouse press event
            case Event::MouseButtonPressed:
                if(event.mouseButton.button == Mouse::Left)
                {
                    //get coordinates of mouse press
                    Vector2i cords = Mouse::getPosition(renderWindow);
                    //check collition between mouse coordinates and m_play_bound
                    if(m_play_bound.contains(cords.x, cords.y)) 
                    {
                        //play click sound, wait and return true(game gest started)
                        m_click_sound.play(); 
                        sleep(milliseconds(300)); 
                        return Game;
                    }
                    //check collition between mouse coordinates and m_quit_bound
                    else if(m_quit_bound.contains(cords.x, cords.y)) 
                    {
                        //play click sound wait and close window
                        m_click_sound.play(); 
                        sleep(milliseconds(300)); 
                        renderWindow.close();  
                    }  
                    else if(m_settings_bound.contains(cords.x, cords.y)) 
                    {
                        //play click sound wait and close window
                        m_click_sound.play(); 
                        sleep(milliseconds(300)); 
                        return Settings;  
                    }  
                }
                break;
            default: break;
        }
    }
    //default returns false so game doesn't start
    return Menu;
}

void CMenu::initTextures()
{
    initButton(m_logo_sprite, m_resource.m_logo_texture, Vector2f(BUTTON_X, 300));
    m_play_bound = initButton(m_play_sprite, m_resource.m_play_texture, Vector2f(BUTTON_X, 500));
    m_settings_bound = initButton(m_settings_sprite, m_resource.m_settings_texture, Vector2f(BUTTON_X, 680));
    m_quit_bound = initButton(m_quit_sprite, m_resource.m_quit_texture, Vector2f(BUTTON_X, 860));

    m_click_sound.setBuffer(m_resource.m_click_buffer);
    m_click_sound.setVolume(*m_settings_values.getVolumeClick());
}

//used to initialise a button with a texture, position and origin
FloatRect CMenu::initButton(Sprite& sprite, Texture& texture, Vector2f pos)
{
    sprite.setTexture(texture);
    sprite.setOrigin(static_cast<int>(sprite.getGlobalBounds().width/2), static_cast<int>(sprite.getGlobalBounds().height/2));
    sprite.setPosition(pos);

    //returns global bounds for button click-detection
    return sprite.getGlobalBounds();
}

//draw dunction draws all 3 logos/buttons
void CMenu::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(m_logo_sprite);
    target.draw(m_play_sprite);
    target.draw(m_quit_sprite);
    target.draw(m_settings_sprite);
}
