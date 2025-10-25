#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

class NumberSprite {
public:
    NumberSprite(const sf::Texture& texture, int value, sf::Vector2f position);
    void setValue(int newValue);
    void draw(sf::RenderWindow& window);

private:
    void buildDigits(int value);

    const sf::Texture& texture_;
    std::vector<sf::Sprite> digits_;
    sf::Vector2f position_;

    // rozměry a umístění číslic v texture
    static constexpr int DIGIT_WIDTH = 64;
    static constexpr int DIGIT_HEIGHT = 112;
    static constexpr int DIGIT_START_X = 384;
    static constexpr int DIGIT_START_Y = 912;
};
