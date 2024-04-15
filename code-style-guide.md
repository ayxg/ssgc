# C& Core Project Guidelines
The code committed to this project must follow the Google C++ Style Guidelines unless the guideline conflicts with a C& Core Project Guideline described below. In this case, the C& Core Project Guideline will override the Google C++ Style Guideline. In case of any other conflicts, prefer to choose the easy-to-understand and clear solution which maintains functionality.

Reference: [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)

# Naming Conventions
| Object | Example  | Description
|--|--|--|
|**variable/member**| my_var | Lower case underscore.| 
|**private member**|  my_var_ | Lower case underscore followed by 1 underscore.| 
|**const variable**| kMyConstVar | Camel case preceded by a 'k'. |
|**global variable**| gMyGlobalVar | Camel case preceded by a 'k'. Prefer to encapsulate global variables in classes or static const accessor methods.|
|**lambda variable**| xMyFooLambda | Camel case preceded by an 'x'|
|**namespace**|  my_namespace | Lower case underscore.| 
|**method**| MyMethod() | Camel case |
|**consteval/full constexpr method**| kMyCvalMethod() | Camel case preceded by a 'k'. |
|**class**| MyMethod() | Camel case |
|**template class/method**| MyTemplate | Camel case.|
|**concept**| iMyConcept | Camel case preceded by an 'i'. |

# Class Definitions
- Place the most common public interface methods and members  at the top of the class definition. 
- Prefer to order methods and variables separately into logical groups closest to call site.
- Place all private members and methods at the bottom of the class definition.
- Define methods out-of-line if the class interface spans over a screen's height, otherwise prefer to define methods inline.
- Use 'private:' and 'public:' to separate methods into of classes with large interfaces into logical groups.
- Postfix virtual base classes' name with 'Base'
# Using the standard template library
When any std library is required include "cppsextended.h" which includes all the standard library headers which this project uses. The specific includes can be found in "import_stl.h". If a required include is not present add it to "import_stl.h". 
Once C++23 is fully supported by the major compilers, will use "import std;" instead.
