//
//  DebugUI.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-26.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__DebugUI__
#define __GameDev2D__DebugUI__

#include "../GameService.h"
#include "../../Events/EventHandler.h"
#include "../../Graphics/Core/Color.h"


using namespace std;
using namespace glm;

namespace GameDev2D
{
    //Defines
    #define CastDebugCallbackUnsignedInt(functionPointer)   static_cast<DebugUI::DebugCallbackUnsignedInt>(&functionPointer)
    #define CastDebugCallbackInt(functionPointer)           static_cast<DebugUI::DebugCallbackInt>(&functionPointer)
    #define CastDebugCallbackFloat(functionPointer)         static_cast<DebugUI::DebugCallbackFloat>(&functionPointer)
    #define CastDebugCallbackDouble(functionPointer)        static_cast<DebugUI::DebugCallbackDouble>(&functionPointer)
    #define CastDebugCallbackBool(functionPointer)          static_cast<DebugUI::DebugCallbackBool>(&functionPointer)
    #define CastDebugCallbackString(functionPointer)        static_cast<DebugUI::DebugCallbackString>(&functionPointer)
    #define CastDebugCallbackVec2(functionPointer)          static_cast<DebugUI::DebugCallbackVec2>(&functionPointer)
    #define CastDebugCallbackDVec2(functionPointer)         static_cast<DebugUI::DebugCallbackDVec2>(&functionPointer)
    #define CastDebugCallbackVec3(functionPointer)          static_cast<DebugUI::DebugCallbackVec3>(&functionPointer)
    #define CastDebugCallbackDVec3(functionPointer)         static_cast<DebugUI::DebugCallbackDVec3>(&functionPointer)

    //Constants
    const unsigned int DEBUG_FLOAT_DOUBLE_PRECISION = 3;
    const unsigned int DEBUG_LABEL_FONT_SIZE = 32;
    const float DEBUG_LABEL_SPACING = 5.0f;

    //Forward Declarations
    class Label;

    //DebugUI class, will display the Game's FPS and other relevant information on screen. You may
    //also register a function pointer for most datatype and it will display it on screen as well
    class DebugUI : public GameService
    {
    public:
        DebugUI();
        ~DebugUI();
        
        void Update(double delta);
        void Draw();
        
        //Pure virtual method in GameService, needed to determine
        //if we should update and draw this GameService
        bool CanUpdate();
        bool CanDraw();
        
        //Event Handler method
        void HandleEvent(Event* event);
        
        //Function pointer definitions
        typedef unsigned int (BaseObject::*DebugCallbackUnsignedInt)();
        typedef int (BaseObject::*DebugCallbackInt)();
        typedef float (BaseObject::*DebugCallbackFloat)();
        typedef double (BaseObject::*DebugCallbackDouble)();
        typedef bool (BaseObject::*DebugCallbackBool)();
        typedef void (BaseObject::*DebugCallbackString)(string& value);
        typedef vec2 (BaseObject::*DebugCallbackVec2)();
        typedef dvec2 (BaseObject::*DebugCallbackDVec2)();
        typedef vec3 (BaseObject::*DebugCallbackVec3)();
        typedef dvec3 (BaseObject::*DebugCallbackDVec3)();
        
        //Methods to watch a value
        void WatchValueUnsignedInt(BaseObject* callbackObject, DebugCallbackUnsignedInt callbackMethodUnsignedInt);
        void WatchValueInt(BaseObject* callbackObject, DebugCallbackInt callbackMethodInt);
        void WatchValueFloat(BaseObject* callbackObject, DebugCallbackFloat callbackMethodFloat);
        void WatchValueDouble(BaseObject* callbackObject, DebugCallbackDouble callbackMethodDouble);
        void WatchValueBool(BaseObject* callbackObject, DebugCallbackBool callbackMethodBool);
        void WatchValueString(BaseObject* callbackObject, DebugCallbackString callbackMethodString);
        void WatchValueVec2(BaseObject* callbackObject, DebugCallbackVec2 callbackMethodVec2);
        void WatchValueDVec2(BaseObject* callbackObject, DebugCallbackDVec2 callbackMethodDVec2);
        void WatchValueVec3(BaseObject* callbackObject, DebugCallbackVec3 callbackMethodVec3);
        void WatchValueDVec3(BaseObject* callbackObject, DebugCallbackDVec3 callbackMethodDVec3);
        
