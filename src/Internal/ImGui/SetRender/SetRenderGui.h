#pragma once
#include <Windows.h>
#include<dxcapi.h>
#include<string>
#include <d3d12.h>
#include <dxgi1_6.h>
#include<wrl.h>
#include"Datamodel/BlendMode.h"



class SetRenderGui {

    void SetBlend();
    int blendMode_ = 0;
    const char* blendItems[kCountOfBlendMode] =
    {
        "None",
        "Normal",
        "Add",
        "Subtract",
        "Multiply",
        "Screen"
    };

    void SetRasterizer();
    int cullMode_ = 1;
    const char* cullItems[3]{
         "NONE",
        "FRONT",
        "BACK"
    };

    int fillMode_ = 1;
    const char* fillItems[2] = {
         "WIREFRAME",
        "SOLID"
    };

    void SetDepthStencil();
    int isEnable_ = 1;
    const char* flagTable[2] = {
         "false",
        "true",
    };

    int WriteMask_ = 1;
    const char* maskItem_[2] = {
        "Zero",
        "All"
    };

    int comparisonFunc_ = 4;
    const char* funcItem_[9]{
        "None",
        "Never",
        "Less",
        "Equal",
        "Less_Equal",
        "Greater",
        "NotEqual",
        "GreaterEqual",
        "Always"
    };


public:
    void Update();
};

