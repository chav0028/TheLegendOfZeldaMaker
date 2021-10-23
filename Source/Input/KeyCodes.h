//
//  KeyCodes.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-05-09.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef GameDev2D_KeyCodes_h
#define GameDev2D_KeyCodes_h

namespace GameDev2D
{
    #define KEY_CODE_COUNT 512

    //An enum of all the keys used, use these keycodes instead platform
    //specific key codes, these will work on all platforms.
    typedef enum
    {
		KEY_CODE_UNKNOWN	= 0,
		KEY_CODE_BACKSPACE	= 8,
		KEY_CODE_TAB		= 9,
		KEY_CODE_CLEAR		= 12,
		KEY_CODE_RETURN		= 13,
		KEY_CODE_PAUSE		= 19,
		KEY_CODE_ESCAPE		= 27,
        KEY_CODE_TILDE      = 28,
		KEY_CODE_SPACE		= 32,
		KEY_CODE_EXCLAIM	= 33,
		KEY_CODE_QUOTEDBL	= 34,
		KEY_CODE_HASH		= 35,
		KEY_CODE_DOLLAR		= 36,
		KEY_CODE_AMPERSAND	= 38,
		KEY_CODE_QUOTE		= 39,
		KEY_CODE_LEFTPAREN	= 40,
		KEY_CODE_RIGHTPAREN	= 41,
		KEY_CODE_ASTERISK	= 42,
		KEY_CODE_PLUS		= 43,
		KEY_CODE_COMMA		= 44,
		KEY_CODE_MINUS		= 45,
		KEY_CODE_PERIOD		= 46,
		KEY_CODE_SLASH		= 47,
		KEY_CODE_0			= 48,
		KEY_CODE_1			= 49,
		KEY_CODE_2			= 50,
		KEY_CODE_3			= 51,
		KEY_CODE_4			= 52,
		KEY_CODE_5			= 53,
		KEY_CODE_6			= 54,
		KEY_CODE_7			= 55,
		KEY_CODE_8			= 56,
		KEY_CODE_9			= 57,
		KEY_CODE_COLON		= 58,
		KEY_CODE_SEMICOLON	= 59,
		KEY_CODE_LESS		= 60,
		KEY_CODE_EQUALS		= 61,
		KEY_CODE_GREATER	= 62,
		KEY_CODE_QUESTION	= 63,
		KEY_CODE_AT			= 64,
    
        KEY_CODE_A			= 65,
		KEY_CODE_B			= 66,
		KEY_CODE_C			= 67,
		KEY_CODE_D			= 68,
		KEY_CODE_E			= 69,
		KEY_CODE_F			= 70,
		KEY_CODE_G			= 71,
		KEY_CODE_H			= 72,
		KEY_CODE_I			= 73,
		KEY_CODE_J			= 74,
		KEY_CODE_K			= 75,
		KEY_CODE_L			= 76,
		KEY_CODE_M			= 77,
		KEY_CODE_N			= 78,
		KEY_CODE_O			= 79,
		KEY_CODE_P			= 80,
		KEY_CODE_Q			= 81,
		KEY_CODE_R			= 82,
		KEY_CODE_S			= 83,
		KEY_CODE_T			= 84,
		KEY_CODE_U			= 85,
		KEY_CODE_V			= 86,
		KEY_CODE_W			= 87,
		KEY_CODE_X			= 88,
		KEY_CODE_Y			= 89,
		KEY_CODE_Z			= 90,
    
		KEY_CODE_LEFT_BRACKET  = 91,
		KEY_CODE_BACKSLASH    = 92,
		KEY_CODE_RIGHT_BRACKET = 93,
		KEY_CODE_CARET		= 94,
		KEY_CODE_UNDERSCORE	= 95,
		KEY_CODE_BACKQUOTE	= 96,
		KEY_CODE_a			= 97,
		KEY_CODE_b			= 98,
		KEY_CODE_c			= 99,
		KEY_CODE_d			= 100,
		KEY_CODE_e			= 101,
		KEY_CODE_f			= 102,
		KEY_CODE_g			= 103,
		KEY_CODE_h			= 104,
		KEY_CODE_i			= 105,
		KEY_CODE_j			= 106,
		KEY_CODE_k			= 107,
		KEY_CODE_l			= 108,
		KEY_CODE_m			= 109,
		KEY_CODE_n			= 110,
		KEY_CODE_o			= 111,
		KEY_CODE_p			= 112,
		KEY_CODE_q			= 113,
		KEY_CODE_r			= 114,
		KEY_CODE_s			= 115,
		KEY_CODE_t			= 116,
		KEY_CODE_u			= 117,
		KEY_CODE_v			= 118,
		KEY_CODE_w			= 119,
		KEY_CODE_x			= 120,
		KEY_CODE_y			= 121,
		KEY_CODE_z			= 122,
		KEY_CODE_DELETE		= 127,

		//Numeric keypad
		KEY_CODE_KP0		= 256,
		KEY_CODE_KP1		= 257,
		KEY_CODE_KP2		= 258,
		KEY_CODE_KP3		= 259,
		KEY_CODE_KP4		= 260,
		KEY_CODE_KP5		= 261,
		KEY_CODE_KP6		= 262,
		KEY_CODE_KP7		= 263,
		KEY_CODE_KP8		= 264,
		KEY_CODE_KP9		= 265,
		KEY_CODE_KP_PERIOD	= 266,
		KEY_CODE_KP_DIVIDE		= 267,
		KEY_CODE_KP_MULTIPLY	= 268,
		KEY_CODE_KP_MINUS		= 269,
		KEY_CODE_KP_PLUS		= 270,
		KEY_CODE_KP_ENTER		= 271,
		KEY_CODE_KP_EQUALS		= 272,
		
		//Arrows + Home/End pad
		KEY_CODE_UP			= 273,
		KEY_CODE_DOWN		= 274,
		KEY_CODE_RIGHT		= 275,
		KEY_CODE_LEFT		= 276,
		KEY_CODE_INSERT		= 277,
		KEY_CODE_HOME		= 278,
		KEY_CODE_END		= 279,
		KEY_CODE_PAGEUP		= 280,
		KEY_CODE_PAGEDOWN	= 281,
		
		//Function keys
		KEY_CODE_F1			= 282,
		KEY_CODE_F2			= 283,
		KEY_CODE_F3			= 284,
		KEY_CODE_F4			= 285,
		KEY_CODE_F5			= 286,
		KEY_CODE_F6			= 287,
		KEY_CODE_F7			= 288,
		KEY_CODE_F8			= 289,
		KEY_CODE_F9			= 290,
		KEY_CODE_F10		= 291,
		KEY_CODE_F11		= 292,
		KEY_CODE_F12		= 293,
		KEY_CODE_F13		= 294,
		KEY_CODE_F14		= 295,
		KEY_CODE_F15		= 296,
		
		//Key state modifier keys
		KEY_CODE_NUMLOCK	    = 300,
		KEY_CODE_CAPSLOCK	    = 301,
		KEY_CODE_SCROLLOCK	    = 302,
		KEY_CODE_RIGHT_SHIFT    = 303,
		KEY_CODE_LEFT_SHIFT	    = 304,
		KEY_CODE_RIGHT_CTRL	    = 305,
		KEY_CODE_LEFT_CTRL	    = 306,
		KEY_CODE_RIGHT_ALT	    = 307,
		KEY_CODE_LEFT_ALT	    = 308,
		KEY_CODE_RIGHT_META	    = 309,
		KEY_CODE_LEFT_META	    = 310,
		KEY_CODE_LEFT_COMMAND   = 311,		//Left "Windows" key
		KEY_CODE_RIGHT_COMMAND  = 312,		//Right "Windows" key
		KEY_CODE_MODE		    = 313,		//"Alt Gr" key */
		KEY_CODE_FUNCTION	    = 314,		// Multi-key compose key
	}KeyCode;
    
