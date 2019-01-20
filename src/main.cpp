#include "main.h"

int main()
{

    ImGui::CreateContext();
    sf::RenderWindow window(sf::VideoMode(1280, 800), "ImGui test");
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);
 
    sf::Clock deltaClock;

    Screen screen;
    screen.draw(2, 2, 2);
    screen.draw(8, 10, 10);
    Processor p("etc/BLITZ", screen);
    Options<Screen> options(p);
    Registers<Screen> registers(p);
    Memory<Screen> memory(p);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
 
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

 
        ImGui::SFML::Update(window, deltaClock.restart());
 
        screen.update();
        options.update();
        registers.update();
        memory.update();

        window.clear();
        ImGui::Render();
        window.display();
    }
 
    ImGui::SFML::Shutdown();
}
