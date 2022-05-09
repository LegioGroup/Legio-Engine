#pragma once
#include <glm/glm.hpp>

/* From GLFW3.h */
#define LG_KEY_SPACE              32
#define LG_KEY_APOSTROPHE         39  /* ' */
#define LG_KEY_COMMA              44  /* , */
#define LG_KEY_MINUS              45  /* - */
#define LG_KEY_PERIOD             46  /* . */
#define LG_KEY_SLASH              47  /* / */
#define LG_KEY_0                  48
#define LG_KEY_1                  49
#define LG_KEY_2                  50
#define LG_KEY_3                  51
#define LG_KEY_4                  52
#define LG_KEY_5                  53
#define LG_KEY_6                  54
#define LG_KEY_7                  55
#define LG_KEY_8                  56
#define LG_KEY_9                  57
#define LG_KEY_SEMICOLON          59  /* ; */
#define LG_KEY_EQUAL              61  /* = */
#define LG_KEY_A                  65
#define LG_KEY_B                  66
#define LG_KEY_C                  67
#define LG_KEY_D                  68
#define LG_KEY_E                  69
#define LG_KEY_F                  70
#define LG_KEY_G                  71
#define LG_KEY_H                  72
#define LG_KEY_I                  73
#define LG_KEY_J                  74
#define LG_KEY_K                  75
#define LG_KEY_L                  76
#define LG_KEY_M                  77
#define LG_KEY_N                  78
#define LG_KEY_O                  79
#define LG_KEY_P                  80
#define LG_KEY_Q                  81
#define LG_KEY_R                  82
#define LG_KEY_S                  83
#define LG_KEY_T                  84
#define LG_KEY_U                  85
#define LG_KEY_V                  86
#define LG_KEY_W                  87
#define LG_KEY_X                  88
#define LG_KEY_Y                  89
#define LG_KEY_Z                  90
#define LG_KEY_LEFT_BRACKET       91  /* [ */
#define LG_KEY_BACKSLASH          92  /* \ */
#define LG_KEY_RIGHT_BRACKET      93  /* ] */
#define LG_KEY_GRAVE_ACCENT       96  /* ` */
#define LG_KEY_WORLD_1            161 /* non-US #1 */
#define LG_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define LG_KEY_ESCAPE             256
#define LG_KEY_ENTER              257
#define LG_KEY_TAB                258
#define LG_KEY_BACKSPACE          259
#define LG_KEY_INSERT             260
#define LG_KEY_DELETE             261
#define LG_KEY_RIGHT              262
#define LG_KEY_LEFT               263
#define LG_KEY_DOWN               264
#define LG_KEY_UP                 265
#define LG_KEY_PAGE_UP            266
#define LG_KEY_PAGE_DOWN          267
#define LG_KEY_HOME               268
#define LG_KEY_END                269
#define LG_KEY_CAPS_LOCK          280
#define LG_KEY_SCROLL_LOCK        281
#define LG_KEY_NUM_LOCK           282
#define LG_KEY_PRINT_SCREEN       283
#define LG_KEY_PAUSE              284
#define LG_KEY_F1                 290
#define LG_KEY_F2                 291
#define LG_KEY_F3                 292
#define LG_KEY_F4                 293
#define LG_KEY_F5                 294
#define LG_KEY_F6                 295
#define LG_KEY_F7                 296
#define LG_KEY_F8                 297
#define LG_KEY_F9                 298
#define LG_KEY_F10                299
#define LG_KEY_F11                300
#define LG_KEY_F12                301
#define LG_KEY_F13                302
#define LG_KEY_F14                303
#define LG_KEY_F15                304
#define LG_KEY_F16                305
#define LG_KEY_F17                306
#define LG_KEY_F18                307
#define LG_KEY_F19                308
#define LG_KEY_F20                309
#define LG_KEY_F21                310
#define LG_KEY_F22                311
#define LG_KEY_F23                312
#define LG_KEY_F24                313
#define LG_KEY_F25                314
#define LG_KEY_KP_0               320
#define LG_KEY_KP_1               321
#define LG_KEY_KP_2               322
#define LG_KEY_KP_3               323
#define LG_KEY_KP_4               324
#define LG_KEY_KP_5               325
#define LG_KEY_KP_6               326
#define LG_KEY_KP_7               327
#define LG_KEY_KP_8               328
#define LG_KEY_KP_9               329
#define LG_KEY_KP_DECIMAL         330
#define LG_KEY_KP_DIVIDE          331
#define LG_KEY_KP_MULTIPLY        332
#define LG_KEY_KP_SUBTRACT        333
#define LG_KEY_KP_ADD             334
#define LG_KEY_KP_ENTER           335
#define LG_KEY_KP_EQUAL           336
#define LG_KEY_LEFT_SHIFT         340
#define LG_KEY_LEFT_CONTROL       341
#define LG_KEY_LEFT_ALT           342
#define LG_KEY_LEFT_SUPER         343
#define LG_KEY_RIGHT_SHIFT        344
#define LG_KEY_RIGHT_CONTROL      345
#define LG_KEY_RIGHT_ALT          346
#define LG_KEY_RIGHT_SUPER        347
#define LG_KEY_MENU               348

#define LG_MOUSE_BUTTON_1         0
#define LG_MOUSE_BUTTON_2         1
#define LG_MOUSE_BUTTON_3         2
#define LG_MOUSE_BUTTON_4         3
#define LG_MOUSE_BUTTON_5         4
#define LG_MOUSE_BUTTON_6         5
#define LG_MOUSE_BUTTON_7         6
#define LG_MOUSE_BUTTON_8         7
#define LG_MOUSE_BUTTON_LAST      LG_MOUSE_BUTTON_8
#define LG_MOUSE_BUTTON_LEFT      LG_MOUSE_BUTTON_1
#define LG_MOUSE_BUTTON_RIGHT     LG_MOUSE_BUTTON_2
#define LG_MOUSE_BUTTON_MIDDLE    LG_MOUSE_BUTTON_3

namespace LG
{
    class Input
    {
    public:
        virtual void Init() = 0;
        virtual bool IsKeyPressed(int keyCode) = 0;
        virtual bool IsMouseButtonPressed(int button) = 0;
        virtual glm::vec2 GetMousePosition() = 0;
        virtual float GetMouseX() = 0;
        virtual float GetMouseY() = 0;
    };
} // namespace LG
