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

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
 
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

 
        ImGui::SFML::Update(window, deltaClock.restart());
 
        static bool animated = true;
        bool terminal = false;
        static float values[80] = { 0 };
        static int values_offset = 0;

        ImGui::Begin("Options");
        ImGui::Checkbox("Terminal output", &terminal);
        ImGui::Checkbox("Animated", &animated);
        ImGui::PlotLines("Lines", values, 80, values_offset, "", 0, 100, ImVec2(0,80));
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        if (animated)
        {
            values[values_offset] = ImGui::GetIO().Framerate;
            values_offset = (values_offset + 1) % 80;
        }
        ImGui::End();


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

        ImGui::Begin("Registers");
        p.display_register(f);
        ImGui::End();
        ImGui::Begin("Memory");
        p.display_memory(f);
        ImGui::End();
        screen.update();
        window.clear();
        ImGui::Render();
        window.display();
    }
 
    ImGui::SFML::Shutdown();
}
