# Legio Engine

This engine is aimed to be a small project to test in order to learn some Graphics APIs.

## Screenshots

![Early build](https://raw.githubusercontent.com/LegioGroup/Legio-Engine/main/repoData/engine01.png)

## Instructions

To clone the repository alongside the engine use the following command

```
git clone --recurse-submodules -j8 https://github.com/LegioGroup/LegioGameApp.git
```

Once you've downloaded the project and the submodules run

```
python GetDeps.py
```

This will download premake and the Vulkan Installer

To configure for vulkan run Configure_Vulkan_VS2022.bat

To configure for OpenGL run Configure_Opengl_VS2022.bat

If you want to configure for VS2019 or any other visualStudio version just modify the previous scripts and change the VS2022 parameter to the desired Visual Studio version.

## VS CODE

To open the repo with Visual Studio Code just go to the directory and type the following command
```
code .
```

The recommended extensions for Visual Studio Code are:

-  [C/C++ for Visual Studio Code](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
-  [C/C++ Extension Pack](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack)
