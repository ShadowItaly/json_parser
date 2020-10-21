# json_parser
A c++ implementation of a throw free json_parser.

## Design goals
The parser must be throw free in order to use it on embedded devices. It should
parse a json superset, meaning it does not detect if the json is fully standard
compatible but it will parse any json that is. The error design is inspired from
Rust. The json supports `map_string()` and `map_bool` to call functions based on
the underlying type

## Compilation
The compiler must support the C++14 standard.

## Usage
```
#include "json_parser.hpp"

int main() {
  Json js;
  js.set("my_attribute","This is a string")
    .set("my_bool",true)
    .set("my_int",10)
    .error([](JsonError err){std::cout<<"An error happened!"<<std::endl;});
  
  return 0;
}
```
