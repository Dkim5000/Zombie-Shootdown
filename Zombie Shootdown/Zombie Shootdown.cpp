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

void MoveMan(PhysicsSprite& man, int elapsedMS) {
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
    RenderWindow window(VideoMode(1600, 1200), "Zombie Shootout");
    World world(Vector2f(0, 0));
    int score(0);

    PhysicsSprite& man = *new PhysicsSprite();
    Texture manTex;
    LoadTex(manTex, "images/man.png");
    man.setTexture(manTex);
    Vector2f sz = man.getSize();
    man.setCenter(Vector2f(400,
        600 - (sz.y / 2)));

    PhysicsSprite bullet;
    Texture bulletTex;
    LoadTex(bulletTex, "images/bullet.png");
    bullet.setTexture(bulletTex);
    bool drawingArrow(false);

    PhysicsRectangle top;
    top.setSize(Vector2f(800, 10));
    top.setCenter(Vector2f(400, 5));
    top.setStatic(true);
    world.AddPhysicsBody(top);

    Clock clock;
    Time lastTime(clock.getElapsedTime());
    Time currentTime(lastTime);

    long duckMS(0);
    while (drawingArrow) {
        currentTime = clock.getElapsedTime();
        Time deltaTime = currentTime - lastTime;
        long deltaMS = deltaTime.asMilliseconds();
        duckMS = duckMS + deltaMS;
    }
    while (true) {
        currentTime = clock.getElapsedTime();
        Time deltaTime = currentTime - lastTime;
        long deltaMS = deltaTime.asMilliseconds();
        duckMS = duckMS + deltaMS;
        if (deltaMS > 9) {
            lastTime = currentTime;
            world.UpdatePhysics(deltaMS);
            MoveMan(man, deltaMS);
        }
        window.clear();
        window.draw(man); 
        window.display(); 
    }
} 
