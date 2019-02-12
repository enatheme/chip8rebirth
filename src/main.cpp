#include "main.h"

#include <experimental/filesystem>
#include <iostream>

namespace fs = std::experimental::filesystem;

class FileBrowser
{
public:
    struct _file
    {
        std::string alias;
        fs::path path;
    };

    FileBrowser(fs::path p) : m_path(p) {}
    friend std::ostream & operator<<(std::ostream & strm, const FileBrowser & f)
    {
        for (auto & file : f.m_files)
        {
            if (fs::is_directory(file.path))
            {
                strm << "|> ";
            }
            strm << file.alias << " (" << file.path << ")\n";
        }
        return strm;
    }
    void update()
    {
        m_files.clear();
        if (m_path.has_parent_path())
        {
            m_files.push_back({
                "..",
                m_path.parent_path()
            });
        }
        for (const auto & entry : fs::directory_iterator(m_path))
        {
            m_files.push_back({
                entry.path().filename().string(),
                entry.path()
            });
        }
    }

    std::unique_ptr<Chip8> & display(std::unique_ptr<Chip8> & chip)
    {
        ImGui::Begin("File browser");
        for (const auto & file : m_files)
        {
            if (ImGui::Button(file.alias.c_str()))
            {
                if (move_to_child(file.alias))
                {
                    std::cout << "moved to "  << file.alias << std::endl;
                }
                else if (fs::exists(m_path / file.alias))
                {
                    chip = std::make_unique<Chip8>(m_path / file.alias);
                }
                else
                {
                    std::cout << "fail!" << std::endl;
                }
                update();
            }
        }
        ImGui::End();
        return chip;
    }

    bool move_to_child(const std::string & child)
    {
        if ((fs::exists(m_path / child)) && fs::is_directory(m_path / child))
        {
            m_path /= child;
            return true;
        }
        return false;
    }

    bool move_to_parent()
    {
        if (fs::exists(m_path.parent_path()))
        {
            m_path = m_path.parent_path();
            return true;
        }
        return false;
    }

private:
    fs::path m_path;
    std::vector<_file> m_files;
};

int main()
{
    ImGui::CreateContext();
    sf::RenderWindow window(sf::VideoMode(1280, 800), "ImGui test");
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);
 
    sf::Clock deltaClock;


    std::unique_ptr<Chip8> chip = nullptr;
    FileBrowser fb("etc");
    fb.update();

    bool is_open = false;

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
 
        fb.display(chip);
        if (chip)
        {
            chip->update();
        }

        window.clear();
        ImGui::Render();
        window.display();
    }
 
    ImGui::SFML::Shutdown();
}
