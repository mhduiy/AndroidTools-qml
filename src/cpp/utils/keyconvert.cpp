#include "keyconvert.h"
#include "../adb/keycodes.h"

KeyConvert::KeyConvert(QObject *parent) : QObject(parent)
{

}

int KeyConvert::convert(Qt::Key key)
{
    int keyCode = AKEYCODE_UNKNOWN;
    switch (key) {
    case Qt::Key_0:
        keyCode = AKEYCODE_0;
        break;
    case Qt::Key_1:
        keyCode = AKEYCODE_1;
        break;
    case Qt::Key_2:
        keyCode = AKEYCODE_2;
        break;
    case Qt::Key_3:
        keyCode = AKEYCODE_3;
        break;
    case Qt::Key_4:
        keyCode = AKEYCODE_4;
        break;
    case Qt::Key_5:
        keyCode = AKEYCODE_5;
        break;
    case Qt::Key_6:
        keyCode = AKEYCODE_6;
        break;
    case Qt::Key_7:
        keyCode = AKEYCODE_7;
        break;
    case Qt::Key_8:
        keyCode = AKEYCODE_8;
        break;
    case Qt::Key_9:
        keyCode = AKEYCODE_9;
        break;

    case Qt::Key_A:
        keyCode = AKEYCODE_A;
        break;
    case Qt::Key_B:
        keyCode = AKEYCODE_B;
        break;
    case Qt::Key_C:
        keyCode = AKEYCODE_C;
        break;
    case Qt::Key_D:
        keyCode = AKEYCODE_D;
        break;
    case Qt::Key_E:
        keyCode = AKEYCODE_E;
        break;
    case Qt::Key_F:
        keyCode = AKEYCODE_F;
        break;
    case Qt::Key_G:
        keyCode = AKEYCODE_G;
        break;
    case Qt::Key_H:
        keyCode = AKEYCODE_H;
        break;
    case Qt::Key_I:
        keyCode = AKEYCODE_I;
        break;
    case Qt::Key_J:
        keyCode = AKEYCODE_J;
        break;
    case Qt::Key_K:
        keyCode = AKEYCODE_K;
        break;
    case Qt::Key_L:
        keyCode = AKEYCODE_L;
        break;
    case Qt::Key_M:
        keyCode = AKEYCODE_M;
        break;
    case Qt::Key_N:
        keyCode = AKEYCODE_N;
        break;
    case Qt::Key_O:
        keyCode = AKEYCODE_O;
        break;
    case Qt::Key_P:
        keyCode = AKEYCODE_P;
        break;
    case Qt::Key_Q:
        keyCode = AKEYCODE_Q;
        break;
    case Qt::Key_R:
        keyCode = AKEYCODE_R;
        break;
    case Qt::Key_S:
        keyCode = AKEYCODE_S;
        break;
    case Qt::Key_T:
        keyCode = AKEYCODE_T;
        break;
    case Qt::Key_U:
        keyCode = AKEYCODE_U;
        break;
    case Qt::Key_V:
        keyCode = AKEYCODE_V;
        break;
    case Qt::Key_W:
        keyCode = AKEYCODE_W;
        break;
    case Qt::Key_X:
        keyCode = AKEYCODE_X;
        break;
    case Qt::Key_Y:
        keyCode = AKEYCODE_Y;
        break;
    case Qt::Key_Z:
        keyCode = AKEYCODE_Z;
        break;

    case Qt::Key_F1:
        keyCode = AKEYCODE_F1;
        break;
    case Qt::Key_F2:
        keyCode = AKEYCODE_F2;
        break;
    case Qt::Key_F3:
        keyCode = AKEYCODE_F3;
        break;
    case Qt::Key_F4:
        keyCode = AKEYCODE_F4;
        break;
    case Qt::Key_F5:
        keyCode = AKEYCODE_F5;
        break;
    case Qt::Key_F6:
        keyCode = AKEYCODE_F6;
        break;
    case Qt::Key_F7:
        keyCode = AKEYCODE_F7;
        break;
    case Qt::Key_F8:
        keyCode = AKEYCODE_F8;
        break;
    case Qt::Key_F9:
        keyCode = AKEYCODE_F9;
        break;
    case Qt::Key_F10:
        keyCode = AKEYCODE_F10;
        break;
    case Qt::Key_F11:
        keyCode = AKEYCODE_F11;
        break;
    case Qt::Key_F12:
        keyCode = AKEYCODE_F12;
        break;

    case Qt::Key_Shift:
        keyCode = AKEYCODE_SHIFT_LEFT;
        break;
    case Qt::Key_Alt:
        keyCode = AKEYCODE_ALT_LEFT;
        break;
    case Qt::Key_Tab:
        keyCode = AKEYCODE_TAB;
        break;
    case Qt::Key_Control:
        keyCode = AKEYCODE_CTRL_LEFT;
        break;
    case Qt::Key_Space:
        keyCode = AKEYCODE_SPACE;
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        keyCode = AKEYCODE_ENTER;
        break;

    case Qt::Key_Up:
        keyCode = AKEYCODE_DPAD_UP;
        break;
    case Qt::Key_Down:
        keyCode = AKEYCODE_DPAD_DOWN;
        break;
    case Qt::Key_Left:
        keyCode = AKEYCODE_DPAD_LEFT;
        break;
    case Qt::Key_Right:
        keyCode = AKEYCODE_DPAD_RIGHT;
        break;

    case Qt::Key_Escape:
        keyCode = AKEYCODE_ESCAPE;
        break;
    case Qt::Key_Backspace:
        keyCode = AKEYCODE_DEL;
        break;

    default:
        keyCode = AKEYCODE_UNKNOWN;
    }
    return keyCode;
}