        //Methods to stop watching a value
        void StopWatchingValueUnsignedInt(BaseObject* callbackObject, DebugCallbackUnsignedInt callbackMethodUnsignedInt);
        void StopWatchingValueInt(BaseObject* callbackObject, DebugCallbackInt callbackMethodInt);
        void StopWatchingValueFloat(BaseObject* callbackObject, DebugCallbackFloat callbackMethodFloat);
        void StopWatchingValueDouble(BaseObject* callbackObject, DebugCallbackDouble callbackMethodDouble);
        void StopWatchingValueBool(BaseObject* callbackObject, DebugCallbackBool callbackMethodBool);
        void StopWatchingValueString(BaseObject* callbackObject, DebugCallbackString callbackMethodString);
        void StopWatchingValueVec2(BaseObject* callbackObject, DebugCallbackVec2 callbackMethodVec2);
        void StopWatchingValueDVec2(BaseObject* callbackObject, DebugCallbackDVec2 callbackMethodDVec2);
        void StopWatchingValueVec3(BaseObject* callbackObject, DebugCallbackVec3 callbackMethodVec3);
        void StopWatchingValueDVec3(BaseObject* callbackObject, DebugCallbackDVec3 callbackMethodDVec3);
        
        //Set the UI label color
        void SetLabelColor(Color color);
        
    private:
        //Enum to keep track of type of function pointer the debug data struct holds
        enum DebugCallbackType
        {
            DebugCallbackTypeUnsignedInt = 0,
            DebugCallbackTypeInt,
            DebugCallbackTypeFloat,
            DebugCallbackTypeDouble,
            DebugCallbackTypeBool,
            DebugCallbackTypeString,
            DebugCallbackTypeVec2,
            DebugCallbackTypeDVec2,
            DebugCallbackTypeVec3,
            DebugCallbackTypeDVec3
        };
    
        //DebugData struct to help manage the function pointers for each data type and Label object
        struct DebugData
        {
            Label* label;
            BaseObject* callbackObject;
            DebugCallbackUnsignedInt callbackMethodUnsignedInt;
            DebugCallbackInt callbackMethodInt;
            DebugCallbackFloat callbackMethodFloat;
            DebugCallbackDouble callbackMethodDouble;
            DebugCallbackBool callbackMethodBool;
            DebugCallbackString callbackMethodString;
            DebugCallbackVec2 callbackMethodVec2;
            DebugCallbackDVec2 callbackMethodDVec2;
            DebugCallbackVec3 callbackMethodVec3;
            DebugCallbackDVec3 callbackMethodDVec3;
            DebugCallbackType callbackType;
            
            DebugData(Label* aLabel, BaseObject* aCallbackObject, DebugCallbackUnsignedInt aCallbackMethodUnsignedInt)
            {
                label = aLabel;
                callbackObject = aCallbackObject;
                callbackMethodUnsignedInt = aCallbackMethodUnsignedInt;
                callbackMethodInt = nullptr;
                callbackMethodFloat = nullptr;
                callbackMethodDouble = nullptr;
                callbackMethodBool = nullptr;
                callbackMethodString = nullptr;
                callbackMethodVec2 = nullptr;
                callbackMethodDVec2 = nullptr;
                callbackMethodVec3 = nullptr;
                callbackMethodDVec3 = nullptr;
                callbackType = DebugCallbackTypeUnsignedInt;
            }
            
