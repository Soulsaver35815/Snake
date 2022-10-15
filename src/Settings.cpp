#include "Settings.hpp"
#include <iostream>
#define LEFT_SIDE_X 300
#define RIGHT_SIDE_X 700
#define MAX_INIT_LENGTH (12 * 13)
#define MAX_FOOD_COUNT 10

using namespace std;
using namespace sf;

Settings::Settings() : 
m_resource(Resources::getInstance()),
m_settings_values(SettingsValues::getInstance())
{
    //read settings from file and write them to member variables
    m_settings_values.getSettings();
    initValues();
    //initialise textures (only if object is an active settings instance)
    initTextures();
}

//destructor
Settings::~Settings()
{
    m_settings_values.setSettings();
}

WindowInstance Settings::settingsTick(RenderWindow& renderWindow)
{
    Event event;
    while(renderWindow.pollEvent(event))
    {
        switch(event.type)
        {
            case Event::Closed:
                renderWindow.close(); break;
            
            //can navigate window with buttons
            case Event::KeyPressed:
                switch(event.key.code)
                {
                    case Keyboard::Escape: return EMenu; break;
                    default: break;
                }
                break;

            //mouse press event
            case Event::MouseButtonPressed:
                if(event.mouseButton.button == Mouse::Left)
                {
                    //get coordinates of mouse press
                    Vector2i cords = Mouse::getPosition(renderWindow);

                    std::shared_ptr<Slider<int>>* slider_int_array[] = 
                    {
                        &m_slider_music,
                        &m_slider_eat,
                        &m_slider_game_over,
                        &m_slider_click,
                        &m_slider_length,
                        &m_slider_food_count
                    };
                    
                    std::shared_ptr<Slider<bool>>* slider_bool_array[] = 
                    {
                        &m_slider_self_crash,
                        &m_slider_wall_crash
                    };

                    for(auto sld: slider_int_array)
                    {
                        if(sld->get()->getSliderBounds().contains(cords.x, cords.y))
                            m_active_slider_int = *sld;
                    }

                    for(auto sld: slider_bool_array)
                    {
                        if(sld->get()->getSliderBounds().contains(cords.x, cords.y))
                            m_active_slider_bool = *sld;
                    }

                    if(m_slider_difficulty->getSliderBounds().contains(cords.x, cords.y))
                    {
                        m_active_slider_diff = m_slider_difficulty;     
                    }
                    else if(m_menu_bounds.contains(cords.x, cords.y)) 
                    {
                        //play click sound, save settings, wait and return Menu
                        m_click_sound.play(); 
                        sleep(milliseconds(300)); 
                        return EMenu;
                    }
                }
                break;

            case Event::MouseButtonReleased:
                //if left mousebutton is released and 
                if(event.mouseButton.button == Mouse::Left)
                {
                    m_active_slider_int = nullptr; 
                    m_active_slider_diff = nullptr;
                    m_active_slider_bool = nullptr;
                }

                break;

            default: break;
        }
        moveSlider(renderWindow);
    }
    //default returns Settings so game doesn't start
    return ESettings;
}


//moves the slider that is currently active to the x cursor position
void Settings::moveSlider(const RenderWindow& renderWindow)
{
    if(m_active_slider_int != nullptr)
    {
        if(m_active_slider_int->moveSlider(Mouse::getPosition(renderWindow).x))
        {
            if(m_active_slider_int == m_slider_length)
                m_active_slider_int->setSliderLabel("Length:" + to_string(*m_length));
            else if(m_active_slider_int == m_slider_food_count)
                m_active_slider_int->setSliderLabel("Apples:" + to_string(*m_food_count));
        }   
    }
    else if(m_active_slider_diff != nullptr)
    {
        if(m_active_slider_diff->moveSlider(Mouse::getPosition(renderWindow).x))
            //set the slider name texture to the corresponding texture from the texture array according to m_difficulty only 
            m_active_slider_diff->setSliderLabel(m_difficulty_string_arr[static_cast<int>(*m_difficulty)]);
    }
    else if(m_active_slider_bool != nullptr)
    {
        if(m_active_slider_bool->moveSlider(Mouse::getPosition(renderWindow).x))
        {
            if(m_active_slider_bool == m_slider_wall_crash)
                m_active_slider_bool->setSliderLabel("WallCrash:" + m_on_off_string_arr[static_cast<int>(*m_wall_crash_b)]);
            else if(m_active_slider_bool == m_slider_self_crash)
                m_active_slider_bool->setSliderLabel("SelfCrash:" + m_on_off_string_arr[static_cast<int>(*m_self_crash_b)]);
        }
    }
}

