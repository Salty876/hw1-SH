#ifndef PLAYER_H
#define PLAYER_H

#include "CollisionObject.h"
class Bullet;

class Player : public CMPUT350::CollisionObject
{
public:
    Player(CMPUT350::Point2D loc);

    // GameObject Functions
    void Initialize(CMPUT350::GameContext* context) override;
    void Update(CMPUT350::GameContext* context) override;
    void LateUpdate(CMPUT350::GameContext* context) override;
    bool HandleKeyEvent(CMPUT350::GameContext* context, char key) override;
    bool IsAlive() const override;
    void Kill() override;

    // Graphics Object Functions
    void RenderBackground(CMPUT350::GameContext* context) override;
    void RenderForeground(CMPUT350::GameContext* context) override;


    // Collision Object Functions
    void CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj) override;
    const CMPUT350::Rect& GetBounds() override;

private:
    bool mIsAlive;
    CMPUT350::Point2D mLocation;
    CMPUT350::Rect mBounds;


    float mPlayerSIze;
    float mMoveSpeed;

    std::weak_ptr<Bullet> mBullet1;
    std::weak_ptr<Bullet> mBullet2;

    void updateBounds();


};

#endif
