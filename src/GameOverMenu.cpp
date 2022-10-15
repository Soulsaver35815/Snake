#include "GameOverMenu.hpp"

GameOverMenu::GameOverMenu(Game* game, sf::Texture& menu_texture, sf::Texture& button_one_texture, sf::Texture& button_two_texture):
GameMenu::GameMenu(game, menu_texture, button_one_texture, button_two_texture, 490, 610)
{
    //set game over sound properties
    m_game_over_sound.setBuffer(m_resource.getSound("game_over_buffer"));
    m_game_over_sound.setVolume(*m_settings_values.getVolumeGameOver());
    m_game_over_sound.play();
}

