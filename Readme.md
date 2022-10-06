# Legio Engine

## Introduction

Legio Engine is a small game engine created with the aim to learn graphics programming and how Game Engine systems interact with ecach other.
This engine is in very early development stages.

![Car Rendered in Legio Game Engine](https://github.com/LegioGroup/Legio-Engine/blob/main/data/LegioEngineCarRender.png)


## Instructions
---

To clone the repository alongside the engine use the following command

```
git clone -b premakeSupport --recurse-submodules -j8 https://github.com/LegioGroup/LegioGameApp.git
```

Once you've downloaded the project and the submodules run

```
python GetDeps.py
```

This will download premake and the Vulkan Installer

To configure for vulkan run Configure_Vulkan_VS2022.bat

To configure for OpenGL run Configure_Opengl_VS2022.bat

If you want to configure for VS2019 or any other visualStudio version just modify the previous scripts and change the VS2022 parameter to the desired Visual Studio version.

Currently the most stable branch is PremakeSupport which uses the OpenGlRender branch from the engine.

To open the repo with Visual Studio Code just go to the directory and type the following command
```
code .
```

The recommended extensions for Visual Studio Code are:

-  [cmake tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
-  [C/C++ for Visual Studio Code](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
-  [C/C++ Extension Pack](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack)
