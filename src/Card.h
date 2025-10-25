#pragma once
#include <SFML/Graphics.hpp>

class Card {
public:
    Card(const sf::Texture& texture, const sf::IntRect& rect, sf::Vector2f pos);

    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update();

private:
    sf::Sprite sprite_;
    bool isDragging_ = false;
    sf::Vector2f dragOffset_;
};

