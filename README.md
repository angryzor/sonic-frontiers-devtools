# Sonic Frontiers DevTools

This is an in-engine modding toolkit for the SEGA game Sonic Frontiers.
Releases can be found on GitHub or on the project's GameBanana page.
A HMM update server is also available.

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
