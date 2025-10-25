#include "NumberSprite.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>

NumberSprite::NumberSprite(const sf::Texture& texture, int value, sf::Vector2f position)
    : texture_(texture), position_(position)
{
    buildDigits(value);
}

void NumberSprite::setValue(int newValue) {
    buildDigits(newValue);
}

void NumberSprite::buildDigits(int value) {
    digits_.clear();
    std::string str = std::to_string(value);
    float xOffset = 0.f;

    for (char c : str) {
        int digit = c - '0';
        sf::IntRect rect(
            sf::Vector2i(DIGIT_START_X + digit * DIGIT_WIDTH, DIGIT_START_Y),
            sf::Vector2i(DIGIT_WIDTH, DIGIT_HEIGHT)
        );

        sf::Sprite sprite(texture_, rect);
        sprite.setPosition(sf::Vector2f(position_.x + xOffset, position_.y));
        digits_.push_back(sprite);
        xOffset += DIGIT_WIDTH;
    }
}

void NumberSprite::draw(sf::RenderWindow& window) {
    for (auto& d : digits_)
        window.draw(d);
}

