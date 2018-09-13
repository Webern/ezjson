// Copyright (c) Matthew James Briggs

#pragma once

#include "ezjson/JDoc.h"
#include <memory>

namespace ezjson
{
    class XFactory
    {
    public:
        static JDocPtr makeJDoc();
        static JValue makeJValue( JDoc& inJDoc );
    };
}
