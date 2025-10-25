#include "Slot.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

Slot::Slot(sf::Vector2f pos, sf::Vector2f size){
    rect_.setPosition(pos);
    rect_.setSize(size);
    rect_.setFillColor(sf::Color(60,60,60));
    rect_.setOutlineColor(sf::Color(150,150,150));
    rect_.setOutlineThickness(2.f);
}

void Slot::draw(sf::RenderWindow& window) {
    window.draw(rect_);
}

bool Slot::contains(const sf::FloatRect& cardBounds) const {
    sf::FloatRect slotBounds = rect_.getGlobalBounds();
    return slotBounds.findIntersection(cardBounds).has_value();
}

sf::Vector2f Slot::getCenter() const {
    return rect_.getPosition() + rect_.getSize() / 2.f;
}

void Slot::setHighlight(bool active) {
    isHightlited_ = active;
    if (active) {
        rect_.setFillColor(sf::Color(100,100,140));
    } else{
        rect_.setFillColor(sf::Color(60,60,60));
    }
}


