#include "Card.h"

Card::Card(const sf::Texture& texture, const sf::IntRect& rect, sf::Vector2f pos)
    : sprite_(texture, rect)   // SFML 3: Sprite má konstruktor s (texture, rect)
{
    sprite_.setPosition(pos);
    sprite_.setScale(sf::Vector2f{2.f, 2.f}); // musí být vektor, ne dva floaty
}

void Card::draw(sf::RenderWindow& window) {
    window.draw(sprite_);
}

void Card::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    // SFML 3: mouse eventy jsou oddělené typy, používáme is<>() a getIf<>()
    if (event.is<sf::Event::MouseButtonPressed>()) {
        const auto* data = event.getIf<sf::Event::MouseButtonPressed>();
        if (data && data->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
            if (sprite_.getGlobalBounds().contains(mousePos)) {
                isDragging_ = true;
                dragOffset_ = mousePos - sprite_.getPosition();
            }
        }
    }
    else if (event.is<sf::Event::MouseButtonReleased>()) {
        const auto* data = event.getIf<sf::Event::MouseButtonReleased>();
        if (data && data->button == sf::Mouse::Button::Left) {
            isDragging_ = false;
        }
    }
    else if (event.is<sf::Event::MouseMoved>()) {
        if (isDragging_) {
            sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
            sprite_.setPosition(mousePos - dragOffset_);
        }
    }
}

void Card::update() {
    // zatím nic
}

