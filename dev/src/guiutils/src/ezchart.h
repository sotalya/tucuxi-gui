/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef EZCHART_H
#define EZCHART_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>

//#include <glm/glm.hpp>

//OpenGl code is adapted from here: https://en.wikibooks.org/wiki/OpenGL_Programming/Scientific_OpenGL_Tutorial_01
//qt code is adapted from the SceneGraph example here: http://doc.qt.io/qt-5/qtquick-scenegraph-openglunderqml-example.html

class EzChartRenderer : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    EzChartRenderer() : m_t(0), m_program(0) { }
    ~EzChartRenderer();

    void setT(qreal t) { m_t = t; }

    qreal scale() const { return m_scale; }
    void setScale(qreal scale) {m_scale = scale;}

    qreal offsetx() const { return m_offsetx; }
    void setOffsetx(qreal offsetx) {m_offsetx = offsetx;}

    qreal offsety() const { return m_offsety; }
    void setOffsety(qreal offsety) {m_offsety = offsety;}

    void setViewportSize(const QSize &size) { m_viewportSize = size; }
    glm::mat4 viewport_transform(float x, float y, float width, float height);

public slots:
    void paint();

private:
    QSize m_viewportSize;
    qreal m_t;
    qreal m_scale;
    qreal m_offsetx;
    qreal m_offsety;
    QOpenGLShaderProgram *m_program;
};
//! [1]

//! [2]
class EzChart : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    Q_PROPERTY(qreal scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(qreal offsetx READ offsetx WRITE setOffsetx NOTIFY offsetxChanged)
    Q_PROPERTY(qreal offsety READ offsety WRITE setOffsety NOTIFY offsetyChanged)

public:
    EzChart();

    qreal t() const { return m_t; }
    void setT(qreal t);

    qreal scale() const { return m_scale; }
    void setScale(qreal scale);

    qreal offsetx() const { return m_offsetx; }
    void setOffsetx(qreal offsetx);

    qreal offsety() const { return m_offsety; }
    void setOffsety(qreal offsety);

signals:
    void tChanged();
    void scaleChanged();
    void offsetxChanged();
    void offsetyChanged();

public slots:
    void sync();
    Q_INVOKABLE void async();
    Q_INVOKABLE void resync();
    Q_INVOKABLE void cleanup();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    qreal m_t;
    qreal m_scale;
    qreal m_offsetx;
    qreal m_offsety;
    EzChartRenderer *m_renderer;
};

#endif // EZCHART_H
