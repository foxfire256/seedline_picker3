#include "gl_widget.h"

#ifdef _MSC_VER // M$ HACKS
#pragma warning(disable : 4996) // crt is secure, disable the warning
#define snprintf _snprintf_s
#endif

//------------------------------------------------------------------------------
gl_widget::gl_widget(const QGLFormat & format, QWidget *parent) :
	QGLWidget(format, parent)
{
	mouse.lmb = mouse.mmb = mouse.rmb = 0;

	framerate_timer = new QTimer(this);

	connect(framerate_timer, SIGNAL(timeout()), this, SLOT(updateFramerate()));

	framerate_timer->start(1000);
}

//------------------------------------------------------------------------------
gl_widget::~gl_widget()
{

}

//------------------------------------------------------------------------------
// this is automatically called once when the program starts
void gl_widget::initializeGL()
{
	gfx_init_gl();

	emit updatedCameraInfo(eye);
	emit updatePickerInfo(picker_ind, ppos, pvel);
}

//------------------------------------------------------------------------------
// this redraws the contents of the opengl widget
void gl_widget::paintGL()
{
	update_camera();
	gfx_render();
}

//------------------------------------------------------------------------------
// this is called when the widget is resized
void gl_widget::resizeGL(int width, int height)
{
	gfx_resize(width, height);
}

//------------------------------------------------------------------------------
void gl_widget::updateFramerate()
{
	framerate = frames;
	frames = 0;
	emit fpsUpdated(framerate);
	emit updatedCameraInfo(eye);
	emit updatePickerInfo(picker_ind, ppos, pvel);
	printf("fps: %d\n", framerate);
	fflush(stdout);
}

//------------------------------------------------------------------------------

void gl_widget::mousePressEvent(QMouseEvent * event)
{
	switch(event->button())
	{
	case Qt::LeftButton:
		mouse.lmb = 1;
		break;
	case Qt::MidButton:
		break;
	case Qt::RightButton:
		mouse.rmb = 1;
		break;
	default:
		break;
	}

	mouse.x = event->x();
	mouse.y = event->y();

	mouse.old_x = mouse.x;
	mouse.old_y = mouse.y;

	// pass to the default event handler
	QGLWidget::mousePressEvent(event);
}

//------------------------------------------------------------------------------

void gl_widget::mouseReleaseEvent(QMouseEvent * event)
{
	switch(event->button())
	{
	case Qt::LeftButton:
		mouse.lmb = 0;
		break;
	case Qt::MidButton:
		break;
	case Qt::RightButton:
		mouse.rmb = 0;
		break;
	default:
		break;
	}

	mouse.x = event->x();
	mouse.y = event->y();

	mouse.old_x = mouse.x;
	mouse.old_y = mouse.y;

	// pass to the default event handler
	QGLWidget::mouseReleaseEvent(event);
}

//------------------------------------------------------------------------------

void gl_widget::mouseMoveEvent(QMouseEvent * event)
{
	mouse.x = event->x();
	mouse.y = event->y();

	mouse.delta_x = mouse.x - mouse.old_x;
	mouse.delta_y = mouse.y - mouse.old_y;

	mouse.old_x = mouse.x;
	mouse.old_y = mouse.y;

	// rotate
	if(mouse.lmb)
	{

	}
	if(mouse.mmb)
	{

	}
	if(mouse.rmb)
	{

	}

	// pass to the default event handler
	QGLWidget::mouseMoveEvent(event);
}

//------------------------------------------------------------------------------

void gl_widget::mouseDoubleClickEvent(QMouseEvent * event)
{
	// pass to the default event handler
	QGLWidget::mouseDoubleClickEvent(event);
}

//------------------------------------------------------------------------------

void gl_widget::keyPressEvent(QKeyEvent *event)
{
	switch(event->key())
	{
	case Qt::Key_A:
		forward = 1;
		break;
	case Qt::Key_Z:
		back = 1;
		break;
	case Qt::Key_Q:
		roll_left = 1;
		break;
	case Qt::Key_W:
		roll_right = 1;
		break;
	case Qt::Key_Left:
		yaw_ccw = 1;
		break;
	case Qt::Key_Right:
		yaw_cw = 1;
		break;
	case Qt::Key_Up:
		pitch_down = 1;
		break;
	case Qt::Key_Down:
		pitch_up = 1;
		break;
	case Qt::Key_Space:
		sl->add(ppos, picker_ind);
		slgfx->update();
		break;
	case Qt::Key_Enter:
		sl->save();
		sl->clear();
		slgfx->update();
		break;
	case Qt::Key_Return:
		sl->save();
		sl->clear();
		slgfx->update();
		break;
	case Qt::Key_X:
		strafe_left = 1;
		break;
	case Qt::Key_C:
		strafe_right = 1;
		break;
	case Qt::Key_S:
		strafe_down = 1;
		break;
	case Qt::Key_D:
		strafe_up = 1;
		break;
	default:
		// pass to the default event handler if we don't use it
		QGLWidget::keyPressEvent(event);
		break;
	}
}

//------------------------------------------------------------------------------

void gl_widget::keyReleaseEvent(QKeyEvent *event)
{
	switch(event->key())
	{
	case Qt::Key_A:
		forward = 0;
		break;
	case Qt::Key_Z:
		back = 0;
		break;
	case Qt::Key_Q:
		roll_left = 0;
		break;
	case Qt::Key_W:
		roll_right = 0;
		break;
	case Qt::Key_Left:
		yaw_ccw = 0;
		break;
	case Qt::Key_Right:
		yaw_cw = 0;
		break;
	case Qt::Key_Up:
		pitch_down = 0;
		break;
	case Qt::Key_Down:
		pitch_up = 0;
		break;
	case Qt::Key_Escape:
		exit(0);
		break;
	case Qt::Key_X:
		strafe_left = 0;
		break;
	case Qt::Key_C:
		strafe_right = 0;
		break;
	case Qt::Key_S:
		strafe_down = 0;
		break;
	case Qt::Key_D:
		strafe_up = 0;
		break;
	default:
		// pass to the default event handler if we don't use it
		QGLWidget::keyPressEvent(event);
		break;
	}
}

