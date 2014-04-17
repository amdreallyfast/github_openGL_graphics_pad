#ifndef ME_GL_WINDOW_H
#define ME_GL_WINDOW_H

#include <QtOpenGL\qglwidget>

//#pragma once

// this class is mostly here to abstract away QT's OpenGL window widget
class me_GL_window : public QGLWidget
{
protected:
	// override the widget's initialization method
	void initializeGL();

	// override the widget's "paint GL" method, which is the method that QT calls whenever it needs to do some painting commands to the window through OpenGL
	void paintGL();
public:
};


#endif