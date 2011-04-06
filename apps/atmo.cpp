/*
    Copyright © 2010 Lubosz Sarnecki

    This file is part of liblub.

    liblub is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    liblub is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with liblub.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <string>
#include <QApplication>
#include "System/Application.h"
#include "Scene/SceneLoader.h"
#include "Scene/SceneData.h"
#include "System/Logger.h"
#include <QPainter>
#include "Mesh/Geometry.h"
#include "Material/ProcTextures.h"
#include "Material/ShaderProgram.h"

class AtmosphereApp: public Application {
 public:
  Node * sphereNode;
  Material *groundFromAtmosphere, *groundFromSpace,
                *skyFromAtmosphere, *skyFromSpace,
                *spaceFromAtmosphere,*spaceFromSpace,
                *HDR;

  bool m_bUseHDR;
    int m_nSamples;
//    GLenum m_nPolygonMode;
    float m_Kr, m_Kr4PI;
    float m_Km, m_Km4PI;
    float m_ESun;
    float m_g;
    float m_fExposure;

    float m_fInnerRadius;
    float m_fOuterRadius;
    float m_fScale;
    float m_fWavelength[3];
    float m_fWavelength4[3];
    float m_fRayleighScaleDepth;
    float m_fMieScaleDepth;

    Camera* camera;
    Light * light;


  explicit AtmosphereApp(string sceneName) {
    QString sceneFile = QString::fromStdString(sceneName + ".xml");
    sceneLoader = new SceneLoader(sceneFile);
  }

  ~AtmosphereApp() {}

  void scene() {
    sceneLoader->load();

//    Material * foo = new Simple("Atmo/GroundFromAtmosphere");
//    if(foo==NULL) printf("1");
    groundFromAtmosphere = new Simple("Atmo/GroundFromAtmosphere");
    groundFromSpace = new Simple("Atmo/GroundFromSpace");
    skyFromAtmosphere = new Simple("Atmo/SkyFromAtmosphere");
    skyFromSpace = new Simple("Atmo/SkyFromSpace");
    spaceFromAtmosphere = new Simple("Atmo/SpaceFromAtmosphere");
    spaceFromSpace = new Simple("Atmo/SpaceFromSpace");

//    HDR = new ShaderProgram();
//    HDR->attachShader("Atmo/HDR.vert", GL_VERTEX_SHADER);
//    HDR->attachShader("Atmo/HDRRect.frag", GL_FRAGMENT_SHADER); //HDRSquare
//    HDR->init();

    Mesh * sphere = Geometry::gluSphere(10, 100, 50);

    Texture * earthMap = TextureFactory::Instance().load("earthmap1k.jpg",
        "myTexture");
//    Texture * glow = TextureFactory::Instance().load(ProcTextures::makeGlow(
//        QSize(1000, 2000), 40.0f, 0.1f), "glow");

    Material * material = new Simple("Color/debug");
    material->addTexture(earthMap);

    sphereNode = new Node("sphere", { 0, 0, 0 }, 1, sphere, material);
    sphereNode->setRotation(QVector3D(-90, 0, 180));
//    SceneGraph::Instance().addNode(sphereNode);
    camera = SceneData::Instance().getCurrentCamera();
    light = new Light(QVector3D(-2.5, 21.5, -5.2), QVector3D(1, -5, 0));
    SceneData::Instance().addLight("foolight", light);
  }
  void renderFrame(){
    RenderEngine::Instance().clear();
    sphereNode->setView(camera);
//    SceneData::Instance().getShadowLight()->bindShaderUpdate(node->getMaterial()->getShaderProgram());
    sphereNode->draw();
    GUI::Instance().draw();


    Material *space = NULL;
    if(camera->position.length() < m_fOuterRadius)
      space = spaceFromAtmosphere;
    else if(camera->position.z() > 0.0f)
      space = spaceFromSpace;


    if(space) {
//      space->activate();
      space->shaderProgram->use();
      space->shaderProgram->setUniform("v3CameraPos", camera->position);
      space->shaderProgram->setUniform("v3LightPos", light->direction);
      space->shaderProgram->setUniform("v3InvWavelength", QVector3D(1/m_fWavelength4[0], 1/m_fWavelength4[1], 1/m_fWavelength4[2]));
      space->shaderProgram->setUniform("fCameraHeight", (float)camera->position.length());
      space->shaderProgram->setUniform("fCameraHeight2", float(camera->position.length()*camera->position.length()));
      space->shaderProgram->setUniform("fInnerRadius", m_fInnerRadius);
      space->shaderProgram->setUniform("fInnerRadius2", m_fInnerRadius*m_fInnerRadius);
      space->shaderProgram->setUniform("fOuterRadius", m_fOuterRadius);
      space->shaderProgram->setUniform("fOuterRadius2", m_fOuterRadius*m_fOuterRadius);
      space->shaderProgram->setUniform("fKrESun", m_Kr*m_ESun);
      space->shaderProgram->setUniform("fKmESun", m_Km*m_ESun);
      space->shaderProgram->setUniform("fKr4PI", m_Kr4PI);
      space->shaderProgram->setUniform("fKm4PI", m_Km4PI);
      space->shaderProgram->setUniform("fScale", 1.0f / (m_fOuterRadius - m_fInnerRadius));
      space->shaderProgram->setUniform("fScaleDepth", m_fRayleighScaleDepth);
      space->shaderProgram->setUniform("fScaleOverScaleDepth", (1.0f / (m_fOuterRadius - m_fInnerRadius)) / m_fRayleighScaleDepth);
      space->shaderProgram->setUniform("g", m_g);
      space->shaderProgram->setUniform("g2", m_g*m_g);
      space->shaderProgram->setUniform("s2Test", 0);
    }
    glError;
  }
};


int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  AtmosphereApp("atmo").run();
}

