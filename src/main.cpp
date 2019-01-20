#include "main.h"

int main()
{

    ImGui::CreateContext();
    sf::RenderWindow window(sf::VideoMode(1280, 800), "ImGui test");
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);
 
    sf::Clock deltaClock;


    Screen screen;
    Processor p("etc/BLITZ", screen);
    Options<Screen> options(p);
    Registers<Screen> registers(p);
    Memory<Screen> memory(p);
    Debug<Screen> debug(p, screen);

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
        debug.update();

        window.clear();
        ImGui::Render();
        window.display();
    }
 
    ImGui::SFML::Shutdown();
}
