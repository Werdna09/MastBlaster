#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

class Card {
public:
    Card(const sf::Texture& texture, const sf::IntRect& rect, sf::Vector2f pos);

    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update();

    void setPosition(sf::Vector2f pos){
        sprite_.setPosition(pos);
    }

    sf::FloatRect getGlobalBounds() const {
        return sprite_.getGlobalBounds();
    }

    bool isBeingDraged() const {
        return isDragging_;
    }

private:
    sf::Sprite sprite_;
    bool isDragging_ = false;
    sf::Vector2f dragOffset_;
};

