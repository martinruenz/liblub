/*
 * Copyright © 2010 Lubosz Sarnecki
 * RenderPasses.cpp
 *
 *  Created on: Oct 5, 2010
 */

#include "Window/MediaLayer.h"
#include "Scene/Camera.h"
#include "Material/Textures.h"
#include "Scene/SceneData.h"
#include "Scene/SceneGraph.h"
#include "Material/Materials.h"

ShadowPass::ShadowPass(FrameBuffer * fbo) {
    this->fbo = fbo;
    targetTexture = new ShadowTexture(fbo->res, "shadowMap");
    fbo->attachTexture(GL_DEPTH_ATTACHMENT, targetTexture);
    fbo->disableColorBuffer();

    material = new Minimal();
}

void ShadowPass::prepare() {
    fbo->bind();

    glPolygonOffset(2.0, 0.0);
    RenderEngine::Instance().clear();

    // In the case we render the shadowmap to a higher resolution,
    // the viewport must be modified accordingly.
    fbo->updateRenderView();
    material->activate();
}

void ShadowPass::draw() {
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  //TODO: Multiple lights
  SceneGraph::Instance().drawCasters(material,SceneData::Instance().getShadowLight());
}

void ShadowPass::cleanUp() {
    fbo->unBind();
    glPolygonOffset(0.0, 0.0);
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
        glViewport(0, 0, SceneData::Instance().width,
            SceneData::Instance().height);
}
WritePass::WritePass(FrameBuffer * fbo, Texture * texture, Material * material) {
 WritePass(fbo, texture, material, true);
}


WritePass::WritePass(FrameBuffer * fbo, Texture * texture, Material * material, bool useColorBuffer) {
    this->fbo = fbo;
    this->material = material;
    targetTexture = texture;
    if (!useColorBuffer) {
      fbo->attachTexture(GL_DEPTH_ATTACHMENT, targetTexture);
      fbo->disableColorBuffer();
    } else {
      fbo->attachTexture(GL_COLOR_ATTACHMENT0, targetTexture);
    }
}

void WritePass::prepare() {
    fbo->bind();
    RenderEngine::Instance().clear();
    fbo->updateRenderView();
    material->activate();
}

void WritePass::draw() {
    SceneGraph::Instance().drawCasters(material);
}

void WritePass::cleanUp() {
    fbo->unBind();
    glViewport(0, 0, SceneData::Instance().width,
        SceneData::Instance().height);
}

FilterPass::FilterPass(FrameBuffer * fbo) {
    this->fbo = fbo;
    QSize res = SceneData::Instance().getResolution();
    targetTexture = new DepthTexture(res, "shadowMap");
    fbo->attachTexture(GL_DEPTH_ATTACHMENT, targetTexture);
    fbo->disableColorBuffer();

    material = new Minimal();
}

void FilterPass::prepare() {
    fbo->bind();

    RenderEngine::Instance().clear();

    // In the case we render the shadowmap to a higher resolution,
    // the viewport must be modified accordingly.
    fbo->updateRenderView();
    SceneGraph::Instance().drawNodes(SceneData::Instance().getCurrentCamera());
    fbo->unBind();

    glPolygonOffset(20.0, 0.0);
    material->activate();

}

void FilterPass::draw(Material * material) {
    fbo->draw(material);
}

void FilterPass::cleanUp() {
    fbo->unBind();
    glPolygonOffset(0.0, 0.0);
}

LightTogglePass::LightTogglePass() {
}

void LightTogglePass::prepare() {
    RenderEngine::Instance().clear();
}

void LightTogglePass::draw() {

    if (RenderEngine::Instance().lightView) {
        SceneGraph::Instance().drawNodes(SceneData::Instance().getShadowLight());
    } else {
        SceneGraph::Instance().drawNodes(SceneData::Instance().getCurrentCamera());
    }
}

FBODebugPass::FBODebugPass(FrameBuffer * fbo) {
  this->fbo = fbo;
}

void FBODebugPass::prepare() {
    RenderEngine::Instance().clear();
}

void FBODebugPass::draw() {
  glViewport(0, 0, SceneData::Instance().width,
      SceneData::Instance().height);
  Material* material = SceneData::Instance().getMaterial("debugFBO");
//  foreach (Texture * texture, material->textures){
//    printf("Texture! %s\n", texture->name.c_str());
//  }
  fbo->draw(material);
}
