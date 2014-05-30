#include <Qt\qapplication.h>

#include "me_GL_window.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	me_GL_window window_for_doing_gl_stuff;
	window_for_doing_gl_stuff.show();

	return app.exec();
}