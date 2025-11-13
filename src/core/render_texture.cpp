#include "render_texture.h"

#include <glad/glad.h>

#include "texture_attachment.h"

RenderTexture::RenderTexture(std::initializer_list<TextureAttachment> attachments)
: width(-1), height(-1) {
    for (auto& attachment : attachments) {
        this->attachments.insert({attachment.getName(), attachment});
    }
}

RenderTexture::~RenderTexture() {
    destroyBuffers();
}

void RenderTexture::createBuffers(const int width, const int height) {
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    int drawBufferCount = 0;
    GLenum buffers[16];
    for (auto& attachmentPair : attachments) {
        auto& attachment = attachmentPair.second;
        if (attachment.getAttachmentType() >= GL_COLOR_ATTACHMENT0 && attachment.getAttachmentType() <= GL_COLOR_ATTACHMENT15) {
            buffers[drawBufferCount++] = attachment.getAttachmentType();
        }
        attachment.createBuffer(width, height);
    }

    glDrawBuffers(drawBufferCount, buffers);
}

void RenderTexture::destroyBuffers() {
    for (auto& attachmentPair : attachments) {
        auto& attachment = attachmentPair.second;
        attachment.destroyBuffer();
    }
    if (fbo != -1) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &fbo);
        fbo = -1;
    }
}

void RenderTexture::resize(const int width, const int height) {
    if (this->fbo != -1) {
        destroyBuffers();
    }
    createBuffers(width, height);
    this->width = width;
    this->height = height;
    this->unbindWrite();
}

void RenderTexture::bindWrite(const bool viewport) const {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    if (viewport) {
        glViewport(0, 0, width, height);
    }
}

void RenderTexture::unbindWrite() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTexture::bindRead(const std::string& attachment) const {
    auto& attachmentObj = attachments.at(attachment);
    glBindTexture(GL_TEXTURE_2D, attachmentObj.getAttachmentId());
}


int RenderTexture::getWidth() const {
    return width;
}

int RenderTexture::getHeight() const {
    return height;
}

unsigned int RenderTexture::getFBO() const {
    return fbo;
}

