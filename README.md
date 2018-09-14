ezjson
======

`ezjson` presents a simple interface to json documents and strings.

Example

```
#include "ezjson/ezjson.h"

auto jdoc = ezjson::JFactory::makeJDoc();
jdoc->loadFile( "path/to/some/file.json" );

if( jdoc->getRoot()->getType() == JValueType::array )
{
    std::cout << "array" << std::endl;
}
else if( jdoc->getRoot()->getType() == JValueType::object )
{
    std::cout << "object" << std::endl;
}

```