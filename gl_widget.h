#ifndef gl_widget_H
#define gl_widget_H

#include "jds_opengl.hpp"
#include <QGLWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>

#include "gfx_base.h"

typedef struct JDS_MOUSE_DATA
{
	// button states
	char lmb, mmb, rmb;
	int old_x, old_y, x, y, delta_x, delta_y;
}JDS_MOUSE_DATA;

class gl_widget : public QGLWidget, public gfx_base
{
	Q_OBJECT
public:
	//gl_widget(QWidget *parent = 0);
	gl_widget(const QGLFormat & format, QWidget *parent = 0);
	~gl_widget();

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

#endif // gl_widget_H
