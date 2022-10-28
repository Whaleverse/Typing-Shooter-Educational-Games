#include "missile.h"
#include <iostream>

Missile::Missile(QObject* parent, float _x, float _y, b2World& world, int height, int width) :
    QObject(parent)
{
    xPos = _x;
    yPos = _y;
    bd.type = b2_dynamicBody;

    // TODO: Use some kind of method to convert pixel to world
    bd.position.Set(xPos, yPos);

    // Add body to the worldi
    body = world.CreateBody(&bd);

    // Render the Texture
    sf::Image missileImg;
    missileImg.loadFromFile("resources/Images/missileSpriteSheetRotated.png");
    setTexture(missileImg, xPos, yPos);
    bindBody();

    animatingIndex = 0;
}

// TODO: Impliment po
// Sets a given image as the sprite's texture
void Missile::setTexture(sf::Image img, float x, float y)
{
    sprite_texture.loadFromImage(img);
    sprite_texture.setSmooth(true);
    sprite.setTexture(sprite_texture);  // Create the sprite

    frameMissileIndex = -1;
    timer = new QTimer(this);
    animateMissile();
    rectSourceSprite = sf::IntRect(0, 0, 96, 96);
}

void Missile::animateMissile()
{
    frameMissileIndex += 1;
    if(frameMissileIndex < kMissileFrames)
    {
        sprite.setTextureRect(sf::IntRect(22*frameMissileIndex + 10, 3, 21, 25));
        sprite.setScale(2.0f,2.0f);
        sprite.setPosition(sf::Vector2f(0.0f, 0.0f));
        timer->singleShot(500,this, &Missile::animateMissile);
    }
}

// Binds the body to a Box2d World
void Missile::bindBody()
{
    // Create shape that the body will be binded to
    b2PolygonShape missileBox;
    missileBox.SetAsBox(40.0f, 40.0f);

    // Create fixture definition for binding body to shape
    b2FixtureDef fd;
    fd.shape = &missileBox;
    fd.density = 1.0f;
    fd.friction = 0.3f;

    // Bind body
    body->CreateFixture(&fd);
    colliding = false;
    body->SetUserData(this);
}

// On Contact
void Missile::contact()
{
    colliding = !colliding;
}

// Sets the sprite image to an exploding one
void Missile::explode(b2World* world, b2Body* ground)
{
    frameExplosionIndex = 0;
    animatingIndex = 1;
    timer = new QTimer(this);

    sf::Image explodingImage;
    explodingImage.loadFromFile("resources/Images/explosion.png");
    sprite_texture.loadFromImage(explodingImage);
    sprite_texture.setSmooth(true);
    sprite.setOrigin(30,12);
    sprite.setTexture(sprite_texture);
    sprite.setTextureRect(sf::IntRect(0,0,96,96));
    if(ground->GetContactList())
    {
        emit groundHit();
        colliding = true;
    }
    world->DestroyBody(body);
    animateExplosion();
}

void Missile::animateExplosion()
{
    frameExplosionIndex += 1;
    if( frameExplosionIndex < kExplosionFrames)
    {
        sprite.setTextureRect(sf::IntRect(frameExplosionIndex*96, 0, 96, 96));
        sprite.setScale(2.0f,2.0f);
        timer->singleShot(30, this, &Missile::animateExplosion);
    }
    else
    {
        //temp fix must destroy missle
        sprite.setTextureRect(sf::IntRect(-10,-10,0,0));
        emit deleteMissile(this);
    }
}

// Updates the sprite based on the world
void Missile::updatePosition()
{
    b2Vec2 position = body->GetPosition();
    float32 angle = body->GetAngle();
    sprite.rotate(angle);
    sprite.setPosition(position.x, -1*position.y);
}

// Returns the sprite that will be drawn later on
sf::Sprite Missile::getSprite(){
    return sprite;
}

b2ContactEdge* Missile::getContactList()
{
    return body->GetContactList();
}

void Missile::stopAnimating()
{
    timer->stop();
}

void Missile::resumeAnimating()
{
    timer->start();
    if(animatingIndex == 0)
    {
        animateMissile();
    }
    else
    {
        animateExplosion();
    }
}
