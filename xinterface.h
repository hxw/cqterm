// xinterface.h          -*- mode: c++ -*-

#if !defined(_XINTERFACE_H_)
#define _XINTERFACE_H_ 1

int getChildIdFrom(int winId);
void resizeChild(int childId, int w, int h);
void sendKeysToChild(int childId, std::string lines);

void sendKeyCode(int childId, bool press, int modifiers, int scancode);

#endif
