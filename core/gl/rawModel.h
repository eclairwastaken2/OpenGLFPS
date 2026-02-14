#ifndef RENDERENGINE_RAW_MODEL_H
#define RENDERENGINE_RAW_MODEL_H


class RawModel {
private:
    unsigned int vaoID_;
    unsigned int vertexCount_;

public:
    RawModel() = default;
    //set the vao id and vertex count
    explicit RawModel(unsigned int id, unsigned int count) {
        vaoID_ = id;
        vertexCount_ = count;
    }

    //getters
    unsigned int get_vaoID() const { return vaoID_; }
    int get_vertexCount() const { return vertexCount_; }

    void set_vaoID(unsigned int vaoID_) {
        RawModel::vaoID_ = vaoID_;
    }

    void set_vertexCount(unsigned int vertexCount_) {
        RawModel::vertexCount_ = vertexCount_;
    }

};


#endif //RENDERENGINE_RAW_MODEL_H