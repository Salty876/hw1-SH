#include <cassert>
#include "Player.h"
#include "Bullet.h"

Player::Player(CMPUT350::Point2D loc)
{
    // TODO: Update code
    mLocation = loc;
    mPlayerSIze = 40.f;
    mMoveSpeed = 10.f;
    mIsAlive = true;

    updateBounds();
}

void Player::Initialize(CMPUT350::GameContext* context)
{
    // Dont gotta Init
}

void Player::Update(CMPUT350::GameContext* context)
{
    //keyboard udpates player
}

void Player::LateUpdate(CMPUT350::GameContext* context)
{
    //No late update
}

bool Player::HandleKeyEvent(CMPUT350::GameContext* context, char key)
{
    float halfSize = mPlayerSIze / 2.f;

    if (key == 'a' || key == 'A') {
        mLocation.x -= mMoveSpeed;

        // Stop player from leaving left side of screen
        if (mLocation.x - halfSize < 0) {
            mLocation.x = halfSize;
        }
        updateBounds();
        return true;
    }

    if (key == 'd' || key == 'D') {
        mLocation.x += mMoveSpeed;

        float windowWidth = context->ScreenContext->GetWindowWidth();


        // Stop player from leaving right side of screen
        if (mLocation.x - halfSize > windowWidth) {
            mLocation.x = windowWidth - halfSize;
        }
        updateBounds();
        return true;
    }

    if (key == ' ') {
        CMPUT350::Point2D bulletLocation(mLocation.x, mLocation.y);
        CMPUT350::Point2D bulletHeading(0.f, -1.f);

        if (mBullet1.expired()) {
            std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(bulletLocation, bulletHeading, true);
            context->mEngineView->AddGameObject(bullet);
            mBullet2 = bullet;
            return true;
        }
        return true;
    }
    return false;

}

void Player::RenderBackground(CMPUT350::GameContext* context)
{
    // No bg to render
}

void Player::RenderForeground(CMPUT350::GameContext* context)
{
    CMPUT350::DrawContext *drawMachine = context->ScreenContext;
    float halfSize = mPlayerSIze / 2.f;

    CMPUT350::Point2D nose(mLocation.x, mLocation.y - halfSize);
    CMPUT350::Point2D leftBottom(mLocation.x - halfSize, mLocation.y + halfSize);
    CMPUT350::Point2D rightBottom(mLocation.x + halfSize, mLocation.y + halfSize);

    drawMachine->DrawLine(leftBottom, nose, 3.f, CMPUT350::Colors::white);
    drawMachine->DrawLine(rightBottom, nose, 3.f, CMPUT350::Colors::white);
    drawMachine->DrawLine(rightBottom, leftBottom, 3.f, CMPUT350::Colors::white);




}

void Player::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj)
{
    std::shared_ptr<Bullet> bullet = std::dynamic_pointer_cast<Bullet>(obj);

    if (bullet != nullptr) {
        // ignore the player bullets
        if (!bullet->IsPlayerBullet()) {
            Kill();
        }
    }
}

void Player::Kill()
{
    mIsAlive = false;
}

bool Player::IsAlive() const
{
    // TODO: Update code
    return mIsAlive;
}

const CMPUT350::Rect& Player::GetBounds()
{
    return mBounds;
}

void Player::updateBounds() {
    CMPUT350::Point2D topLeft(mLocation.x - mPlayerSIze / 2.f , mLocation.y - mPlayerSIze / 2.f);
    mBounds = CMPUT350::Rect(topLeft, mPlayerSIze, mPlayerSIze);
}


