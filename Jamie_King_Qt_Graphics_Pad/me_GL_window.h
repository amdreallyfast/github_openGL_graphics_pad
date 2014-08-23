#ifndef ME_GL_WINDOW_H
#define ME_GL_WINDOW_H

#include <QtOpenGL\qglwidget>
#include <QtGui\qmouseevent>

//#pragma once

// this class is mostly here to abstract away QT's OpenGL window widget
class me_GL_window : public QGLWidget
{
public:

protected:
	// override the widget's initialization method
	void initializeGL();

	// override the widget's "paint GL" method, which is the method that QT calls whenever it needs to do some painting commands to the window through OpenGL
	void paintGL();

   // override the widget's "mouse move event"
   // this will ??do what??
   void mouseMoveEvent(QMouseEvent*);
   void mousePressEvent(QMouseEvent*);
   void mouseReleaseEvent(QMouseEvent*);

   // for keyboard inputs
   void keyPressEvent(QKeyEvent* );

private:
   // define this within this class so that it can have access to the width() and height() functions for
   // when it needs to calculate aspect ratio
   void send_data_to_open_GL();
};


#endif