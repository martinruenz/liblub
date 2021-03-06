#include <QtGui>
#include <QUiLoader>
#include "glWidget.h"
#include "Window.h"
#include "Scene/SceneData.h"
#include "System/Config.h"
#include "PlanetWidget.h"
#include "FloatEditorWidget.h"

QtWindow::QtWindow() {
  glWidget = new GLWidget;
  Config::Instance().load("config.xml");
//  QUiLoader uiLoader;
//  QFile uiFile("scripts/valueChanger.ui");
//  QWidget *valueChanger = uiLoader.load(&uiFile);
//  mainLayout->addWidget(valueChanger);

  QHBoxLayout *mainLayout = new QHBoxLayout;
  mainLayout->addWidget(glWidget);

  QVBoxLayout *sideLayout = new QVBoxLayout;
  mainLayout->addLayout(sideLayout);

  QListWidget * planetList = new QListWidget();

  foreach(Planet * planet , glWidget->planets){
    QListWidgetItem * planetItem = new QListWidgetItem(planetList);
    planetItem->setText(planet->name);
  }
  QCheckBox *checkBox = new QCheckBox();
  checkBox->setText("Postprocessing");
  checkBox->setChecked(true);
  connect(checkBox, SIGNAL(clicked(bool)), glWidget, SLOT(setPostprocessing(bool)));
  connect(checkBox, SIGNAL(clicked(bool)), glWidget, SLOT(updateGL()));
  sideLayout->addWidget(checkBox);

  FloatEditorWidget* exposure = new FloatEditorWidget("Exposure",SLOT(setExposure(double)), 2.0, 0, 10, glWidget);
  connect(exposure, SIGNAL(updateGL()), glWidget, SLOT(updateGL()));
  sideLayout->addWidget(exposure);

  QCheckBox *checkBox2 = new QCheckBox();
  checkBox2->setText("Wireframe");
  checkBox2->setChecked(false);
  connect(checkBox2, SIGNAL(clicked(bool)), glWidget, SLOT(setWireframe(bool)));
  connect(checkBox2, SIGNAL(clicked(bool)), glWidget, SLOT(updateGL()));
  sideLayout->addWidget(checkBox2);

  QCheckBox *checkBox3 = new QCheckBox();
  checkBox3->setText("Lazy Rendering");
  checkBox3->setChecked(true);
  connect(checkBox3, SIGNAL(clicked(bool)), glWidget, SLOT(setLazy(bool)));
  connect(checkBox3, SIGNAL(clicked(bool)), glWidget, SLOT(updateGL()));
  sideLayout->addWidget(checkBox3);

//  sideLayout->addWidget(planetList);
  sideLayout->addWidget(focusPlanet());


  setLayout(mainLayout);

  setWindowTitle(tr("Planets Demo"));
  glWidget->setFocus();
  setMaximumSize(QSize(1920, 1200));
}

PlanetWidget * QtWindow::focusPlanet(){
  PlanetWidget * planetWidget = new PlanetWidget(glWidget->focusedPlanet);
  connect(planetWidget, SIGNAL(updateGL(void)), glWidget, SLOT(updateGL(void)));
  return planetWidget;
}

void QtWindow::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_Escape)
    close();
  else
    pressedKeys.push_back(e->key());

  executeKeys();
}

void QtWindow::executeKeys() {
  float inputSpeed = .1;
  foreach(int key, pressedKeys) {
      if (key == Qt::Key_W)
        SceneData::Instance().getCurrentCamera()->forwardDirection(inputSpeed);
      if (key == Qt::Key_A)
        SceneData::Instance().getCurrentCamera()->leftDirection(inputSpeed);
      if (key == Qt::Key_S)
        SceneData::Instance().getCurrentCamera()->backwardDirection(inputSpeed);
      if (key == Qt::Key_D)
        SceneData::Instance().getCurrentCamera()->rightDirection(inputSpeed);
  }
  glWidget->updateGL();
}

void QtWindow::keyReleaseEvent(QKeyEvent *e) {
  pressedKeys.removeAll(e->key());
}

