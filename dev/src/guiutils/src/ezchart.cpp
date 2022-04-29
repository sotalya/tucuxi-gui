//@@license@@

#include "ezchart.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

EzChart::EzChart()
    : m_t(0),
      m_scale(1.0),
      m_offsetx(0.0),
      m_offsety(0.0),
      m_renderer(0)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
}
//! [7]

//! [8]
void EzChart::setT(qreal t)
{
    if (t == m_t)
        return;
    m_t = t;
    emit tChanged();
    if (window())
        window()->update();
}

void EzChart::setScale(qreal scale)
{
    if (scale == m_scale)
        return;
    m_scale = scale;
    emit scaleChanged();
    if (window())
        window()->update();
}

void EzChart::setOffsetx(qreal offset)
{
    if (offset == m_offsetx)
        return;
    m_offsetx = offset;
    emit offsetxChanged();
    if (window())
        window()->update();
}

void EzChart::setOffsety(qreal offset)
{
    if (offset == m_offsety)
        return;
    m_offsety = offset;
    emit offsetyChanged();
    if (window())
        window()->update();
}

//! [8]

//! [1]
void EzChart::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
        connect(win, SIGNAL(sceneGraphInvalidated()), this, SLOT(cleanup()), Qt::DirectConnection);
//! [1]
        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
//! [3]
        win->setClearBeforeRendering(false);
    }
}
//! [3]

//! [6]
void EzChart::cleanup()
{
    if (m_renderer) {
        delete m_renderer;
        m_renderer = 0;
    }
}

EzChartRenderer::~EzChartRenderer()
{
    delete m_program;
}
//! [6]

//! [9]
void EzChart::sync()
{
    if (!m_renderer) {
        m_renderer = new EzChartRenderer();
        connect(window(), SIGNAL(afterRendering()), m_renderer, SLOT(paint()), Qt::DirectConnection);
    }

    QSize _sz(window()->size().width() / 2 * window()->devicePixelRatio(), window()->size().height() * 3/4 * window()->devicePixelRatio());
    m_renderer->setViewportSize(_sz);
    m_renderer->setT(t());
    m_renderer->setScale(scale());
    m_renderer->setOffsetx(offsetx());
    m_renderer->setOffsety(offsety());
}

void EzChart::resync()
{
    connect(window(), SIGNAL(afterRendering()), m_renderer, SLOT(paint()), Qt::DirectConnection);
}

void EzChart::async()
{
    disconnect(window(), SIGNAL(afterRendering()), m_renderer, SLOT(paint()));
}

//! [9]

