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


bool create_GL_window(char *title_arg, int width_arg, int height_arg, int bits_arg, bool fullscreen_flag_arg)
{
   LPCWSTR window_class_name = L"OpenGL";

   // make and fill in the window class structure
   WNDCLASS wc;
   wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  // redraw on move, use our own device context instance for this window
   wc.lpfnWndProc = my_window_proc;    // the window message processing function
   wc.cbClsExtra = 0;                  // no extra window data
   wc.cbWndExtra = 0;                  // no extra window data

   g_application_instance_handle = GetModuleHandle(NULL);
   wc.hInstance = g_application_instance_handle;   // the application that this window belongs to
   wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);   // default window icon
   wc.hCursor = LoadCursor(NULL, IDC_ARROW); // ??what kind?? arrow pointer
   wc.hbrBackground = NULL;            // default background is black; openGL draws this
   wc.lpszMenuName = NULL;             // no menu
   wc.lpszClassName = window_class_name;        // the window class name

   // register the window class
   if (!RegisterClass(&wc))
   {
      MessageBox(
         NULL,
         L"Failed to register the window class.",
         L"ERROR",
         MB_OK | MB_ICONEXCLAMATION);
      return false;
   }


   // handle the "fullscreen" options
   g_fullscreen = fullscreen_flag_arg;     // set the global flag
   if (g_fullscreen)
   {
      // create and configure a "device mode" structure
      DEVMODE dm_screen_settings;
      memset(&dm_screen_settings, 0, sizeof(dm_screen_settings));
      dm_screen_settings.dmSize = sizeof(dm_screen_settings);
      dm_screen_settings.dmPelsWidth = width_arg;      // pixel width
      dm_screen_settings.dmPelsHeight = height_arg;    // pixel height
      dm_screen_settings.dmBitsPerPel = bits_arg;      // bits per pixel
      dm_screen_settings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

      // try to change to fullscreen
      if (ChangeDisplaySettings(&dm_screen_settings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
      {
         if (IDYES ==
            MessageBox(
            NULL,
            L"The requested fullscreen mode is not supported by\nyour video card.\n\nUse windowed mode instead?",
            L"NeHe GL",
            MB_YESNO | MB_ICONEXCLAMATION))
         {
            // user decided to remain in windowed mode
            g_fullscreen = false;
         }
         else
         {
            // user said "no, don't stay in windowed mode", so quite
            MessageBox(
               NULL,
               L"Program will now close.",
               L"ERROR",
               MB_OK | MB_ICONSTOP);
            return false;
         }
      }
   }

   // store normal and extended styles in their own variables
   DWORD dw_extended_style;
   DWORD dw_style;

   // in the event that we are not fullscreen or the switch to fullscreen had a 
   // problem and the user chose to remain in window mode, check for fullscreen 
   // again
   if (g_fullscreen)
   {
      // still fullscreen, so record some style information

      // force a (??the??) top level window down to the taskbar once our window is
      // visible (??the wone that we are making??)
      dw_extended_style = WS_EX_APPWINDOW;
      
      // a popup window has no borders; excellent for fullscreen mode
      dw_style = WS_POPUP;

      // hide the cursor (this is a personal tutorial preference)
      ShowCursor(FALSE);
   }
   else
   {
      // not in fullscreen mode for whatever reason

      // the style will be a standard window with a fancy edge
      dw_extended_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
      dw_style = WS_OVERLAPPEDWINDOW;
   }

   // fill in the default values of the rectange that will be used to calculate the
   // required window dimenions based on the desired drawing area size
   RECT window_rect;
   window_rect.left = (long)0;
   window_rect.right = (long)width_arg;
   window_rect.top = (long)0;
   window_rect.bottom = (long)height_arg;
   
   // adjust the rectangle dimensions based on our styles so far
   //??what about the boolean "menu" option (third arg)? is that already covered by the styles and thus redundant??
   AdjustWindowRectEx(&window_rect, dw_style, FALSE, dw_extended_style);

   // create the window
   g_window_handle = CreateWindowEx(
      dw_extended_style,
      window_class_name,
      (LPCWSTR)title_arg,
      dw_style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,  // prevent other windows from drawing over/into this window (required for openGL to work properly)
      0,                      // default x
      0,                      // default y
      window_rect.right - window_rect.left,
      window_rect.bottom - window_rect.top,
      NULL,                   // no parent window
      NULL,                   // no menu
      g_application_instance_handle,
      NULL);                  // don't pass anything to WM_CREATE(??why??)

   if (!g_window_handle)
   {
      // failed to create window
      kill_GL_window();
      MessageBox(
         NULL,
         L"Window creation error.",
         L"ERROR",
         MB_OK | MB_ICONEXCLAMATION);
      return false;
   }

   // create a pixel format structure (??why static??)
   static PIXELFORMATDESCRIPTOR pfd = 
   {
      sizeof(PIXELFORMATDESCRIPTOR),   // size of this descriptor
      1,                               // version number
      PFD_DRAW_TO_WINDOW |             // must support Win32 windows
      PFD_SUPPORT_OPENGL |             // must suppor openGL
      PFD_DOUBLEBUFFER,                // must support double buffering
      PFD_TYPE_RGBA,                   // color has red, green, blue, and alpha
      bits_arg,                        // set color depth (bits per color)
      0, 0, 0, 0, 0, 0,                // ignore color bits and bit shifts
      0,                               // no alpha buffer
      0,                               // ignore alpha bit shift 
      0,                               // no accumulation buffer
      0, 0, 0, 0,                      // ignore accumulation bits
      16,                              // 16bit Z-buffer (depth buffer)
      0,                               // no stencil buffer
      0,                               // no auxiliary buffer
      PFD_MAIN_PLANE,                  // main drawing layer
      0,                               // reserved (??for what??)
      0, 0, 0                          // layer masks ignored
   };

   // try to grab the device context so that you can set the pixel format
   if (!(g_device_context_handle = GetDC(g_window_handle)))
   {
      // couldn't get device context; bad
      kill_GL_window();
      MessageBox(
         NULL,
         L"Can't create a GL device context.",
         L"ERROR",
         MB_OK | MB_ICONEXCLAMATION);
      return false;
   }

   // try to find a pixel format that matches what we want
   GLuint pixel_format;
   if (!(pixel_format = ChoosePixelFormat(g_device_context_handle, &pfd)))
   {
      // couldn't find a pixel format that matched what we wanted
      kill_GL_window();
      MessageBox(
         NULL,
         L"Can't find a suitable pixel format.",
         L"ERROR",
         MB_OK | MB_ICONEXCLAMATION);
      return false;
   }

   // if a pixel format was found, try to set it
   if (!SetPixelFormat(g_device_context_handle, pixel_format, &pfd))
   {
      kill_GL_window();
      MessageBox(
         NULL,
         L"Can't set the pixel format.",
         L"ERROR",
         MB_OK | MB_ICONEXCLAMATION);
      return false;
   }

   // if pixel format was set properly, try to create a rendering context
   if (!(g_render_context_handle = wglCreateContext(g_device_context_handle)))
   {
      kill_GL_window();
      MessageBox(
         NULL,
         L"Can't create a GL rendering context.",
         L"ERROR",
         MB_OK | MB_ICONEXCLAMATION);
      return false;
   }

   // if we created both device and rendering contexts, then make the render 
   // context active
   if (!wglMakeCurrent(g_device_context_handle, g_render_context_handle))
   {
      kill_GL_window();
      MessageBox(
         NULL,
         L"Can't activate the GL rendering context.",
         L"ERROR",
         MB_OK | MB_ICONEXCLAMATION);
      return false;
   }

   // if everything went smoothly and our window was created, then show the window,
   // set it as the foreground window, set the focus to be that window, resize the
   // viewport to match, and finally run the "init GL" function for setting various
   // matrices
   ShowWindow(g_window_handle, SW_SHOW);
   SetForegroundWindow(g_window_handle);
   SetFocus(g_window_handle);
   resize_GL_scene(width_arg, height_arg);
   if (!init_GL())
   {
      kill_GL_window();
      MessageBox(
         NULL,
         L"GL init failed.",
         L"ERROR",
         MB_OK | MB_ICONEXCLAMATION);
      return false;
   }


   // we made it this far, so it is safe to assume that everything went well and 
   // that we can return "true"
   return true;
}


// define the window message processing function
LRESULT CALLBACK my_window_proc(
   HWND window_handle_arg,    // handle for the window that the message came from
   UINT u_msg,                // message for the window
   WPARAM w_param,            // additional message information
   LPARAM l_param)            // additional message information
{
   switch (u_msg)
   {
   case WM_ACTIVATE:
   {
      // check minimization state
      if (!HIWORD(w_param))
      {
         // set program as active
         g_active = true;
      }
      else
      {
         // set program as inactive
         g_active = false;
      }

      return 0;
   }
   case WM_SYSCOMMAND:
   {
      switch (w_param)
      {
      case SC_SCREENSAVE:     // screensaver trying to start?
      case SC_MONITORPOWER:   // monitor trying to enter power save mode?
         return 0;            // prevent the monitor from doing that (??how??)

      default:
         break;
      }

      break;
   }
   case WM_CLOSE:
   {
      PostQuitMessage(0);     // send a "quit" message with exit code 0 ??to where??
      return 0;
   }
   case WM_KEYDOWN:
   {
      // a key is being held down
      g_keys[w_param] = true;
      return 0;
   }
   case WM_KEYUP:
   {
      // a key is being released
      g_keys[w_param] = false;
      return 0;
   }
   case WM_SIZE:
   {
      resize_GL_scene(LOWORD(w_param), HIWORD(l_param));
      return 0;
   }

   default:
      break;
   }

   // pass all unhandled messages to the default window message processor
   return DefWindowProc(window_handle_arg, u_msg, w_param, l_param);
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