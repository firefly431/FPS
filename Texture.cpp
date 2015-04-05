#include "Texture.h"

#include <SFML/Graphics/Image.hpp>

Texture::Texture(const std::string &fname, int texture) :
        texture(texture) {
    sf::Image im;
    glActiveTexture(GL_TEXTURE0 + texture);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    im.loadFromFile(fname);
    auto sz = im.getSize();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, sz.x, sz.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, im.getPixelsPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
