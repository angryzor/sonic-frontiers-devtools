# Sonic Frontiers DevTools

This is an in-engine modding toolkit for the SEGA game Sonic Frontiers.
Releases can be found on [https://github.com/angryzor/sonic-frontiers-devtools/releases](GitHub)
or on [https://gamebanana.com/tools/15218](the project's GameBanana page).
A HMM update server is also available.

![The level editor](/.github/editor.png)

## Setting up the development environment

You will need to have the following prerequisites installed:

* Visual Studio 2022
* CMake 3.28 or higher

Check out the project and make sure to also check out its submodules:

```sh
git clone --recurse-submodules https://github.com/angryzor/sonic-frontiers-devtools.git
```

Now let CMake do its thing:

```sh
cmake -A x64 -B build
```

If you have Sonic Frontiers installed in a non-standard location, you can specify that location
with the `DEVTOOLS_GAME_FOLDER` variable:

```sh
cmake -A x64 -B build -DDEVTOOLS_GAME_FOLDER="C:\ShadowFrontiers"
```

Once CMake is finished, navigate to the build directory and open `devtools.sln` with VS2022.
You should have a fully working environment available.

Building the INSTALL project will install the mod into HedgeModManager's `Mods` directory.

## Screenshots

DevTools has a builtin level editor complete with rendering of debug visualizations such as volumes and colliders:

![The level editor](/.github/editor.png)

As well as debug information left in the game's binary:

![Rendering of debug visuals](/.github/debugv.jpg)

DevTools is able to manipulate datastructures and resources inside the game's memory through reflection:

![RFL editing](/.github/rfl.jpg)

DevTools has a resource browser where you can see that game's currently loaded resources, edit them and hot reload them while the game is running:

![Resource browser](/.github/resource-browser.jpg)

DevTools can edit NeedleFXSceneData graphics parameters in realtime, giving you instant feedback when modding postprocessing and lighting parameters:

![NeedleFxSceneData editing](/.github/lighting.jpg)

DevTools provides inspectors for a variety of game systems, such as the memory allocation system:

![The memory inspector](/.github/memory-inspector.png)
