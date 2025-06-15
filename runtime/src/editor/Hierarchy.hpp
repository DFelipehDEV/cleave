#pragma once
#include <memory>

#include "GameView.hpp"
#include "entities/Entity.hpp"

class Hierarchy {
public:
    Hierarchy(std::shared_ptr<GameView> gameView)
        : m_gameView(gameView),
          m_selectedEntity(gameView && gameView->GetScene()
                               ? gameView->GetScene()->GetRoot()
                               : nullptr) {}
    ~Hierarchy() = default;

    void OnRender();

    Scene* GetScene();
    void SetScene(std::shared_ptr<Scene> scene);

    Entity* GetSelectedEntity();

private:
    std::shared_ptr<GameView> m_gameView;
    Entity* m_selectedEntity;
};