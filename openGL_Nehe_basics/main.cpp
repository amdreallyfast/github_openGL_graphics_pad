#include <Windows.h>    // header file for windows
#include <windowsx.h>   // header file for "extended windows" because WNDCLASS is depreciated
#include <gl\GL.h>      // header file for the openGL ??version?? library
#include <gl\GLU.h>     // header file for the GL??version?? library

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLu32.lib")


// declare some persistent globals for rendering and device contexts
// - We will have a window, which is interfaced with in the Win32 world through a
// handle.  We will draw to this window.
// - The window will be connected to our application through the application 
// instance handle. 
// - The window is drawn to through the graphic device interface (GDI).  The device
// context links the window and GDI.
// - The OpenGL links to the device context through the render context.
// Summary: OpenGL API call -> render context -> device context -> GDI -> window
// ??Or something like that??
HGLRC g_render_context_handle = NULL;
HDC g_device_context_handle = NULL;
HWND g_window_handle = NULL;
HINSTANCE g_application_instance_handle;

// declare some miscellaneous globals 
// - An array of booleans to indicate which keyboard keys are currently selected.
// - A boolean to indicate if the application is minimized or active 
// (default true).
// - A boolean to indicate if the application window is fullscreen or not 
// (default true).
bool g_keys[256];
bool g_active = true;
bool g_fullscreen = true;


// forward declaration of the window message processing function
// Note: You only need to specify the types in forward declarations.
LRESULT CALLBACK my_window_proc(HWND, UINT, WPARAM, LPARAM);

// this function will resize the viewpoirt whenever our window has been resized
GLvoid resize_GL_scene(GLsizei new_width, GLsizei new_height)
{
   // this check is a crude and simple yet effective way to avoid an aspect ratio
   // problem of dividing by 0
   if (0 == new_height)
   {
      new_height = 1;
   }

   // resize the GL viewport
   glViewport(0, 0, new_width, new_height);

   // select the projection matrix, reset it, and set the perspective matrix
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(
      45.0f,            // 45 degree field of view
      (GLfloat)new_width / (GLfloat)new_height,  // aspect ratio
      0.1f,             // near plane
      100.0f);          // far plane

   // load and reset the model-view matrix
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}


// do all setup for openGL
bool init_GL()
{
   glShadeModel(GL_SMOOTH);
   glClearColor(0.0f, 0.0f, 1.0f, 0.0f);

   glClearDepth(1.0f);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);    // depth test is "less than or equal"

   // take a small performance hit, but make the perspective look better
   //??try leaving out??
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 

   return true;
}


// this is variable code for rendering a single frame
bool draw_GL_scene()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // reset the current model-view matrix
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // ??stuff here??
   
   return true;
}


// this function releases the context resources and the window handle, and it is 
// called when the program starts to quit
GLvoid kill_GL_window()
{
   // on some video cards, destroying the window before disabling fullscreen mode
   // will make the desktop corrupt, so change it to window mode before killing it
   if (g_fullscreen)
   {
      ChangeDisplaySettings(NULL, 0);
      ShowCursor(TRUE);
   }

   // check if we have a render context handle 
   if (g_render_context_handle)
   {
      // try to release the render and device contexts from use by telling GL to
      // use NULL for both
      if (!wglMakeCurrent(NULL, NULL))
      {
         // couldn't stop using one or both of the contexts; show an error message
         MessageBox(
            NULL,
            L"Release of DC and RC failed.",
            L"SHUTDOW ERROR",
            MB_OK | MB_ICONINFORMATION);
      }

      // regardless of what happened, try to delete the render context
      if (!wglDeleteContext(g_render_context_handle))
      {
         // failed to delete
         MessageBox(
            NULL,
            L"Release rendering context failed.",
            L"SHUTDOW ERROR",
            MB_OK | MB_ICONINFORMATION);
      }

      // regardless of what happened (??good idea??), set the handle to NULL
      g_render_context_handle = NULL;
   }

   // check for a device context and delete it if it exists
   //??how does this check work? is the first half even necessary??
   if (g_device_context_handle && 
      !ReleaseDC(g_window_handle, g_device_context_handle))
   {
      // failed to release device context
      MessageBox(
         NULL,
         L"Release device context failed.",
         L"SHUTDOWN ERROR",
         MB_OK | MB_ICONINFORMATION);

      // force the handle to NULL (??good idea??)
      g_device_context_handle = NULL;
   }

   // check for a window handle and delete it if it exists
   if (g_window_handle && !DestroyWindow(g_window_handle))
   {
      MessageBox(
         NULL,
         L"Could not release window handle.",
         L"SHUTDOWN ERROR",
         MB_OK | MB_ICONINFORMATION);

      // force the handle to NULL (??good idea??)
      g_window_handle = NULL;
   }

   // lastly, try to unregister our window class from the application so that the
   // window can be properly killed and won't leave a dangling registered window
   //??how would this happen??
   if (!UnregisterClass(L"OpenGL", g_application_instance_handle))
   {
      // couldn't unregister window
      MessageBox(
         NULL,
         L"Could not unregister class.",
         L"SHUTDOWN ERROR",
         MB_OK | MB_ICONINFORMATION);

      // force the handle to NULL (??good idea??)
      g_application_instance_handle = NULL;
   }
}



