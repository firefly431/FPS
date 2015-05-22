#include "Texture.h"

#include "stb_image.h"

Texture::Texture(const std::string &fname, int texture) :
        texture(texture) {
    //  we don't care about some things so we use _
    int width, height, _;
    unsigned char *data = stbi_load(fname.c_str(), &width, &height, &_, 4);
    glActiveTexture(GL_TEXTURE0 + texture);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    // set image format
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // put data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    // set tex parameters I like
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // necessary for wall/floor
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // don't need pixels anymore
    stbi_image_free(data);
}

Texture::Texture(Texture &&move) : id(move.id), texture(move.texture) {
    move.id = 0;
    move.texture = 0;
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

void Texture::activate() {
    glActiveTexture(GL_TEXTURE0 + texture);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::deactivate(const Texture &tex) {
    glActiveTexture(GL_TEXTURE0 + tex.texture);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::deactivate(const int texture) {
    glActiveTexture(GL_TEXTURE0 + texture);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// only for active texture
void Texture::deactivate() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::getUnit() {
    return texture;
}
