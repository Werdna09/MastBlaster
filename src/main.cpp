#include <SFML/Graphics.hpp>
#include "Card.h"
#include <vector>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({1000, 700}), "Math Blaster – Card Drag Test");
    window.setFramerateLimit(60);

    sf::Texture cardTexture;
    if (!cardTexture.loadFromFile("../assets/cards.png")) {
        std::cerr << "Nelze načíst cards.png\n";
        return 1;
    }

    const int CARD_WIDTH = 80;
    const int CARD_HEIGHT = 112;

    std::vector<Card> cards;
    for (int i = 0; i < 4; ++i) {
        // SFML 3: používá dva vektory – pozici a velikost
        sf::IntRect rect({i * CARD_WIDTH, 0}, {CARD_WIDTH, CARD_HEIGHT});
        cards.emplace_back(cardTexture, rect, sf::Vector2f(100.f + i * 180.f, 500.f));
    }

    while (window.isOpen()) {
        // SFML 3: pollEvent() vrací std::optional<Event>
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // SFML 3: klik myší = Event::MouseButtonPressed
            if (event->is<sf::Event::MouseButtonPressed>()) {
                auto data = event->getIf<sf::Event::MouseButtonPressed>();
                if (data && data->button == sf::Mouse::Button::Left) {
                    for (auto& card : cards)
                        card.handleEvent(*event, window);
                }
            }

            // ostatní typy (pohyb, puštění tlačítka)
            for (auto& card : cards)
                card.handleEvent(*event, window);
        }

        for (auto& card : cards)
            card.update();

        window.clear(sf::Color(25, 25, 25));
        for (auto& card : cards)
            card.draw(window);
        window.display();
    }
}

