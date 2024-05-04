// Zombie Shootdown.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include <vector>

using namespace std;
using namespace sf;
using namespace sfp;

const float KB_SPEED = 0.5;

void LoadTex(Texture& tex, string filename) {
    if (!tex.loadFromFile(filename)) {
        cout << "Could not load " << filename << endl;
    }
}

void Moveman(PhysicsSprite& man, int elapsedMS) {
    if (Keyboard::isKeyPressed(Keyboard::Right)) {
        Vector2f newPos(man.getCenter());
        newPos.x = newPos.x + (KB_SPEED * elapsedMS);
        newPos.y -= 0.5; 
        man.setCenter(newPos);
    }
    if (Keyboard::isKeyPressed(Keyboard::Left)) {
        Vector2f newPos(man.getCenter());
        newPos.x = newPos.x - (KB_SPEED * elapsedMS);
        newPos.y -= 0.5;
        man.setCenter(newPos);
    }
    if (Keyboard::isKeyPressed(Keyboard::Down)) {
        Vector2f newPos(man.getCenter());
        newPos.y = newPos.y + (KB_SPEED * elapsedMS);
        newPos.x -= 0.5;
        man.setCenter(newPos);
    }
    if (Keyboard::isKeyPressed(Keyboard::Up)) {
        Vector2f newPos(man.getCenter()); 
        newPos.y = newPos.y - (KB_SPEED * elapsedMS);
        newPos.x -= 0.5;
        man.setCenter(newPos); 
    } 
}

int main()
{
    RenderWindow window(VideoMode(1600, 1200), "Zombie Shootdown");
    World world(Vector2f(0, 0));
    int score(0);
    int bullets(5);

    PhysicsSprite& man = *new PhysicsSprite();
    Texture cbowTex;
    LoadTex(cbowTex, "images/man.png");
    man.setTexture(cbowTex); 
    Vector2f sz = man.getSize(); 
    man.setCenter(Vector2f(400, 
        600 - (sz.y / 2)));

    PhysicsSprite bullet; 
    Texture bulletTex; 
    LoadTex(bulletTex, "images/bullet.png");
    bullet.setTexture(bulletTex); 
    bool drawingbullet(false);

    PhysicsRectangle top;
    top.setSize(Vector2f(800, 10));
    top.setCenter(Vector2f(400, 5));
    top.setStatic(true);
    world.AddPhysicsBody(top);

    Texture redTex;
    LoadTex(redTex, "images/zombie.png");
    PhysicsShapeList<PhysicsSprite> zombies;
    for (int i(0); i < 6; i++) {
        PhysicsSprite& zombie = zombies.Create();
        zombie.setTexture(redTex); 
        int x = 50 + ((700 / 5) * i);
        Vector2f sz = zombie.getSize(); 
        zombie.setCenter(Vector2f(x, 20 + (sz.y / 2))); 
        zombie.setVelocity(Vector2f(0.25, 0)); 
        world.AddPhysicsBody(zombie);
        zombie.onCollision =
            [&drawingbullet, &world, &bullet, &zombie, &zombies, &score]
            (PhysicsBodyCollisionResult result) {
            if (result.object2 == bullet) {
                drawingbullet = false;
                world.RemovePhysicsBody(bullet);
                world.RemovePhysicsBody(zombie); 
                zombies.QueueRemove(zombie);
                score += 10;
            }
            };
    }

    top.onCollision = [&drawingbullet, &world, &bullet]
    (PhysicsBodyCollisionResult result) {
        drawingbullet = false; 
        world.RemovePhysicsBody(bullet);
        };

    Text scoreText;
    Font font;
    if (!font.loadFromFile("comic.ttf")) {
        cout << "Couldn't load font sans.ttf" << endl;
        exit(1);
    }
    scoreText.setFont(font);
    Text bulletCountText;
    bulletCountText.setFont(font); 

    Clock clock;
    Time lastTime(clock.getElapsedTime());
    Time currentTime(lastTime);

    long zombieMS(0);
    while ((bullets > 0) || drawingbullet) {
        currentTime = clock.getElapsedTime();
        Time deltaTime = currentTime - lastTime;
        long deltaMS = deltaTime.asMilliseconds();
        zombieMS = zombieMS + deltaMS;
        if (deltaMS > 9) {
            lastTime = currentTime;
            world.UpdatePhysics(deltaMS);
            Moveman(man, deltaMS); 
            if (Keyboard::isKeyPressed(Keyboard::Space) &&
                !drawingbullet) {
                drawingbullet = true;
                bullet.setCenter(man.getCenter()); 
                bullet.setVelocity(Vector2f(0, -1));
                world.AddPhysicsBody(bullet);
                bullets -= 1;
            }

            window.clear();
            if (drawingbullet) {
                window.draw(bullet); 
            }
            zombies.DoRemovals();
            for (PhysicsShape& zombie : zombies) { 
                window.draw((PhysicsSprite&)zombie);
            }
            window.draw(man);
            scoreText.setString(to_string(score));
            FloatRect textBounds = scoreText.getGlobalBounds();
            scoreText.setPosition(
                Vector2f(790 - textBounds.width, 590 - textBounds.height));
            window.draw(scoreText);
            bulletCountText.setString(to_string(bullets));
            textBounds = bulletCountText.getGlobalBounds();
            bulletCountText.setPosition(
                Vector2f(10, 590 - textBounds.height));
            window.draw(bulletCountText); 

            window.display();
            zombies.DoRemovals();
        }
        if (zombieMS > 300) { 
            zombieMS = 0;
            PhysicsSprite& zombie = zombies.Create();
            zombie.setTexture(redTex); 
            int x = 50 + ((700 / 5));
            Vector2f sz = zombie.getSize(); 
            zombie.setCenter(Vector2f(-100, 20 + (sz.y / 2))); 
            zombie.setVelocity(Vector2f(0.25, 0)); 
            world.AddPhysicsBody(zombie); 
            zombie.onCollision =
                [&drawingbullet, &world, &bullet, &zombie, &zombies, &score]
                (PhysicsBodyCollisionResult result) {
                if (result.object2 == bullet) {
                    drawingbullet = false;
                    world.RemovePhysicsBody(bullet); 
                    world.RemovePhysicsBody(zombie); 
                    zombies.QueueRemove(zombie); 
                    score += 10;
                }
                };
        }
    }
    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    FloatRect textBounds = gameOverText.getGlobalBounds();
    gameOverText.setPosition(Vector2f(
        400 - (textBounds.width / 2),
        300 - (textBounds.height / 2)
    ));
    window.draw(gameOverText);
    window.display();
    while (true) {
        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            exit(0);
        }
    }
}