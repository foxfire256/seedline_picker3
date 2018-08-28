#include "MainWindow.h"
#include <QtGui>
#include <ctime>
#include <QGLFormat>
#include <cstdio>
#include <string>

#ifdef _MSC_VER // M$ HACKS
#pragma warning(disable : 4996) // crt is secure, disable the warning
#define snprintf _snprintf_s
#endif

//------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	srand(time(NULL));

	setupUi();

	gfx_timer = new QTimer(this);

	connect(gfx_timer, SIGNAL(timeout()), glWidget, SLOT(updateGL()));
	connect(glWidget, SIGNAL(fpsUpdated(int)), this, SLOT(updateFps(int)));
	connect(gfx_timer, SIGNAL(timeout()), this, SLOT(fixFocus()));
	connect(glWidget, SIGNAL(updatedCameraInfo(glm::vec3)), this,
		SLOT(updateCamera(glm::vec3)));
	connect(glWidget, SIGNAL(updatePickerInfo(glm::uvec3,glm::vec3,glm::vec3)),
		this, SLOT(updatePickerInfo(glm::uvec3,glm::vec3,glm::vec3)));

	// a value of 0 here makes this run all the time when the message queue
	// is clear
	gfx_timer->start(0); // time in ms
}

//------------------------------------------------------------------------------

MainWindow::~MainWindow()
{

}


//------------------------------------------------------------------------------

