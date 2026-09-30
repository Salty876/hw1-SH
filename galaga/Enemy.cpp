#include "Enemy.h"
#include "Bullet.h"

Enemy::Enemy(CMPUT350::Point2D loc)
{   
    float mEnemySize = 20.0f; // This can be changed to a parameter

    this->mLocation = loc;
    this->mIsAlive = true;

    // Input is the coordinates of the center of the Enemy
    // Rectangle class requires top-left point, so compute center_x - width / 2 and center_y - width / 2
    CMPUT350::Point2D topLeft = CMPUT350::Point2D(loc.x - (mEnemySize / 2.0f), loc.y - (mEnemySize / 2.0f));
    this->mBounds = CMPUT350::Rect(topLeft, mEnemySize, mEnemySize); // Enemy is a square of length mEnemySize
}

void Enemy::Initialize(CMPUT350::GameContext* context)
{
    // No action required
}

void Enemy::Update(CMPUT350::GameContext* context)
{
    // No action required
}

void Enemy::LateUpdate(CMPUT350::GameContext* context)
{
    // No action required
}

bool Enemy::HandleKeyEvent(CMPUT350::GameContext* context, char key)
{
    return false;
}

void Enemy::RenderBackground(CMPUT350::GameContext* context)
{
    // No action required
}

void Enemy::RenderForeground(CMPUT350::GameContext* context)
{
    CMPUT350::DrawContext* api = context->ScreenContext;
    api->DrawRect(GetBounds(), CMPUT350::Colors::red);
}

void Enemy::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj)
{
    // Test the type of colliding object
    Bullet* ptr = dynamic_cast<Bullet*>(obj.get());
    if (ptr != nullptr) Kill();
}

void Enemy::Kill()
{
    mIsAlive = false;
}

bool Enemy::IsAlive() const
{
    return mIsAlive;
}

const CMPUT350::Rect& Enemy::GetBounds()
{
    return mBounds;
}
