#include "Resources.hpp"
#include <iostream>

using std::ifstream, std::string, std::vector;
Resources Resources::m_instance;

Resources::Resources() :
DATA_PATH("resources/")
{
    ifstream file(DATA_PATH + "TexturePaths.txt");
    string line;
    
    if(!file.is_open())
        throw std::runtime_error("Can't open TexturePaths file");

    while(getline(file, line)) 
    {
        size_t pos = line.find(" ");
        string type = line.substr(0, pos - 1);

        line = line.substr(pos + 1);
        pos = line.find(" ");
        string key = line.substr(0, pos);
        string path = line.substr(pos + 1);

        if(type == "texture")
        {
            texture_dict[key].loadFromFile(DATA_PATH + path);
        }
        else if(type == "sound")
        {
            sound_dict[key].loadFromFile(DATA_PATH + path);
        }
        else if(type == "music")
        {
            music_dict[key].openFromFile(DATA_PATH + path);
        }
        else if(type == "font")
        {
            font_dict[key].loadFromFile(DATA_PATH + path);
        }
    }

    file.close();

    //icons
    //m_snake_icon.loadFromFile(DATA_PATH + "Icons/SnakeIcon.png");
}

sf::Texture& Resources::getTexture(std::string key)
{
    auto iter = texture_dict.find(key);
    if(iter == texture_dict.end())
        throw std::runtime_error("Texture " + key + " not found!");
    
    return iter->second;
}

sf::SoundBuffer& Resources::getSound(std::string key)
{
    auto iter = sound_dict.find(key);
    if(iter == sound_dict.end())
        throw std::runtime_error("Sound " + key + " not found!");
    
    return iter->second;
}

sf::Music& Resources::getMusic(std::string key)
{
    auto iter = music_dict.find(key);
    if(iter == music_dict.end())
        throw std::runtime_error("Music " + key + " not found!");
    
    return iter->second;
}

sf::Font& Resources::getFont(std::string key)
{
    auto iter = font_dict.find(key);
    if(iter == font_dict.end())
        throw std::runtime_error("Font " + key + " not found!");
    
    return iter->second;
}


Resources& Resources::getInstance()
{
    return m_instance;
}