// Copyright (c) Matthew James Briggs

#pragma once

#include "ezjson/JDoc.h"
#include "ezjson/JValue.h"
#include <memory>

namespace ezjson
{
    class JFactory
    {
    public:
        static JDocPtr makeJDoc();
        static JValueUPtr makeJValue( JDocPtr& inJDoc );
    };
}