static const UINT SCREEN_WIDTH_PIXELS = 800;
static const UINT SCREEN_HEIGHT_PIXELS = 600;


// this is the main message handler for the program
LRESULT CALLBACK my_window_proc(
   HWND window_handle,
   UINT message,
   WPARAM w_param, //??the hey is this??
   LPARAM l_param) //??the hey is this??)
{
   switch (message)
   {
   case WM_DESTROY:
      // this message is delivered to the program when the window is clossed
      PostQuitMessage(0);
      return 0;
      // no need for break because "return" is just before it
   default:
      break;
   }

   // call the default window procedure to process any messages that we are not
   // explicitly handling
   return DefWindowProc(window_handle, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE h_instance,
   HINSTANCE h_prev_instance,
   LPSTR lp_cmd_line,
   int n_show_cmd)
{
   // this will be the handle for the window
   HWND window_handle;

   // this struct will hold information for the window class
   WNDCLASSEX wc;
   ZeroMemory(&wc, sizeof(WNDCLASSEX));

   // fill the strict with behavioral information
   wc.cbSize = sizeof(WNDCLASSEX);           // ??why? is this legacy stuff??
   wc.style = CS_HREDRAW | CS_VREDRAW;       // redraw on vertical or horizatial resize
   wc.lpfnWndProc = my_window_proc;          // a function pointer to our custom window message handling function
   wc.hInstance = h_instance;                // the handle to our applicaiton instance
   wc.hCursor = LoadCursor(NULL, IDC_ARROW); // no application handle; default mouse pointer
   wc.lpszClassName = L"WindowClass1";       // name for window class

   RegisterClassEx(&wc);

   // calculate the window size necessary to enclose our desired-size window 
   // rectangle
   RECT window_rectangle =
   {
      0,                      // origin is (0,0) (origin doesn't matter because we
      0,                      // only need the width and heigh for calculations)
      SCREEN_WIDTH_PIXELS,
      SCREEN_HEIGHT_PIXELS
   };
   AdjustWindowRectEx(
      &window_rectangle,      // pointer to a rectangle
      WS_OVERLAPPEDWINDOW,    // default window options (necessary for calculations)
      FALSE,                  // no menus
      NULL);                  // no "extended window" styles

   // create the window on the screen

   window_handle = CreateWindowEx(
      NULL,                         // no extra style options
      L"WindowClass1",              // name of the window class that this handle will refer to
      L"'Hello World' Win32 Window Program", // displayed on window's title bar
      WS_OVERLAPPEDWINDOW,          // several default window options
      300,                          // X origin relative to desktop
      300,                          // Y origin relative to desktop
      window_rectangle.right - window_rectangle.left, // window width
      window_rectangle.bottom - window_rectangle.top, // window height
      NULL,                         // no parent window
      NULL,                         // no menus
      h_instance,                   // identifies window with this application
      NULL);                        // no multiple windows

   // display window on screen
   ShowWindow(window_handle, n_show_cmd);

   // enter the main loop

   // this struct holds Windows event message
   MSG event_message = { 0 };

   // enter the loop and stay here forever (or at least until the window closes)
   while (true)
   {
      // check to see if any messages are in the queue
      // Note: Remove them if there are any, then handle them
      if (PeekMessage(&event_message, NULL, 0, 0, PM_REMOVE))
      {
         TranslateMessage(&event_message);
         DispatchMessage(&event_message);

         if (WM_QUIT == event_message.message)
         {
            break;
         }
      }
   }

   
   return event_message.wParam;
}