            DebugData(Label* aLabel, BaseObject* aCallbackObject, DebugCallbackInt aCallbackMethodInt)
            {
                label = aLabel;
                callbackObject = aCallbackObject;
                callbackMethodUnsignedInt = nullptr;
                callbackMethodInt = aCallbackMethodInt;
                callbackMethodFloat = nullptr;
                callbackMethodDouble = nullptr;
                callbackMethodBool = nullptr;
                callbackMethodString = nullptr;
                callbackMethodVec2 = nullptr;
                callbackMethodDVec2 = nullptr;
                callbackMethodVec3 = nullptr;
                callbackMethodDVec3 = nullptr;
                callbackType = DebugCallbackTypeInt;
            }
            
            DebugData(Label* aLabel, BaseObject* aCallbackObject, DebugCallbackFloat aCallbackMethodFloat)
            {
                label = aLabel;
                callbackObject = aCallbackObject;
                callbackMethodUnsignedInt = nullptr;
                callbackMethodInt = nullptr;
                callbackMethodFloat = aCallbackMethodFloat;
                callbackMethodDouble = nullptr;
                callbackMethodBool = nullptr;
                callbackMethodString = nullptr;
                callbackMethodVec2 = nullptr;
                callbackMethodDVec2 = nullptr;
                callbackMethodVec3 = nullptr;
                callbackMethodDVec3 = nullptr;
                callbackType = DebugCallbackTypeFloat;
            }
            
            DebugData(Label* aLabel, BaseObject* aCallbackObject, DebugCallbackDouble aCallbackMethodDouble)
            {
                label = aLabel;
                callbackObject = aCallbackObject;
                callbackMethodUnsignedInt = nullptr;
                callbackMethodInt = nullptr;
                callbackMethodFloat = nullptr;
                callbackMethodDouble = aCallbackMethodDouble;
                callbackMethodBool = nullptr;
                callbackMethodString = nullptr;
                callbackMethodVec2 = nullptr;
                callbackMethodDVec2 = nullptr;
                callbackMethodVec3 = nullptr;
                callbackMethodDVec3 = nullptr;
                callbackType = DebugCallbackTypeDouble;
            }
            
            DebugData(Label* aLabel, BaseObject* aCallbackObject, DebugCallbackBool aCallbackMethodBool)
            {
                label = aLabel;
                callbackObject = aCallbackObject;
                callbackMethodUnsignedInt = nullptr;
                callbackMethodInt = nullptr;
                callbackMethodFloat = nullptr;
                callbackMethodDouble = nullptr;
                callbackMethodBool = aCallbackMethodBool;
                callbackMethodString = nullptr;
                callbackMethodVec2 = nullptr;
                callbackMethodDVec2 = nullptr;
                callbackMethodVec3 = nullptr;
                callbackMethodDVec3 = nullptr;
                callbackType = DebugCallbackTypeBool;
            }
            
            DebugData(Label* aLabel, BaseObject* aCallbackObject, DebugCallbackString aCallbackMethodString)
            {
                label = aLabel;
                callbackObject = aCallbackObject;
                callbackMethodUnsignedInt = nullptr;
                callbackMethodInt = nullptr;
                callbackMethodFloat = nullptr;
                callbackMethodDouble = nullptr;
                callbackMethodBool = nullptr;
                callbackMethodString = aCallbackMethodString;
                callbackMethodVec2 = nullptr;
                callbackMethodDVec2 = nullptr;
                callbackMethodVec3 = nullptr;
                callbackMethodDVec3 = nullptr;
                callbackType = DebugCallbackTypeString;
            }
            
            DebugData(Label* aLabel, BaseObject* aCallbackObject, DebugCallbackVec2 aCallbackMethodVec2)
            {
                label = aLabel;
                callbackObject = aCallbackObject;
                callbackMethodUnsignedInt = nullptr;
                callbackMethodInt = nullptr;
                callbackMethodFloat = nullptr;
                callbackMethodDouble = nullptr;
                callbackMethodBool = nullptr;
                callbackMethodString = nullptr;
                callbackMethodVec2 = aCallbackMethodVec2;
                callbackMethodDVec2 = nullptr;
                callbackMethodVec3 = nullptr;
                callbackMethodDVec3 = nullptr;
                callbackType = DebugCallbackTypeVec2;
            }
            
