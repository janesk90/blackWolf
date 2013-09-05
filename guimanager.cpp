#include "guimanager.h"

guiManager::guiManager():
    window(sf::VideoMode(800, 800), "Black Wolf")    
{
    window.setFramerateLimit(60);

    //icon.loadFromFile("Graphics/Boardbrown.jpg");
    //window.setIcon(32,32,icon.getPixelsPtr());
}

void guiManager::run()
{
    boardMaster boss(window, desktop);

    window.resetGLStates();
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            desktop.HandleEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();            
        }

        desktop.Update(clock.restart().asSeconds());

        window.clear();
        boss.display();
        sfgui_.Display(window);
        window.display();
    }

}


