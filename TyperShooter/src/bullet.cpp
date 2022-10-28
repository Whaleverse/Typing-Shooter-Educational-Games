#include "bullet.h"

Bullet::Bullet(float _x, float _y, b2World& world, int _targetX, int _targetY)
{
    x = _x;
    y = _y;
    bd.type = b2_dynamicBody;

    // TODO: Use some kind of method to convert pixel to world
    bd.position.Set(x, y);
    bd.bullet = true;

    // Add body and forces.
    body = world.CreateBody(&bd);
    b2Vec2 force = b2Vec2(_targetX,_targetY);
    body->SetLinearDamping(0.01f);
    body->ApplyLinearImpulse(force, body->GetWorldCenter(), true);
    body->SetGravityScale(0);
    // Render the Texture
    sf::Image missileImg;
    missileImg.loadFromFile("resources/Images/sphereBullet.png");
    setTexture(missileImg, int(x), int(y));
    bindBody();
}

// TODO: Impliment pos
// Sets a given image as the sprite's texture
void Bullet::setTexture(sf::Image img, int x, int y)
{
    sprite_texture.loadFromImage(img);
    sprite_texture.setSmooth(true);
    sprite.setTexture(sprite_texture);  // Create the sprite
    sprite.setScale(1.0f,1.0f);
    sprite.setPosition(x,y);
    rectSourceSprite = sf::IntRect(0, 0, 96, 96);
}

// Binds the body to a Box2d World
void Bullet::bindBody()
{
    // Create shape that the body will be binded to
    // TODO: Use some kind of method to convert pixel to world.
    b2PolygonShape missileBox;
    missileBox.SetAsBox(1.0f, 1.0f);

    // Create fixture definition for binding body to shape
    b2FixtureDef fd;
    fd.shape = &missileBox;
    fd.density = 1.0f;
    fd.friction = 0.0f;

    body->IsBullet();
    // Bind body
    body->CreateFixture(&fd);
    colliding = false;
    body->SetUserData(this);
}

// Updates the sprite based on the world
void Bullet::updatePosition()
{
    b2Vec2 position = body->GetPosition();
    sprite.setPosition(position.x, -1*position.y);
}

// Returns the sprite that will be drawn later on
sf::Sprite Bullet::getSprite()
{
    return sprite;
}

b2Vec2 Bullet::getPosition()
{
    return body->GetPosition();
}

b2Body* Bullet::getBody()
{
    return body;
}

