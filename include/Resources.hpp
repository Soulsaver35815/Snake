#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <utility>

class Resources 
{
    public:
        static Resources& getInstance();
        std::string DATA_PATH;

        sf::Texture& getTexture(std::string key);
        sf::SoundBuffer& getSound(std::string key);
        sf::Music& getMusic(std::string key);
        sf::Font& getFont(std::string key);
        //icons
        //sf::Image m_snake_icon;
    private:
        Resources();
        Resources(const Resources&) = delete;
        void operator=(const Resources&) = delete;
        static Resources m_instance;

        std::unordered_map<std::string, sf::Texture> texture_dict;
        std::unordered_map<std::string, sf::SoundBuffer> sound_dict;
        std::unordered_map<std::string, sf::Music> music_dict;
        std::unordered_map<std::string, sf::Font> font_dict;
};
