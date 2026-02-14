#pragma once

class Renderer; 
class GameObject
{
public:
    virtual void update(float dt) {}
    virtual void render(Renderer& renderer) = 0;

    virtual ~GameObject() = default;
};