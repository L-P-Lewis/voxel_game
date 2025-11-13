#pragma once
#include <string>
#include <unordered_map>
#include "texture_attachment.h"

class RenderTexture {
    int width;
    int height;
    unsigned int fbo = -1;
    std::unordered_map<std::string, TextureAttachment> attachments;
public:
    RenderTexture(std::initializer_list<TextureAttachment> attachments);
    ~RenderTexture();
    void createBuffers(int width, int height);
    void destroyBuffers();
    void resize(int width, int height);
    void bindWrite(bool viewport) const;
    void unbindWrite() const;
    void bindRead(const std::string& attachment) const;
    int getWidth() const;
    int getHeight() const;
    unsigned int getFBO() const;

};