//initialise all sliders
void Settings::initSliders()
{
    m_slider_music = make_shared<Slider<int>>(m_volume_music, RIGHT_SIDE_X, 260, "Music");
    m_slider_eat = make_shared<Slider<int>>(m_volume_eat, RIGHT_SIDE_X, 380, "Eat");
    m_slider_game_over = make_shared<Slider<int>>(m_volume_game_over, RIGHT_SIDE_X, 500, "Lost");
    m_slider_click = make_shared<Slider<int>>(m_volume_click, RIGHT_SIDE_X, 620, "Click");
    m_slider_difficulty = make_shared<Slider<Difficulty>>(m_difficulty, LEFT_SIDE_X, 260, m_difficulty_string_arr[static_cast<int>(*m_difficulty)], 0, 4);
    m_slider_length = make_shared<Slider<int>>(m_length, LEFT_SIDE_X, 380, "Length:" + to_string(*m_length), 2, MAX_INIT_LENGTH);
    m_slider_food_count = make_shared<Slider<int>>(m_food_count, LEFT_SIDE_X, 500, "Apples:" + to_string(*m_food_count), 1, MAX_FOOD_COUNT);
    m_slider_wall_crash = make_shared<Slider<bool>>(m_wall_crash_b, LEFT_SIDE_X, 620, "WallCrash:" + m_on_off_string_arr[static_cast<int>(*m_wall_crash_b)], 0, 1);
    m_slider_self_crash = make_shared<Slider<bool>>(m_self_crash_b, LEFT_SIDE_X, 740, "SelfCrash:" + m_on_off_string_arr[static_cast<int>(*m_self_crash_b)], 0, 1);
}

//initialises all textures if settings are active
void Settings::initTextures()
{
        m_menu_bounds = initButton(m_menu_sprite, m_resource.getTexture("menu_texture"), Vector2f(500, 880));
        initButton(m_difficulty_sprite, m_resource.getTexture("difficulty_texture"), Vector2f(LEFT_SIDE_X, 160));
        initButton(m_volume_sprite, m_resource.getTexture("volume_texture"), Vector2f(RIGHT_SIDE_X, 160));
        //set sound source and volume for click sound
        m_click_sound.setBuffer(m_resource.getSound("click_buffer"));
        m_click_sound.setVolume(*m_volume_click);
        //initialise sliders
        initSliders();
}

void Settings::initValues()
{
    m_volume_music = m_settings_values.getVolumeMusic();
    m_volume_click = m_settings_values.getVolumeClick();
    m_volume_eat = m_settings_values.getVolumeEat();
    m_volume_game_over = m_settings_values.getVolumeGameOver();
    m_length = m_settings_values.getLength();
    m_food_count = m_settings_values.getFoodCount();
    m_wall_crash_b = m_settings_values.getWallCrash();
    m_self_crash_b = m_settings_values.getSelfCrash();
    m_difficulty = m_settings_values.getDifficulty();
}

//used to initialise a button with a texture, position and origin
FloatRect Settings::initButton(Sprite& sprite, Texture& texture, Vector2f pos)
{
    sprite.setTexture(texture);
    sprite.setOrigin(int(sprite.getGlobalBounds().width/2), int(sprite.getGlobalBounds().height/2));
    sprite.setPosition(pos);

    //returns global bounds for button click-detection
    return sprite.getGlobalBounds();
}

//draws all the sprites, buttons and sliders
void Settings::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(*m_slider_music);
    target.draw(*m_slider_eat);
    target.draw(*m_slider_click);
    target.draw(*m_slider_game_over);
    target.draw(*m_slider_length);
    target.draw(*m_slider_difficulty);
    target.draw(*m_slider_food_count);
    target.draw(*m_slider_wall_crash);
    target.draw(*m_slider_self_crash);

    target.draw(m_difficulty_sprite);
    target.draw(m_volume_sprite);
    target.draw(m_menu_sprite);
}