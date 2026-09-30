#include "Bullet.h"
#include "DrawContext.h"
#include "Player.h"
#include <cmath>

Bullet::Bullet(CMPUT350::Point2D location, CMPUT350::Point2D heading, bool player)
{
    this->mCurrentLocation = location;
    this->mPreviousLocation = location;
    this->mHeading = heading;
    this->mIsPlayerBullet = player;
    this->mIsAlive = true;

    // Since collision in the same frame as creation is not possible
    // initiate the bound as a point and update it in the first Update()
    this->mBounds = CMPUT350::Rect(location, location);

    // Using a hard-coded for now, we could
    // make this a parameter if needed
    this->mBulletSpeed = 15.0f;
}

bool Bullet::IsPlayerBullet()
{
    return mIsPlayerBullet;
}

void Bullet::Initialize(CMPUT350::GameContext* context)
{
    // No action required
}

void Bullet::Update(CMPUT350::GameContext* context)
{
    mPreviousLocation = mCurrentLocation;
    mCurrentLocation += mBulletSpeed * mHeading; // Speed * Direction

    bool condition = (
        mCurrentLocation.x < 0 ||
        mCurrentLocation.x > context->ScreenContext->GetWindowWidth() ||
        mCurrentLocation.y < 0 ||
        mCurrentLocation.y > context->ScreenContext->GetWindowHeight()
    );

    if (condition) Kill();
    // Kill any bullets that escape the screen
    // We do this here instead of LateUpdate()
    // to avoid unecessary collision checking logic.

    this->mBounds = CMPUT350::Rect(mPreviousLocation, mCurrentLocation);
    return;
}

void Bullet::LateUpdate(CMPUT350::GameContext* context)
{
    // No action required
}

bool Bullet::HandleKeyEvent(CMPUT350::GameContext* context, char key)
{
    return false;
}

void Bullet::RenderBackground(CMPUT350::GameContext* context)
{
    // No action required
}

void Bullet::RenderForeground(CMPUT350::GameContext* context)
{
    CMPUT350::DrawContext* api = context->ScreenContext;
    api->DrawLine(mPreviousLocation, mCurrentLocation, 5.0f, CMPUT350::Colors::white);
}

void Bullet::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj)
{   
    std::shared_ptr<Player> mObj = std::dynamic_pointer_cast<Player>(obj);
    if (mObj) // Colliding Object is Player
    {
        if (IsPlayerBullet()) {
            return;
        }
    }

    Kill();
}

void Bullet::Kill()
{
    this->mIsAlive = false;
}

bool Bullet::IsAlive() const
{
    return mIsAlive;
}

const CMPUT350::Rect& Bullet::GetBounds()
{
    return mBounds;
}
