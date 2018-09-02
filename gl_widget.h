#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "jds_opengl.hpp"
#include <QGLWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>

#include "GfxBase.h"

typedef struct JDS_MOUSE_DATA
{
	// button states
	char lmb, mmb, rmb;
	int old_x, old_y, x, y, delta_x, delta_y;
}JDS_MOUSE_DATA;

class GLWidget : public QGLWidget, public GfxBase
{
	Q_OBJECT
public:
	//GLWidget(QWidget *parent = 0);
	GLWidget(const QGLFormat & format, QWidget *parent = 0);
	~GLWidget();

private slots:
	void updateFramerate();

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	// override the default key event handlers
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	// override mouse event handlers
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	void mouseDoubleClickEvent(QMouseEvent * event);

signals:
	void fpsUpdated(int fps);
	void updatedCameraInfo(glm::vec3);
	// index, position, velocity
	void updatePickerInfo(glm::uvec3, glm::vec3, glm::vec3);

private:
	//double delta_time, update_time, render_time;
	QTimer *framerate_timer;

	// gui
	struct JDS_MOUSE_DATA mouse;
};

#endif // GLWIDGET_H
