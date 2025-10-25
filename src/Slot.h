#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

class Slot {
public:
    Slot(sf::Vector2f pos, sf::Vector2f size = {120.f, 160.f});

    void draw(sf::RenderWindow& window);
    bool contains(const sf::FloatRect& cardBounds) const;
    sf::Vector2f getCenter() const;
    void setHighlight(bool active);

private:
    sf::RectangleShape rect_;
    bool isHightlited_ = false;
};
