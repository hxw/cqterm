// xinterface.cpp

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <iostream>
#include <unistd.h>


// wait for a sub window to be establisheshed and get its ID
int getChildIdFrom(int winId) {

	auto *d = XOpenDisplay(NULL);
	auto done = false;
	auto childId = 0; // error

	for (auto i = 0; !done && i < 20; ++i) {

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
				childId = childWindows[0];
				done = true;
				break;
			}
		}
		usleep(25000); // delay to allow child application to open its windows
		XFree(childWindows);
	}
	XCloseDisplay(d);
	return childId;
}


void resizeChild(int childId, int w, int h) {
	auto *d = XOpenDisplay(NULL);
	XResizeWindow(d, childId, w, h);
	XCloseDisplay(d);
}


typedef struct {
	char c;
	unsigned int state;
	unsigned int keycode;
} keymapping;
// convert ASCII character to the correct key
// unfortunately since key codes are sent the
// corresponding shift is also needed
// e.g. the key codes for 2 and @ are the same
// this code probably is tied to the US-international layout
#define NoModMask 0
#define SizeOfArray(a) (sizeof(a) / sizeof((a)[0]))
static const keymapping keys[] = {
	{'\r', NoModMask, XK_Return},     {'\n', NoModMask, XK_Linefeed},
	{' ', NoModMask, XK_space},       {'!', ShiftMask, XK_exclam},
	{'"', ShiftMask, XK_quotedbl},    {'#', ShiftMask, XK_numbersign},
	{'$', ShiftMask, XK_dollar},      {'%', ShiftMask, XK_percent},
	{'&', ShiftMask, XK_ampersand},   {'\'', NoModMask, XK_apostrophe},
	{'(', ShiftMask, XK_parenleft},   {')', ShiftMask, XK_parenright},
	{'*', ShiftMask, XK_asterisk},    {'+', ShiftMask, XK_plus},
	{',', NoModMask, XK_comma},       {'-', NoModMask, XK_minus},
	{'.', NoModMask, XK_period},      {'/', NoModMask, XK_slash},
	{'0', NoModMask, XK_0},		  {'1', NoModMask, XK_1},
	{'2', NoModMask, XK_2},		  {'3', NoModMask, XK_3},
	{'4', NoModMask, XK_4},		  {'5', NoModMask, XK_5},
	{'6', NoModMask, XK_6},		  {'7', NoModMask, XK_7},
	{'8', NoModMask, XK_8},		  {'9', NoModMask, XK_9},
	{':', ShiftMask, XK_colon},       {';', NoModMask, XK_semicolon},
	{'<', ShiftMask, XK_less},	{'=', NoModMask, XK_equal},
	{'>', ShiftMask, XK_greater},     {'?', ShiftMask, XK_question},
	{'@', ShiftMask, XK_at},	  {'A', ShiftMask, XK_A},
	{'B', ShiftMask, XK_B},		  {'C', ShiftMask, XK_C},
	{'D', ShiftMask, XK_D},		  {'E', ShiftMask, XK_E},
	{'F', ShiftMask, XK_F},		  {'G', ShiftMask, XK_G},
	{'H', ShiftMask, XK_H},		  {'I', ShiftMask, XK_I},
	{'J', ShiftMask, XK_J},		  {'K', ShiftMask, XK_K},
	{'L', ShiftMask, XK_L},		  {'M', ShiftMask, XK_M},
	{'N', ShiftMask, XK_N},		  {'O', ShiftMask, XK_O},
	{'P', ShiftMask, XK_P},		  {'Q', ShiftMask, XK_Q},
	{'R', ShiftMask, XK_R},		  {'S', ShiftMask, XK_S},
	{'T', ShiftMask, XK_T},		  {'U', ShiftMask, XK_U},
	{'V', ShiftMask, XK_V},		  {'W', ShiftMask, XK_W},
	{'X', ShiftMask, XK_X},		  {'Y', ShiftMask, XK_Y},
	{'Z', ShiftMask, XK_Z},		  {'[', NoModMask, XK_bracketleft},
	{'\\', NoModMask, XK_backslash},  {']', NoModMask, XK_bracketright},
	{'^', ShiftMask, XK_asciicircum}, {'_', ShiftMask, XK_underscore},
	{'`', NoModMask, XK_grave},       {'a', NoModMask, XK_a},
	{'b', NoModMask, XK_b},		  {'c', NoModMask, XK_c},
	{'d', NoModMask, XK_d},		  {'e', NoModMask, XK_e},
	{'f', NoModMask, XK_f},		  {'g', NoModMask, XK_g},
	{'h', NoModMask, XK_h},		  {'i', NoModMask, XK_i},
	{'j', NoModMask, XK_j},		  {'k', NoModMask, XK_k},
	{'l', NoModMask, XK_l},		  {'m', NoModMask, XK_m},
	{'n', NoModMask, XK_n},		  {'o', NoModMask, XK_o},
	{'p', NoModMask, XK_p},		  {'q', NoModMask, XK_q},
	{'r', NoModMask, XK_r},		  {'s', NoModMask, XK_s},
	{'t', NoModMask, XK_t},		  {'u', NoModMask, XK_u},
	{'v', NoModMask, XK_v},		  {'w', NoModMask, XK_w},
	{'x', NoModMask, XK_x},		  {'y', NoModMask, XK_y},
	{'z', NoModMask, XK_z},		  {'{', ShiftMask, XK_braceleft},
	{'|', ShiftMask, XK_bar},	 {'}', ShiftMask, XK_braceright},
	{'~', ShiftMask, XK_asciitilde},
};


void sendKeysToChild(int childId, std::string lines) {

	auto *d = XOpenDisplay(NULL);

	for (auto &c : lines) {
		for (unsigned int i = 0; i < sizeof(keys); ++i) {
			if (keys[i].c == c) {
				XKeyEvent ev = {};
				ev.type = KeyPress;
				ev.display = d;
				ev.root = DefaultRootWindow(d);
				ev.window = childId;
				ev.state = keys[i].state;
				ev.keycode = XKeysymToKeycode(d, keys[i].keycode);

				XSendEvent(d, childId, true, KeyPressMask, (XEvent *)(&ev));
				ev.type = KeyRelease;
				XSendEvent(d, childId, true, KeyReleaseMask, (XEvent *)(&ev));

				XFlush(d);
				usleep(1000);
				break;
			}
		}
	}

	XCloseDisplay(d);
}


void sendKeyCode(int childId, bool press, int modifiers, int scancode) {
	auto *d = XOpenDisplay(NULL);

	auto mask = press ? KeyPressMask : KeyReleaseMask;

	XKeyEvent ev = {};
	ev.type = press ? KeyPress : KeyRelease;
	ev.display = d;
	ev.root = DefaultRootWindow(d);
	ev.window = childId;
	ev.state = modifiers;
	ev.keycode = scancode;

	XSendEvent(d, childId, true, mask, (XEvent *)(&ev));

	XFlush(d);
	XCloseDisplay(d);
}
