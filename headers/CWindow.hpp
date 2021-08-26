#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "CGame.hpp"
#include "CMenu.hpp"
#include "CBreakMenu.hpp"
#include "CGameOverMenu.hpp"
#include "CSettings.hpp"
#include "headerEnum.hpp"
#include "dataPath.hpp"


class CWindow : public sf::Drawable
{
    public:
        CWindow();
        void windowTick(sf::RenderWindow&);
        void changeInstance();
        void playClick();
    private:
        sf::Music m_game_music;
        WindowInstance m_window_instance = Menu;
        WindowInstance m_new_instance = Menu;
        CGame* m_game_instance = nullptr;
        CMenu* m_menu_instance = nullptr;
        CBreakMenu* m_break_menu_instance = nullptr;
        CGameOverMenu* m_game_over_menu_instance = nullptr;
        CSettings* m_settings_instance = nullptr;
        CSettings* m_settings = new CSettings;
        
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};