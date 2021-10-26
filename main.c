#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Context {
    Display* display;
    Window window;

    int displayWidth;
    int displayHeight;
};

Window GetActiveWindow(Display* d)
{
    Window root, parent, window, *children = NULL;
    int revert_to;
    unsigned int num_children;

    XGetInputFocus(d, &window, &revert_to);
    XQueryTree(d, window, &root, &parent, &children, &num_children);

    return parent;
}

struct Context CreateContext(Display* display)
{
    struct Context context;
    int screenNumber;

    screenNumber = DefaultScreen(display);

    context.display = display;
    context.displayWidth = DisplayWidth(display, screenNumber);
    context.displayHeight = DisplayHeight(display, screenNumber);
    context.window = GetActiveWindow(display);

    return context;
}

void TileLeft(struct Context context)
{
    XResizeWindow(context.display, context.window, context.displayWidth / 2, context.displayHeight);
    XMoveWindow(context.display, context.window, 0, 0);

    XSync(context.display, False);
}

void TileRight(struct Context context)
{
    XWindowAttributes attributes;
    XGetWindowAttributes(context.display, context.window, &attributes);

    XResizeWindow(context.display, context.window, context.displayWidth / 2, context.displayHeight);
    XMoveWindow(context.display, context.window, context.displayWidth - attributes.width, 0);

    XSync(context.display, False);
}

void TileCenter(struct Context context)
{
    XWindowAttributes attributes;
    XGetWindowAttributes(context.display, context.window, &attributes);

    int x = (context.displayWidth / 2) - (attributes.width / 2);
    int y = (context.displayHeight / 2) - (attributes.height / 2);

    XMoveWindow(context.display, context.window, x, y);
    XSync(context.display, False);
}

int main(int argc, char* argv[])
{
    Display* display;
    struct Context context;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    context = CreateContext(display);

    if (strcmp(argv[1], "left") == 0) {
        TileLeft(context);
    } else if (strcmp(argv[1], "right") == 0) {
        TileRight(context);
    } else if (strcmp(argv[1], "center") == 0) {
        TileCenter(context);
    }

    return 0;
}