            DebugData(Label* aLabel, BaseObject* aCallbackObject, DebugCallbackDVec2 aCallbackMethodDVec2)
            {
                label = aLabel;
                callbackObject = aCallbackObject;
                callbackMethodUnsignedInt = nullptr;
                callbackMethodInt = nullptr;
                callbackMethodFloat = nullptr;
                callbackMethodDouble = nullptr;
                callbackMethodBool = nullptr;
                callbackMethodString = nullptr;
                callbackMethodVec2 = nullptr;
                callbackMethodDVec2 = aCallbackMethodDVec2;
                callbackMethodVec3 = nullptr;
                callbackMethodDVec3 = nullptr;
                callbackType = DebugCallbackTypeDVec2;
            }
            
            DebugData(Label* aLabel, BaseObject* aCallbackObject, DebugCallbackVec3 aCallbackMethodVec3)
            {
                label = aLabel;
                callbackObject = aCallbackObject;
                callbackMethodUnsignedInt = nullptr;
                callbackMethodInt = nullptr;
                callbackMethodFloat = nullptr;
                callbackMethodDouble = nullptr;
                callbackMethodBool = nullptr;
                callbackMethodString = nullptr;
                callbackMethodVec2 = nullptr;
                callbackMethodDVec2 = nullptr;
                callbackMethodVec3 = aCallbackMethodVec3;
                callbackMethodDVec3 = nullptr;
                callbackType = DebugCallbackTypeVec3;
            }
            
            DebugData(Label* aLabel, BaseObject* aCallbackObject, DebugCallbackDVec3 aCallbackMethodDVec3)
            {
                label = aLabel;
                callbackObject = aCallbackObject;
                callbackMethodUnsignedInt = nullptr;
                callbackMethodInt = nullptr;
                callbackMethodFloat = nullptr;
                callbackMethodDouble = nullptr;
                callbackMethodBool = nullptr;
                callbackMethodString = nullptr;
                callbackMethodVec2 = nullptr;
                callbackMethodDVec2 = nullptr;
                callbackMethodVec3 = nullptr;
                callbackMethodDVec3 = aCallbackMethodDVec3;
                callbackType = DebugCallbackTypeDVec3;
            }
        };
        
        //Private methods
        void UpdateLabel(double delta, Label* label, BaseObject* callbackObject, DebugCallbackUnsignedInt callbackMethodUnsignedInt);
        void UpdateLabel(double delta, Label* label, BaseObject* callbackObject, DebugCallbackInt callbackMethodInt);
        void UpdateLabel(double delta, Label* label, BaseObject* callbackObject, DebugCallbackFloat callbackMethodFloat);
        void UpdateLabel(double delta, Label* label, BaseObject* callbackObject, DebugCallbackDouble callbackMethodDouble);
        void UpdateLabel(double delta, Label* label, BaseObject* callbackObject, DebugCallbackBool callbackMethodBool);
        void UpdateLabel(double delta, Label* label, BaseObject* callbackObject, DebugCallbackString callbackMethodString);
        void UpdateLabel(double delta, Label* label, BaseObject* callbackObject, DebugCallbackVec2 callbackMethodVec2);
        void UpdateLabel(double delta, Label* label, BaseObject* callbackObject, DebugCallbackDVec2 callbackMethodDVec2);
        void UpdateLabel(double delta, Label* label, BaseObject* callbackObject, DebugCallbackVec3 callbackMethodVec3);
        void UpdateLabel(double delta, Label* label, BaseObject* callbackObject, DebugCallbackDVec3 callbackMethodDVec3);
        void UpdateLayout();
        
        //Conveniance method to create a debug label
        Label* CreateDebugLabel();
        
        //Method to erase a label at an given index
        void EraseLabelAtIndex(int index);
    
        //Member variables
        vector<DebugData> m_Labels;
        Color m_FontColor;
    };
}

#endif /* defined(__GameDev2D__DebugUI__) */
