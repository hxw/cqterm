// xinterface.cpp

#include <iostream>
#include <X11/Xlib.h>


void resizeChild(int winId, int w, int h) {

	auto *d = XOpenDisplay(NULL);

	Window rootWindow = 0;
	Window parentWindow = 0;
	unsigned int childCount = 0;
	Window *childWindows = NULL;

	Status s = XQueryTree(d, winId, &rootWindow, &parentWindow, &childWindows, &childCount);

	if (0 != s) {
#if 0
		std::cout << "rw: " << rootWindow << std::endl;
		std::cout << "pw: " << parentWindow << std::endl;
		std::cout << "Nc: " << childCount << std::endl;
#endif
		if (childCount >= 1) {
#if 0
			std::cout << "c0: " << childWindows[0] << std::endl;
#endif
			XResizeWindow(d, childWindows[0], w, h);
		}
	}
	XFree(childWindows);

	XCloseDisplay(d);
}