void MainWindow::setupUi()
{
	int gl_size = 400;

	QSizePolicy rightSizePolicy = QSizePolicy(QSizePolicy::Minimum,
		QSizePolicy::Minimum);
	int larger_width = 80;
	int smaller_width = 80;

	QGLFormat fmt, fmt_actual;

	printf("Qt compile time version: %s\n", QT_VERSION_STR);
	printf("Qt run time version: %s\n", qVersion());

	fmt.setSwapInterval(0); // disable vsync
	//fmt.setAccum(true);
#if (QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
	int gl_major = 4, gl_minor = 3;
	fmt.setProfile(QGLFormat::CoreProfile);
	fmt.setVersion(gl_major, gl_minor);
#endif
	//QGLFormat::setDefaultFormat(fmt);

	mainWidget = new QWidget(this);

	glWidget = new GLWidget(fmt, this);
	glWidget->setMinimumSize(gl_size, gl_size);
	//glWidget->setMaximumSize(gl_size, gl_size);
	//glWidget->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,
	//	QSizePolicy::Fixed));
	glWidget->setMouseTracking(true);

#if (QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
	// see what OpenGL version we were able to get
	fmt_actual = glWidget->format();
	printf("Qt OpenGL Context Version: %d.%d\n", fmt_actual.majorVersion(),
		fmt_actual.minorVersion());
	if(fmt_actual.profile() == QGLFormat::NoProfile)
	{
		printf("Qt OpenGL Profile: NoProfile\n");
	}
	else if(fmt_actual.profile() == QGLFormat::CoreProfile)
	{
		printf("Qt OpenGL Profile: CoreProfile\n");
	}
	else if(fmt_actual.profile() == QGLFormat::CompatibilityProfile)
	{
		printf("Qt OpenGL Profile: CompatibilityProfile\n");
	}
#endif

	camLbl = new QLabel("Camera", this);
	camLbl->setSizePolicy(rightSizePolicy);
	camLbl->setMaximumWidth(larger_width);
	camxLbl = new QLabel("0.0", this);
	camxLbl->setSizePolicy(rightSizePolicy);
	camxLbl->setMaximumWidth(smaller_width);
	camyLbl = new QLabel("0.0", this);
	camyLbl->setSizePolicy(rightSizePolicy);
	camyLbl->setMaximumWidth(smaller_width);
	camzLbl = new QLabel("0.0", this);
	camzLbl->setSizePolicy(rightSizePolicy);
	camzLbl->setMaximumWidth(smaller_width);

	piLbl = new QLabel("Selected", this);
	piLbl->setSizePolicy(rightSizePolicy);
	piLbl->setMaximumWidth(larger_width);
	pposLbl = new QLabel("Position", this);
	pposLbl->setSizePolicy(rightSizePolicy);
	pposLbl->setMaximumWidth(larger_width);
	pindLbl = new QLabel("Index", this);
	pindLbl->setSizePolicy(rightSizePolicy);
	pindLbl->setMaximumWidth(larger_width);
	pvelLbl = new QLabel("Velocity", this);
	pvelLbl->setSizePolicy(rightSizePolicy);
	pvelLbl->setMaximumWidth(larger_width);

	pposX = new QLabel("0.0", this);
	pposX->setSizePolicy(rightSizePolicy);
	pposX->setMaximumWidth(smaller_width);
	pposY = new QLabel("0.0", this);
	pposY->setSizePolicy(rightSizePolicy);
	pposY->setMaximumWidth(smaller_width);
	pposZ = new QLabel("0.0", this);
	pposZ->setSizePolicy(rightSizePolicy);
	pposZ->setMaximumWidth(smaller_width);

	pvelX = new QLabel("0.0", this);
	pvelX->setSizePolicy(rightSizePolicy);
	pvelX->setMaximumWidth(smaller_width);
	pvelY = new QLabel("0.0", this);
	pvelY->setSizePolicy(rightSizePolicy);
	pvelY->setMaximumWidth(smaller_width);
	pvelZ = new QLabel("0.0", this);
	pvelZ->setSizePolicy(rightSizePolicy);
	pvelZ->setMaximumWidth(smaller_width);

	pindX = new QLabel("0.0", this);
	pindX->setSizePolicy(rightSizePolicy);
	pindX->setMaximumWidth(smaller_width);
	pindY = new QLabel("0.0", this);
	pindY->setSizePolicy(rightSizePolicy);
	pindY->setMaximumWidth(smaller_width);
	pindZ = new QLabel("0.0", this);
	pindZ->setSizePolicy(rightSizePolicy);
	pindZ->setMaximumWidth(smaller_width);

	// displays at bottom
	fpsLbl = new QLabel("FPS", this);
	fpsLbl->setSizePolicy(rightSizePolicy);
	fpsLbl->setMaximumWidth(larger_width);
	fpsVal = new QLabel("0", this);
	fpsVal->setSizePolicy(rightSizePolicy);
	fpsVal->setMaximumWidth(larger_width);

	mainLayout = new QHBoxLayout(mainWidget);
	leftLayout = new QVBoxLayout;
	rightLayout = new QVBoxLayout;

	leftLayout->addWidget(glWidget);

	rightLayout->addWidget(camLbl);
	rightLayout->addWidget(camxLbl);
	rightLayout->addWidget(camyLbl);
	rightLayout->addWidget(camzLbl);

	rightLayout->addWidget(piLbl);
	rightLayout->addWidget(pindLbl);
	rightLayout->addWidget(pindX);
	rightLayout->addWidget(pindY);
	rightLayout->addWidget(pindZ);

	rightLayout->addWidget(pposLbl);
	rightLayout->addWidget(pposX);
	rightLayout->addWidget(pposY);
	rightLayout->addWidget(pposZ);

	rightLayout->addWidget(pvelLbl);
	rightLayout->addWidget(pvelX);
	rightLayout->addWidget(pvelY);
	rightLayout->addWidget(pvelZ);

	rightLayout->addStretch();
	rightLayout->addWidget(fpsLbl);
	rightLayout->addWidget(fpsVal);

	mainLayout->addLayout(leftLayout);
	mainLayout->addLayout(rightLayout);

	this->setCentralWidget(mainWidget);

	fflush(stdout);
}

//------------------------------------------------------------------------------
void MainWindow::updatePickerInfo(glm::uvec3 i, glm::vec3 p, glm::vec3 v)
{
	QString s;

	pindX->setText(s.number(i.x));
	pindY->setText(s.number(i.y));
	pindZ->setText(s.number(i.z));

	pposX->setText(s.number(p.x));
	pposY->setText(s.number(p.y));
	pposZ->setText(s.number(p.z));

	pvelX->setText(s.number(v.x));
	pvelY->setText(s.number(v.y));
	pvelZ->setText(s.number(v.z));
}

//------------------------------------------------------------------------------
void MainWindow::updateCamera(glm::vec3 cam)
{
	QString s;
	camxLbl->setText(s.number(cam.x));
	camyLbl->setText(s.number(cam.y));
	camzLbl->setText(s.number(cam.z));
}

//------------------------------------------------------------------------------
void MainWindow::fixFocus()
{
	glWidget->setFocus();
}

//------------------------------------------------------------------------------
void MainWindow::updateFps(int fps)
{
	QString s;
	fpsVal->setText(s.number(fps));
}
