#ifndef Missile_H
#define Missile_H

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <QPixmap>
#include <QTimer>



class Missile : public QObject
{
    Q_OBJECT
    b2Body* body;
    b2BodyDef bd;
    sf::RenderTexture texture;
    sf::Texture sprite_texture;
    sf::Sprite sprite;
    sf::Clock spriteClock;
    sf::IntRect rectSourceSprite;
    float xPos, yPos;
    float w, h;
    bool colliding;
    int frameExplosionIndex;
    int frameMissileIndex;
    QTimer* timer;
    int const kExplosionFrames = 12;
    int const kMissileFrames = 6;
    int animatingIndex;

    // Sets a given image as the sprite's texture
    void setTexture(sf::Image img, float x, float y);

    // Binds the body to a Box2d World
    void bindBody();

public:
    Missile(QObject* parent, float _x, float _y, b2World& world, int height, int width);

    // On Contact
    void contact();

    // Sets the sprite image to an exploding one
    void explode(b2World* world, b2Body* ground);

    // Updates the sprite based on the world
    void updatePosition();

    // Returns the missile sprite
    sf::Sprite getSprite();

    // Returns a contact list for the missile
    b2ContactEdge* getContactList();

    void stopAnimating();
    void resumeAnimating();

private:
    void animateExplosion();
    void animateMissile();

signals:
    void groundHit();
    void deleteMissile(Missile* missile);


};

#endif // Missile_H
