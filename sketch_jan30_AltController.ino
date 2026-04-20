#include <AltController.h>

AltController controller;

void setup() {
    controller.addKeyPress(1, KEY_UP_ARROW, 25);
    controller.addKeyPress(2, KEY_RIGHT_ARROW, 25);
    controller.addKeyPress(3, KEY_LEFT_ARROW, 25);
    controller.addKeyPress(4, KEY_DOWN_ARROW, 25);
    controller.addPrint(5, " "); //jump

    controller.addPrint(6, "/", 100); //open chat
    controller.addPrint(7, "i love pizza");
    controller.addPrint(8, "go away");
    controller.addPrint(9, "/e dance");
    controller.addKeyPress(10, KEY_RETURN);

    controller.addMacro(11, {KEY_LEFT_CTRL, 'n'}, 100);
    controller.addPrint(12, "https://youtu.be/dQw4w9WgXcQ?si=9M4qjSLUECJItCXb");
    controller.addKeyPress(13, KEY_RETURN);

}

void loop() {
    controller.update();
}