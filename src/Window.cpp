#include "Window.hpp"

//constructor initializes game/menuInstance with nullptr and
//windowInstance with Menu so Menu is opend when object is created
Window::Window() :
m_resource(Resources::getInstance()),
m_settings_values(SettingsValues::getInstance())
{
    m_resource.getMusic("game_music").setLoop(true);
}

//window Tick takes renderWindow
void Window::windowTick(sf::RenderWindow& renderWindow)
{
    changeInstance();
    //switch for Instance (either Menu or Game)
    switch(m_window_instance)
    {
        case EMenu: 
            //if no menuInstance exists
            if(m_menu_instance == nullptr) 
            {
                //delete gameInstance and create new Menu Instance
                renderWindow.setKeyRepeatEnabled(false);
                delete m_break_menu_instance;
                delete m_game_over_menu_instance;
                delete m_game_instance;
                delete m_settings_instance;
                m_settings_instance = nullptr;
                m_break_menu_instance = nullptr;
                m_game_over_menu_instance = nullptr;
                m_game_instance = nullptr;
                m_menu_instance = new Menu; 
                //stop game music
                m_resource.getMusic("game_music").stop();
            }
            else
            {
                //if menuInstance exists get bool from menuTick to determine whether or not to change the Instance
                m_new_instance = m_menu_instance->menuTick(renderWindow);
            }
            break;

        case EGame: 
            //if no gameInstance exists
            if(m_game_instance == nullptr) 
            {
                //delete menuInstance and create new Game Instance
                delete m_menu_instance;
                if(m_game_over_menu_instance != nullptr) 
                {
                    delete m_game_over_menu_instance;
                    m_game_over_menu_instance = nullptr;
                }
                m_menu_instance = nullptr;
                m_game_instance = new Game; 
                //start playing gamemusic and set volume
                m_resource.getMusic("game_music").setVolume(*m_settings_values.getVolumeMusic());
                m_resource.getMusic("game_music").play();
            }
            else
            {
                if(m_break_menu_instance != nullptr)
                {
                    delete m_break_menu_instance;
                    m_break_menu_instance = nullptr;
                    m_resource.getMusic("game_music").play();
                }
                if(m_game_over_menu_instance != nullptr)
                {
                    delete m_game_over_menu_instance;
                    m_game_over_menu_instance = nullptr;
                    m_resource.getMusic("game_music").play();
                }
                //if gameInstance exists get bool from gameTick to determine whether or not to change the Instance
                m_new_instance = m_game_instance->gameTick(renderWindow);
            }
            break;
        case ESettings:
            //if no settingsInstance exists
            if(m_settings_instance == nullptr) 
            {
                renderWindow.setKeyRepeatEnabled(false);
                //delete menuInstance and create new Settings Instance
                delete m_menu_instance;
                m_menu_instance = nullptr; 
                m_settings_instance = new Settings;
            }
            else
            {
                //if menuInstance exists get bool from menuTick to determine whether or not to change the Instance
                m_new_instance = m_settings_instance->settingsTick(renderWindow);
            }
            break;
        
        case EBreakMenu:
            if(m_break_menu_instance == nullptr)
            {
                m_break_menu_instance = new BreakMenu(m_game_instance, 
                                                        m_resource.getTexture("game_menu_break_menu"), 
                                                        m_resource.getTexture("game_menu_resume"), 
                                                        m_resource.getTexture("game_menu_menu"));
                m_resource.getMusic("game_music").stop();
            }
            else
            {
                m_new_instance = m_break_menu_instance->gameMenuTick(renderWindow);
            }
            break;

        case EGameOverMenu: 
            if(m_game_over_menu_instance == nullptr)
            {
                m_game_over_menu_instance = new GameOverMenu(m_game_instance, 
                                                        m_resource.getTexture("game_menu_game_over"), 
                                                        m_resource.getTexture("game_menu_play_again"), 
                                                        m_resource.getTexture("game_menu_menu"));
                m_resource.getMusic("game_music").stop();
            }
            else
            {
                m_new_instance = m_game_over_menu_instance->gameMenuTick(renderWindow);
                if(m_new_instance == EGame)
                {
                    m_window_instance = EGame;
                    delete m_game_instance;
                    m_game_instance = new Game;
                }
            }
            break;
    }
}

//draw function either draws Game or Menu
void Window::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    switch(m_window_instance)
    {
        case EMenu: target.draw(*m_menu_instance); break;
        case EGame: target.draw(*m_game_instance); break;
        case EBreakMenu: target.draw(*m_break_menu_instance); break;
        case EGameOverMenu: target.draw(*m_game_over_menu_instance); break;
        case ESettings: target.draw(*m_settings_instance); break;
    }
}

//changes Instance
void Window::changeInstance()
{
    if (m_new_instance != m_window_instance)
        m_window_instance = m_new_instance;
}
