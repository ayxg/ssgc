# C& Programming Language Full Environment Package
A complete package with minimal external dependencies for creating and deploying C& programs and libraries.

The C& complier may be used directly from the command line to build and execute files or as a library included in your C++ code. A rudimentary IDE with a user interface is provided to ease the learning curve and enourage use of the language. Currently the IDE uses ImGui-SFML to generate the user interface - the end goal is to only depend on VulkanAPI and embed the IDE into a browser to allow writing C& program from the browser.

| **Includes** | **Description** |
|--|--|
| **CIDE**: C-Ide | GUI and command line for writing and building C&, and C++ programs. Currently depends on Imgui-SFML (Temporary GUI Solution).|
| **CAOCO**: C& Official Compiler | Official compiler and virtual machine implementation for the C& language runtime. Compiles and runs C& programs. |
| **CXX** : C++ Extended Standard Library | Common patterns and utilities as an extension to the standard library. Depends on **MTA**.|
| **MTA** : C++ Meta Template Archive | Variety of meta template methods and structures , for more ergonomic metaprogramming.|
| **MINT** : Minimal C++ Testing Framework | Used to unit test other project submodules.|

# The C& Programming Language
Pronunciation: “**candy**” or “**see-and**”.

First of, I will let the code do the talking. Have a taste of C&:

**Example#1: Unrelated animal types which all make sounds, displays use of dynamic types.**
 ![ex1](https://i.imgur.com/BJbjYaK.png)
 
**Example#2: The Husky which makes diffrent sounds based on it's mood, displays use of template metaprogramming.**
 ![ex2](https://imgur.com/N3yvqqq)

### Goals 
1. Quick and easy **deployment of a minimum viable product** with the **ability to scale and stay maintainable** by opting in to **type and logic safety** incrementally.
2. Low cognitive load on the programmer with **legible self-explanatory syntax** that is **familiar in logic to mainstream C-like languages**. (C/C++)    
3. **Plug and play** on top of any C++ environment and call native C++ methods from C&. **#include “candi.h” is all you need.**  

### Design Philosophies 
1. “When there is a right way, there should be one choice. When there is no right way, there should be as many choices as possible.” 
2. “If it can happen at compile time it should happen at compile time,  unless it must happen at run time.” 
