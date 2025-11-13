#include "texture_attachment.h"

#include <glad/glad.h>

#include <utility>

TextureAttachment::TextureAttachment(GLenum attachmentType, std::string name, GLint internalFormat, GLenum texelFormat, GLenum dataFormat) {
    this->internalFormat = internalFormat;
    this->texelFormat = texelFormat;
    this->dataFormat = dataFormat;
    this->attachmenType = attachmentType;
    this->name = std::move(name);
    this->attachmentId = -1;
}


void TextureAttachment::createBuffer(int width, int height) {
    glGenTextures(1, &attachmentId);
    glBindTexture(GL_TEXTURE_2D, attachmentId);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, texelFormat, dataFormat, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmenType, GL_TEXTURE_2D, attachmentId, 0);
}

void TextureAttachment::destroyBuffer() {
    if (this->attachmentId != -1) {
        glDeleteTextures(1, &attachmentId);
        this->attachmentId = -1;
    }
}

unsigned int TextureAttachment::getAttachmentId() const {
    return attachmentId;
}

std::string TextureAttachment::getName() const {
    return name;
}

GLint TextureAttachment::getInternalFormat() const {
    return internalFormat;
}

GLenum TextureAttachment::getTexelFormat() const {
    return texelFormat;
}

GLenum TextureAttachment::getDataFormat() const {
    return dataFormat;
}

GLenum TextureAttachment::getAttachmentType() const {
    return attachmenType;
}

TextureAttachment TextureAttachment::createDepthAttachment() {
    return TextureAttachment(GL_DEPTH_ATTACHMENT, "depth", GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
}

