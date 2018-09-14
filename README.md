ezjson
======

`ezjson` presents a simple interface to json documents and strings.

### Example

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

### Why?

Dunno. Just wanted a simple interface. Rapidjson is used under the hood for parsing, but the entire document is read into memory.

### TODO

* Implement shared strings and other tricks to save memory space.
* A lot more testing.
* Continuous Integration.