#pragma once

class Object {
public:
    virtual void Initialize() {}

    virtual void Update(float deltaTime) {}

    virtual void Render(const Camera &camera) const {}
};