    //Conveniance method to convert a keycode into a string
    inline const char* KeyCodeString(KeyCode keyCode)
    {
        switch (keyCode)
        {
            case KEY_CODE_BACKSPACE:
                return "Backspace";
            case KEY_CODE_TAB:
                return "Tab";
            case KEY_CODE_CLEAR:
                return "Clear";
            case KEY_CODE_RETURN:
                return "Return";
            case KEY_CODE_PAUSE:
                return "Pause";
            case KEY_CODE_ESCAPE:
                return "Escape";
            case KEY_CODE_TILDE:
                return "Tilde";
            case KEY_CODE_SPACE:
                return "Space";
            case KEY_CODE_EXCLAIM:
                return "!";
            case KEY_CODE_QUOTEDBL:
                return "\"";
            case KEY_CODE_HASH:
                return "#";
            case KEY_CODE_DOLLAR:
                return "$";
            case KEY_CODE_QUOTE:
                return "'";
            case KEY_CODE_LEFTPAREN:
                return "(";
            case KEY_CODE_RIGHTPAREN:
                return ")";
            case KEY_CODE_ASTERISK:
                return "*";
            case KEY_CODE_PLUS:
                return "+";
            case KEY_CODE_COMMA:
                return ",";
            case KEY_CODE_MINUS:
                return "-";
            case KEY_CODE_PERIOD:
                return ".";
            case KEY_CODE_SLASH:
                return "/";
            case KEY_CODE_0:
                return "0";
            case KEY_CODE_1:
                return "1";
            case KEY_CODE_2:
                return "2";
            case KEY_CODE_3:
                return "3";
            case KEY_CODE_4:
                return "4";
            case KEY_CODE_5:
                return "5";
            case KEY_CODE_6:
                return "6";
            case KEY_CODE_7:
                return "7";
            case KEY_CODE_8:
                return "8";
            case KEY_CODE_9:
                return "9";
            case KEY_CODE_COLON:
                return ":";
            case KEY_CODE_SEMICOLON:
                return ";";
            case KEY_CODE_LESS:
                return "<";
            case KEY_CODE_EQUALS:
                return "=";
            case KEY_CODE_GREATER:
                return ">";
            case KEY_CODE_QUESTION:
                return "?";
            case KEY_CODE_AT:
                return "@";
            case KEY_CODE_A:
                return "A";
            case KEY_CODE_B:
                return "B";
            case KEY_CODE_C:
                return "C";
            case KEY_CODE_D:
                return "D";
            case KEY_CODE_E:
                return "E";
            case KEY_CODE_F:
                return "F";
            case KEY_CODE_G:
                return "G";
            case KEY_CODE_H:
                return "H";
            case KEY_CODE_I:
                return "I";
            case KEY_CODE_J:
                return "J";
            case KEY_CODE_K:
                return "K";
            case KEY_CODE_L:
                return "L";
            case KEY_CODE_M:
                return "M";
            case KEY_CODE_N:
                return "N";
            case KEY_CODE_O:
                return "O";
            case KEY_CODE_P:
                return "P";
            case KEY_CODE_Q:
                return "Q";
            case KEY_CODE_R:
                return "R";
            case KEY_CODE_S:
                return "S";
            case KEY_CODE_T:
                return "T";
            case KEY_CODE_U:
                return "U";
            case KEY_CODE_V:
                return "V";
            case KEY_CODE_W:
                return "W";
            case KEY_CODE_X:
                return "X";
            case KEY_CODE_Y:
                return "Y";
            case KEY_CODE_Z:
                return "Z";
            case KEY_CODE_LEFT_BRACKET:
                return "[";
            case KEY_CODE_BACKSLASH:
                return "\\";
            case KEY_CODE_RIGHT_BRACKET:
                return "]";
            case KEY_CODE_CARET:
                return "^";
            case KEY_CODE_UNDERSCORE:
                return "_";
            case KEY_CODE_BACKQUOTE:
                return "`";
            case KEY_CODE_a:
                return "a";
            case KEY_CODE_b:
                return "b";
            case KEY_CODE_c:
                return "c";
            case KEY_CODE_d:
                return "d";
            case KEY_CODE_e:
                return "e";
            case KEY_CODE_f:
                return "f";
            case KEY_CODE_g:
                return "g";
            case KEY_CODE_h:
                return "h";
            case KEY_CODE_i:
                return "i";
            case KEY_CODE_j:
                return "j";
            case KEY_CODE_k:
                return "k";
            case KEY_CODE_l:
                return "l";
            case KEY_CODE_m:
                return "m";
            case KEY_CODE_n:
                return "n";
            case KEY_CODE_o:
                return "o";
            case KEY_CODE_p:
                return "p";
            case KEY_CODE_q:
                return "q";
            case KEY_CODE_r:
                return "r";
            case KEY_CODE_s:
                return "s";
            case KEY_CODE_t:
                return "t";
            case KEY_CODE_u:
                return "u";
            case KEY_CODE_v:
                return "v";
            case KEY_CODE_w:
                return "w";
            case KEY_CODE_x:
                return "x";
            case KEY_CODE_y:
                return "y";
            case KEY_CODE_z:
                return "z";
            case KEY_CODE_DELETE:
                return "Delete";
            case KEY_CODE_KP0:
                return "0";
            case KEY_CODE_KP1:
                return "1";
            case KEY_CODE_KP2:
                return "2";
            case KEY_CODE_KP3:
                return "3";
            case KEY_CODE_KP4:
                return "4";
            case KEY_CODE_KP5:
                return "5";
            case KEY_CODE_KP6:
                return "6";
            case KEY_CODE_KP7:
                return "7";
            case KEY_CODE_KP8:
                return "8";
            case KEY_CODE_KP9:
                return "9";
            case KEY_CODE_KP_PERIOD:
                return ".";
            case KEY_CODE_KP_DIVIDE:
                return "/";
            case KEY_CODE_KP_MULTIPLY:
                return "*";
            case KEY_CODE_KP_MINUS:
                return "-";
            case KEY_CODE_KP_PLUS:
                return "+";
            case KEY_CODE_KP_ENTER:
                return "Enter";
            case KEY_CODE_KP_EQUALS:
                return "=";
            case KEY_CODE_UP:
                return "Up";
            case KEY_CODE_DOWN:
                return "Down";
            case KEY_CODE_RIGHT:
                return "Right";
            case KEY_CODE_LEFT:
                return "Left";
            case KEY_CODE_INSERT:
                return "Insert";
            case KEY_CODE_HOME:
                return "Home";
            case KEY_CODE_END:
                return "End";
            case KEY_CODE_PAGEUP:
                return "Page Up";
            case KEY_CODE_PAGEDOWN:
                return "Page Down";
            case KEY_CODE_F1:
                return "F1";
            case KEY_CODE_F2:
                return "F2";
            case KEY_CODE_F3:
                return "F3";
            case KEY_CODE_F4:
                return "F4";
            case KEY_CODE_F5:
                return "F5";
            case KEY_CODE_F6:
                return "F6";
            case KEY_CODE_F7:
                return "F7";
            case KEY_CODE_F8:
                return "F8";
            case KEY_CODE_F9:
                return "F9";
            case KEY_CODE_F10:
                return "F10";
            case KEY_CODE_F11:
                return "F11";
            case KEY_CODE_F12:
                return "F12";
            case KEY_CODE_F13:
                return "F13";
            case KEY_CODE_F14:
                return "F14";
            case KEY_CODE_F15:
                return "F15";
            case KEY_CODE_NUMLOCK:
                return "Num Lock";
            case KEY_CODE_CAPSLOCK:
                return "Caps Lock";
            case KEY_CODE_SCROLLOCK:
                return "Scroll Lock";
            case KEY_CODE_RIGHT_SHIFT:
                return "Right Shift";
            case KEY_CODE_LEFT_SHIFT:
                return "Left Shift";
            case KEY_CODE_RIGHT_CTRL:
                return "Right Control";
            case KEY_CODE_LEFT_CTRL:
                return "Left Control";
            case KEY_CODE_RIGHT_ALT:
                return "Right Alt";
            case KEY_CODE_LEFT_ALT:
                return "Left Alt";
            case KEY_CODE_RIGHT_META:
                return "Left Meta";
            case KEY_CODE_LEFT_META:
                return "Left Meta";
            case KEY_CODE_LEFT_COMMAND:
                return "Left Command";
            case KEY_CODE_RIGHT_COMMAND:
                return "Right Command";
            case KEY_CODE_MODE:
                return "Mode";
            case KEY_CODE_FUNCTION:
                return "Function";
            case KEY_CODE_UNKNOWN:
            default:
                return "Unknown";
        }
        
        return "";
    }
}

#endif
