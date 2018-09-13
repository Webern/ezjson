// Copyright (c) Matthew James Briggs

#pragma once

#ifndef EZJ_FORWARD_DECLARE_ELEMENT
#define EZJ_FORWARD_DECLARE_ELEMENT \
namespace ezjson \
{ \
	class XElement; \
	using XElementPtr ## Ptr = std::shared_ptr<XElement>; \
} \

#endif
