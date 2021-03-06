/*
 * Copyright © 2010 Lubosz Sarnecki
 * DirectionNode.h
 *
 *  Created on: Oct 5, 2010
 */

#pragma once

#include "common/Qt3D.h"
#include <QtCore>
const QVector3D up(0, 1, 0);

class DirectionNode {
public:
  DirectionNode();
  virtual ~DirectionNode();

  // View Params
  qreal aspect;
  qreal fov;
  qreal nearClip;
  qreal farClip;
  qreal yaw;
  qreal pitch;
  qreal roll;
  QVector3D position, direction, defaultDirection;

  QMatrix4x4 viewMatrix, projectionMatrix, rotation;

  QVector3D getDirection() const;

  QVector3D getPosition() const;

  QMatrix4x4 getView() const;
  QMatrix4x4 getViewNoTranslation() const;
  QMatrix4x4 getProjection() const;

  void setPosition(const QVector3D& position);
  void setDirection(const QVector3D & direction);
  void setAspect(qreal aspect);
  void setParams(qreal fov, qreal near, qreal far);

  void forwardDirection(qreal distance);
  void backwardDirection(qreal distance);
  void leftDirection(qreal distance);
  void rightDirection(qreal distance);

  void leftWorld(qreal distance);
  void rightWorld(qreal distance);
  void upWorld(qreal distance);
  void downWorld(qreal distance);
  void forwardWorld(qreal distance);
  void backWorld(qreal distance);

  void updatePerspective();
  void updateView();

  virtual void update() = 0;
};
