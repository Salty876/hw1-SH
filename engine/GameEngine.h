
#ifndef GAMEENGINE_H
#define GAMEENGINE_H

namespace CMPUT350 {
class GameEngine;
}

#include "EngineView.h"
#include "GameObject.h"
#include "MathUtil.h"
#include <SFML/Graphics.hpp>

namespace CMPUT350 {

class DrawContext;

class GameEngine : public EngineView {
public:
    // 1. Generates a width * height sized game window
    // 2. Load any required resources, including the font
    // 3. Limit frame-rate to 30 FPS
    GameEngine(unsigned int width, unsigned int height, const std::string& name);

    // Closes game window
    ~GameEngine();

    GameEngine(const GameEngine&) = delete;             // Prevent copy-construction
    GameEngine(GameEngine&&) = delete;                  // Prevent move-construction
    GameEngine& operator=(const GameEngine&) = delete;  // Prevent assignment
    GameEngine& operator=(GameEngine&&) = delete;       // Prevent move-assignment

    // Add a game object; store it in a temporary list of new objects
    // that will be rendered in the next frame
    void AddGameObject(std::shared_ptr<GameObject> gameObject) override;

    // 1. Remove dead objects
    // 2. Add and initialize new objects
    // 3. Process any SFML events to capture key hits
    // 4. Update all game objects
    // 5. Process collisions
    // 6. Do late updates on all game objects
    // 7. Render the background
    // 8. Render the foreground
    // 9. Call display()
    // Optionally, we can add pause functionality here.
    void Run();

private:
    std::shared_ptr<sf::RenderWindow> mWindow;
    std::shared_ptr<sf::Font> mFont;
    std::vector<std::shared_ptr<GameObject>> mObjects;
    std::vector<std::shared_ptr<GameObject>> mNewObjects;

    // Helper functions for Run()
    void ProcessEvents(GameContext *context);
    void ProcessCollisions(GameContext *context);
};

}  // namespace CMPUT350

#endif  // GAMEENGINE_H
