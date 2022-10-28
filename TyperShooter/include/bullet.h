#ifndef BULLET_H
#define BULLET_H

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <QPixmap>

class Bullet
{
    b2Body* body;
    b2BodyDef bd;
    sf::RenderTexture texture;
    sf::Texture sprite_texture;
    sf::Sprite sprite;
    sf::Clock spriteClock;
    sf::IntRect rectSourceSprite;
    float x, y, targetX, targetY;
    float w, h;
    bool colliding;

    // Sets a given image as the sprite's texture
    void setTexture(sf::Image img, int x, int y);

    // Binds the body to a Box2d World
    void bindBody();

public:
    Bullet(float _x, float _y, b2World& world, int _targetX, int _targetY);

    sf::Sprite getSprite();

    void updatePosition();

    b2Vec2 getPosition();

    b2Body* getBody();
};

#endif // BULLET_H
