#ifndef main_window_H
#define main_window_H

#include <QMainWindow>
//#include "jds_opengl.hpp"
#include "gl_widget.h"
#include <QTimer>
#define GLM_FORCE_ONLY_XYZW
#include <glm/glm.hpp>
using glm::vec3;

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QPushButton;
class QWidget;
class QLabel;
class QLineEdit;
class QComboBox;

class main_window : public QMainWindow
{
	Q_OBJECT

public:
	main_window(QWidget *parent = 0);
	~main_window();

private:
	gl_widget *glWidget;
	QTimer *gfx_timer;
	QWidget *mainWidget;
	QHBoxLayout *mainLayout;
	QVBoxLayout *rightLayout, *leftLayout;
	QLabel *fpsLbl,*fpsVal;

	QLabel *camLbl, *camxLbl, *camyLbl, *camzLbl;

	QLabel *piLbl, *pposLbl, *pindLbl, *pvelLbl;
	QLabel *pposX, *pposY, *pposZ, *pindX, *pindY, *pindZ, *pvelX, *pvelY, *pvelZ;

	void setupUi();

public slots:
	void fixFocus();
	void updateFps(int fps);
	void updateCamera(glm::vec3 cam);
	void updatePickerInfo(glm::uvec3 i, glm::vec3 p, glm::vec3 v);
};

#endif // main_window_H
