#pragma once
#include <string>
#include <glad/glad.h>


class TextureAttachment {
    unsigned int attachmentId;
    std::string name;
    GLint internalFormat;
    GLenum texelFormat;
    GLenum dataFormat;
    GLenum attachmenType;
public:
    explicit TextureAttachment(GLenum attachmentType, std::string name, GLint internalFormat = GL_RGBA, GLenum texelFormat = GL_RGBA, GLenum dataFormat = GL_UNSIGNED_BYTE);
    void createBuffer(int width, int height);
    void destroyBuffer();
    unsigned int getAttachmentId() const;
    std::string getName() const;
    GLint getInternalFormat() const;
    GLenum getTexelFormat() const;
    GLenum getDataFormat() const;
    GLenum getAttachmentType() const;

    static TextureAttachment createDepthAttachment();
};
