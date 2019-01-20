#include "main.h"

int main()
{

    ImGui::CreateContext();
    sf::RenderWindow window(sf::VideoMode(1024, 840), "ImGui test");
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);
 
    sf::Clock deltaClock;
    bool terminal = false;
    bool animated = true;
    float values[80] = { 0 };
    int values_offset = 0;

    Screen screen;
    screen.draw(2, 2, 2);
    screen.draw(8, 10, 10);
    Processor p("etc/BLITZ", screen);
    Options<Screen> options(p);
    Registers<Screen> registers(p);

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
 
        static bool animated = true;
        bool terminal = false;
        static float values[80] = { 0 };
        static int values_offset = 0;

        auto f = [terminal](const char * text, size_t size)
        { 
            if (terminal)
            {
                std::cout << text;
            }
            else
            {
                ImGui::TextUnformatted(text, text + size);
            }
        };

        ImGui::Begin("Memory");
        p.display_memory(f);
        ImGui::End();
        screen.update();
        options.update();
        registers.update();
        window.clear();
        ImGui::Render();
        window.display();
    }
 
    ImGui::SFML::Shutdown();
}
