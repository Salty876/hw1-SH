#include "GameEngine.h"
#include "GameContext.h"
#include "DrawContext.h"
#include "GraphicsObject.h"
#include "CollisionObject.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

/// @brief
namespace CMPUT350 {
#include "FontData.h"

GameEngine::GameEngine(unsigned int width, unsigned int height, const std::string& name) {
    mFont = std::make_shared<sf::Font>();
    if (!mFont->openFromMemory(&_font, _font_len))
    {
        fprintf(stderr, "WARNING: Font did not load.\n");
    }

    mWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode(sf::Vector2u(width, height)), name);
    mWindow->setFramerateLimit(30);
}

GameEngine::~GameEngine() {
    if (mWindow->isOpen()) mWindow->close();
}

void GameEngine::AddGameObject(std::shared_ptr<GameObject> gameObject) {
    mNewObjects.push_back(gameObject);
}

/**
 * @method Run
 * @arguments None
 * @description Gives control to the game engine. Will not return until the game window is closed or
 * all objects have been destroyed.
 */
void GameEngine::Run() {
    DrawContext drawContext = DrawContext(mWindow, mFont);
    GameContext gameContext;

    gameContext.mEngineView = this;
    gameContext.ScreenContext = &drawContext;

    while (true) {
        if (mWindow->isOpen() == false) break;

        // 0. Remove any objects that are now dead
        for (int i = mObjects.size() - 1; i >= 0; --i) {
            if (!mObjects[i]->IsAlive()) {
                // Delete using swap-and-pop
                mObjects[i] = std::move(mObjects.back());
                mObjects.pop_back();
            }
        }

        // 1. Activate and initialize any objects added during the last frame
        while (mNewObjects.size() != 0) {
            mNewObjects.back()->Initialize(&gameContext);
            mObjects.push_back(mNewObjects.back());
            mNewObjects.pop_back();
        }

        // 2. Process events
        ProcessEvents(&gameContext);

        // 3. Update game objects
        for (int i = mObjects.size() - 1; i >= 0; --i) {
            mObjects[i]->Update(&gameContext);
        }

        // 4. Process collision events
        ProcessCollisions(&gameContext);

        // 5. Late updates
        for (int i = mObjects.size() - 1; i >= 0; --i) {
            mObjects[i]->LateUpdate(&gameContext);
        }

        // Clear window
        mWindow->clear(sf::Color::Black);

        // Collect all drawable objects
        std::vector<GraphicsObject*> graphicsObjects;
        for (int i = mObjects.size() - 1; i >= 0; --i) {
            // Use std::dynamic_cast to test pointer type
            GraphicsObject* obj = dynamic_cast<GraphicsObject*>(mObjects[i].get());

            if (obj != nullptr) graphicsObjects.push_back(obj);
        }

        // 6. Render background
        for (int i = graphicsObjects.size() - 1; i >= 0; --i) {
            graphicsObjects[i]->RenderBackground(&gameContext);
        }

        // 7. Render foreground
        for (int i = graphicsObjects.size() - 1; i >= 0; --i) {
            graphicsObjects[i]->RenderForeground(&gameContext);
        }

        // Actually render to window
        mWindow->display();
    }

    return; // Window is closed; terminate program
}

void GameEngine::ProcessEvents(GameContext *context) {
    while (const std::optional event = mWindow->pollEvent())
    {   
        if (event->is<sf::Event::Closed>()) {
            mWindow->close();
        }
        else if (event->is<sf::Event::Resized>()) {} // No action needed
        else if (const auto* keyPressed = event->getIf<sf::Event::TextEntered>()) {   
            for (int i = mObjects.size() - 1; i >= 0; --i) {
                mObjects[i]->HandleKeyEvent(context, keyPressed->unicode);
            }
        }
    }
}

void GameEngine::ProcessCollisions(GameContext *context) {
    std::vector<std::shared_ptr<CollisionObject>> collisionObjects;
    for (int i = mObjects.size() - 1; i >= 0; --i) {
        // Use std::dynamic_pointer_cast to test pointer type
        std::shared_ptr<CollisionObject> obj = std::dynamic_pointer_cast<CollisionObject>(mObjects[i]);

        if (obj != nullptr) collisionObjects.push_back(obj);
    }

    for (int i = 0; i < collisionObjects.size(); ++i) {
        // Note: the inner loop starts from i + 1 as those collisions
        // are already tested during the previous iterations.
        for (int j = i + 1; j < collisionObjects.size(); ++j) {
            Rect iBounds = collisionObjects[i]->GetBounds();
            Rect jBounds = collisionObjects[j]->GetBounds();

            // AABB Collision Detection
            bool condition = 
                (iBounds.topLeft.x < jBounds.topLeft.x + jBounds.width &&
                jBounds.topLeft.x < iBounds.topLeft.x + iBounds.width &&
                iBounds.topLeft.y < jBounds.topLeft.y + jBounds.height &&
                jBounds.topLeft.y < iBounds.topLeft.y + iBounds.height);
            
            if (condition) {
                // Alert both objects
                collisionObjects[i]->CollisionEnter(collisionObjects[j]);
                collisionObjects[j]->CollisionEnter(collisionObjects[i]);
            }
        }
    }
    return;
}

}  // namespace CMPUT350
