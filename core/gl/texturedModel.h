#ifndef RENDERENGINE_TEXTURED_MODEL_H
#define RENDERENGINE_TEXTURED_MODEL_H

#include "rawModel.h"

class TexturedModel {
public:
    //simple constructor, store the raw model and texture
    explicit TexturedModel(const RawModel& model) : rawModel_(model), texture_(tex) {}

    //getters
    const RawModel& getModel() const {
        return rawModel_;
    }

private:
    RawModel rawModel_;
};


#endif //RENDERENGINE_TEXTURED_MODEL_H