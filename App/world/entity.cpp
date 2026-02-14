#include <cmath>
#include "entity.h"

Entity::Entity(const TexturedModel& model_, const glm::vec3& position_, const glm::vec3& rotation_, float scale_)
    : model_(model_), position_(position_), rotation_(rotation_), scale_(scale_) {
}

float Entity::getTextureXOffset() const {
    auto rowCount = model_.getTexture().getNumOfRows();
    auto column = textureIndex_ % rowCount;
    return static_cast<float>(column) / static_cast<float>(rowCount);
}

float Entity::getTextureYOffset() const {
    auto rowCount = model_.getTexture().getNumOfRows();
    auto row = textureIndex_ / rowCount;
    return static_cast<float>(row) / static_cast<float>(rowCount);
}

void Entity::scale(float _scale) {
    Entity::_scale = _scale;
}

void Entity::translate(float dx, float dy, float dz) {
    position_.x += dx;
    position_.y += dy;
    position_.z += dz;
}

//rotates the entity using Euler angles
void Entity::rotate(float dx, float dy, float dz) {
    rotation_.x += dx;
    rotation_.y += dy;
    rotation_.z += dz;
}

// -- getters and setters ----------------------------------
const TexturedModel& Entity::getModel() const {
    return model_;
}

void Entity::setModel(const TexturedModel& model) {
    model_ = model;
}

const glm::vec3& Entity::getPosition() const {
    return position_;
}

void Entity::setPosition(const glm::vec3& position) {
    position_ = position;
}

const glm::vec3& Entity::getRotation() const {
    return rotation_;
}

void Entity::setRotation(const glm::vec3& rotation) {
    rotation_ = rotation;
}

float Entity::getScale() const {
    return scale_;
}