//! [4]
void EzChartRenderer::paint()
{
    if (!m_program) {
        initializeOpenGLFunctions();

        m_program = new QOpenGLShaderProgram();
        m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                           "attribute vec2 coord2d;"
                                           "uniform mat4 transform;"

                                           "void main(void) {"
                                             "gl_Position = transform * vec4(coord2d.xy, 0, 1);"
                                           "}"
                                           );
        m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                           "uniform vec4 color;"

                                           "void main(void) {"
                                               "gl_FragColor = color;"
                                           "}"
                                           );

        m_program->bindAttributeLocation("coord2d", 0);
        m_program->link();

    }

    m_program->bind();

    struct point {
      GLfloat x;
      GLfloat y;
    };

    point graph[2000];

    for(int i = 0; i < 2000; i++) {
      float x = (i - 1000.0) / 100.0;
      graph[i].x = x;
      graph[i].y = sin(x * 10.0) / (1.0 + x * x);
    }

    GLuint vbo;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof graph, graph, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    GLint attribute_coord2d = m_program->attributeLocation("coord2d");
    m_program->enableAttributeArray(attribute_coord2d);

    GLfloat red[4] = {0, 0, 1, 1};
    GLint uniform_color = m_program->uniformLocation("color");

    glUniform4fv(uniform_color, 1, red);
    GLint uniform_transform = m_program->uniformLocation("transform");

    glm::mat4 transform = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(scale(), scale(), 1)), glm::vec3(offsetx(), offsety(), 0));
    glUniformMatrix4fv(uniform_transform, 1, GL_FALSE, glm::value_ptr(transform));

    const int margin = 20;
    const int ticksize = 10;

    glViewport(
        m_viewportSize.width() + margin + ticksize,
        margin + ticksize,
        m_viewportSize.width() - margin * 2 - ticksize,
        m_viewportSize.height() - margin * 2 - ticksize
    );

    glScissor(
        m_viewportSize.width() + margin + ticksize,
        margin + ticksize,
        m_viewportSize.width() - margin * 2 - ticksize,
        m_viewportSize.height() - margin * 2 - ticksize
    );

    glEnable(GL_SCISSOR_TEST);

    glVertexAttribPointer(
      attribute_coord2d,   // attribute
      2,                   // number of elements per vertex, here (x,y)
      GL_FLOAT,            // the type of each element
      GL_FALSE,            // take our values as-is
      0,                   // no space between values
      0                    // use the vertex buffer object
    );

    glDrawArrays(GL_LINE_STRIP, 0, 2000);

    //box


    glViewport(m_viewportSize.width(), 0, m_viewportSize.width(), m_viewportSize.height());
    glDisable(GL_SCISSOR_TEST);

    transform = viewport_transform(
      margin + ticksize,
      margin + ticksize,
      m_viewportSize.width() - margin * 2 - ticksize,
      m_viewportSize.height() - margin * 2 - ticksize
    );

    glUniformMatrix4fv(uniform_transform, 1, GL_FALSE, glm::value_ptr(transform));

    GLuint box_vbo;
    glGenBuffers(1, &box_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, box_vbo);

    static const point box[4] = {{-1, -1}, {1, -1}, {1, 1}, {-1, 1}};
    glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_STATIC_DRAW);

    GLfloat black[4] = {0, 0, 0, 1};
    glUniform4fv(uniform_color, 1, black);

    glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINE_LOOP, 0, 4);

    //tickmarks

    float border = margin + ticksize;

    float pixel_x = 2.0 / (m_viewportSize.width() - border * 2 - ticksize);
    float pixel_y = 2.0 / (m_viewportSize.height() - border * 2 - ticksize);

    GLuint ticks_vbo;
    glGenBuffers(1, &ticks_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, ticks_vbo);

    point ticks[42];

    //y tickmarks

    float tickspacing = 0.1 * powf(10, -floor(log10(scale())));
    float down = -1.0 / scale() - offsety();
    float up = 1.0 / scale() - offsety();
    int down_i = ceil(down / tickspacing);
    int up_i = floor(up / tickspacing);
    float remy = down_i * tickspacing - down;
    float firstticky = -1.0 + remy * scale();

    int nticksy = up_i - down_i + 1;
    if(nticksy > 21)
      nticksy = 21;

    for(int i = 0; i < nticksy; i++) {
        float y = firstticky + i * tickspacing * scale();
        float tickscale = ((i + down_i) % 10) ? 0.5 : 1;
        ticks[i * 2].x = -1;
        ticks[i * 2].y = y;
        ticks[i * 2 + 1].x = -1 - ticksize * tickscale * pixel_x;
        ticks[i * 2 + 1].y = y;
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof ticks, ticks, GL_STREAM_DRAW);

    glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINES, 0, nticksy * 2);

    //x tickmarks

//    float tickspacing = 0.1 * powf(10, -floor(log10(scale())));
    float left = -1.0 / scale() - offsetx();
    float right = 1.0 / scale() - offsetx();
    int left_i = ceil(left / tickspacing);
    int right_i = floor(right / tickspacing);
    float rem = left_i * tickspacing - left;
    float firsttickx = -1.0 + rem * scale();

    int nticksx = right_i - left_i + 1;
    if(nticksx > 21)
      nticksx = 21;

    for(int i = 0; i < nticksx; i++) {
      float x = firsttickx + i * tickspacing * scale();
      float tickscale = ((i + left_i) % 10) ? 0.5 : 1;
      ticks[i * 2].x = x;
      ticks[i * 2].y = -1;
      ticks[i * 2 + 1].x = x;
      ticks[i * 2 + 1].y = -1 - ticksize * tickscale * pixel_y;
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof ticks, ticks, GL_STREAM_DRAW);

    glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINES, 0, nticksx * 2);

    m_program->disableAttributeArray(attribute_coord2d);
    m_program->release();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

glm::mat4 EzChartRenderer::viewport_transform(float x, float y, float width, float height) {
  // Calculate how to translate the x and y coordinates:
  float offset_x = (2.0 * x + (width - m_viewportSize.width())) / m_viewportSize.width();
  float offset_y = (2.0 * y + (height - m_viewportSize.height())) / m_viewportSize.height();

  // Calculate how to rescale the x and y coordinates:
  float scale_x = width / m_viewportSize.width();
  float scale_y = height / m_viewportSize.height();

  return glm::scale(glm::translate(glm::mat4(1), glm::vec3(offset_x, offset_y, 0)), glm::vec3(scale_x, scale_y, 1));
}
