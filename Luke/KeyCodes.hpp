#ifndef LUKE_KEYCODES_HPP
#define LUKE_KEYCODES_HPP

#include <Stick/Platform.hpp>

namespace luke
{
// based on USB HID Usage Tables
STICK_API_ENUM_CLASS(KeyCode){ // Unknown
                               Unknown = -1,

                               // Letters
                               A = 4,
                               B = 5,
                               C = 6,
                               D = 7,
                               E = 8,
                               F = 9,
                               G = 10,
                               H = 11,
                               I = 12,
                               J = 13,
                               K = 14,
                               L = 15,
                               M = 16,
                               N = 17,
                               O = 18,
                               P = 19,
                               Q = 20,
                               R = 21,
                               S = 22,
                               T = 23,
                               U = 24,
                               V = 25,
                               W = 26,
                               X = 27,
                               Y = 28,
                               Z = 29,

                               // Numbers
                               One = 30,
                               Two = 31,
                               Three = 32,
                               Four = 33,
                               Five = 34,
                               Six = 35,
                               Seven = 36,
                               Eight = 37,
                               Nine = 38,
                               Zero = 39,

                               // Other
                               Return = 40, // Enter
                               Escape = 41,
                               Backspace = 42,
                               Tab = 43,
                               Space = 44,
                               Subtract = 45,
                               Equal = 46,
                               LeftBracket = 47,
                               RightBracket = 48,
                               Backslash = 49,
                               WorldOne = 50,
                               Semicolon = 51,
                               Apostrophe = 52,
                               GraveAccent = 53,
                               Comma = 54,
                               Period = 55,
                               Slash = 56,
                               CapsLock = 57,
                               F1 = 58,
                               F2 = 59,
                               F3 = 60,
                               F4 = 61,
                               F5 = 62,
                               F6 = 63,
                               F7 = 64,
                               F8 = 65,
                               F9 = 66,
                               F10 = 67,
                               F11 = 68,
                               F12 = 69,
                               F13 = 104,
                               F14 = 105,
                               F15 = 106,
                               F16 = 107,

                               Insert = 73,
                               Home = 74,
                               PageUp = 75,
                               Delete = 76,
                               End = 77,
                               PageDown = 78,

                               Caret = 100, // only on ISO keyboards

                               LeftControl = 224,
                               LeftShift = 225,
                               LeftAlt = 226,
                               LeftCommand = 227, // left windows key on windows,
                               RightControl = 228,
                               RightShift = 229,
                               RightAlt = 230,
                               RightCommand = 231, // right windows key on windows,

                               Right = 79,
                               Left = 80,
                               Down = 81,
                               Up = 82,

                               // special
                               Help = 117,
                               // ForwardDelete = 123,
                               Mute = 127,
                               VolumeUp = 128,
                               VolumeDown = 129,

                               // keypad
                               KPDivide = 84,
                               KPMultiply = 85,
                               KPSubtract = 86,
                               KPAdd = 87,
                               KPReturn = 88,
                               KPOne = 89,
                               KPTwo = 90,
                               KPThree = 91,
                               KPFour = 92,
                               KPFive = 93,
                               KPSix = 94,
                               KPSeven = 95,
                               KPEight = 96,
                               KPNine = 97,
                               KPZero = 98,
                               KPDecimal = 99,
                               KPEqual = 103,
                               KPComma = 133
};

STICK_API_ENUM_CLASS(KeyModifier){ None = 0,
                                   LeftShift = 1,
                                   RightShift = 1 << 1,
                                   LeftCommand = 1 << 2,
                                   RightCommand = 1 << 3,
                                   LeftControl = 1 << 4,
                                   RightControl = 1 << 5,
                                   LeftAlt = 1 << 6,
                                   RightAlt = 1 << 7,
                                   CapsLock = 1 << 8,
                                   Numbers = 1 << 9,
                                   Shift = (LeftShift | RightShift),
                                   Command = (LeftCommand | RightCommand),
                                   Control = (LeftControl | RightControl),
                                   Alt = (LeftAlt | RightAlt) };
} // namespace luke

#endif // LUKE_KEYCODES_HPP
