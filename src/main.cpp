#include <SFML/Graphics.hpp>
#include "Card.h"
#include "Slot.h"
#include "NumberSprite.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
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
    
    NumberSprite n(cardTexture, 125, {200.f, 100.f});

    const int CARD_WIDTH = 80;
    const int CARD_HEIGHT = 112;

    std::vector<Card> cards;
    for (int i = 0; i < 4; ++i) {
        sf::IntRect rect({i * CARD_WIDTH, 0}, {CARD_WIDTH, CARD_HEIGHT});
        cards.emplace_back(cardTexture, rect, sf::Vector2f(100.f + i * 180.f, 500.f));
    }

    std::vector<Slot> slots = {
        Slot({200.f, 250.f}),
        Slot({400.f, 250.f}),
        Slot({600.f, 250.f})
    };

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

                    if (event->is<sf::Event::MouseButtonReleased>()) {
                        for (auto& slot : slots) {
                            slot.setHighlight(false);

                            if (slot.contains(cards[i].getGlobalBounds())) {
                                // Snapuní karty na střed slotu
                                sf::Vector2f center = slot.getCenter();
                                sf::FloatRect bounds = cards[i].getGlobalBounds();

                                cards[i].setPosition({center.x - bounds.size.x / 2.f, center.y - bounds.size.y / 2.f});
                            }
                        }

                        eventHandled = true;
                   
                    } else {
                        // Highlightne slot pokud je nad ním karta
                        for (auto& slot : slots) {
                            slot.setHighlight(slot.contains(cards[i].getGlobalBounds()));
                        }
                    }
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
    
        // Čísla
        n.draw(window);

        // sloty
        for (auto& slot : slots) {
            slot.draw(window);
        }

        // karty
        for (auto& card : cards)
            card.draw(window);
        window.display();
    }
}

