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
        sf::IntRect rect({i * CARD_WIDTH, 0}, {CARD_WIDTH, CARD_HEIGHT});
        cards.emplace_back(cardTexture, rect, sf::Vector2f(100.f + i * 180.f, 500.f));
    }

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            bool eventHandled = false;

            // Procházej karty odzadu (od nejvyšší k nejnižší)
            for (int i = static_cast<int>(cards.size()) - 1; i >= 0 && !eventHandled; --i) {

                // Pokud se tahá tato karta → zpracuj jen ji
                if (cards[i].isBeingDraged()) {
                    cards[i].handleEvent(*event, window);

                    if (event->is<sf::Event::MouseButtonReleased>())
                        eventHandled = false;
                    else
                        eventHandled = true;

                    continue;
                }

                // Kliknutí na kartu
                if (event->is<sf::Event::MouseButtonPressed>()) {
                    auto data = event->getIf<sf::Event::MouseButtonPressed>();
                    if (data && data->button == sf::Mouse::Button::Left) {
                        sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

                        if (cards[i].getGlobalBounds().contains(mousePos)) {
                            // Zpracuj klik pro tuto kartu
                            cards[i].handleEvent(*event, window);

                            // Zvedni kartu "nahoru"
                            Card top = cards[i];
                            cards.erase(cards.begin() + i);
                            cards.push_back(top);

                            eventHandled = true; // důležité — stop
                            break;                // konec smyčky
                        }
                    }
                }

                // pokud se event nevyřešil, zkus pohyb atd.
                if (!eventHandled) {
                    cards[i].handleEvent(*event, window);
                }
            }
        }

        for (auto& card : cards)
            card.update();

        window.clear(sf::Color(25, 25, 25));
        for (auto& card : cards)
            card.draw(window);
        window.display();
    }
}

