# Mnemen Engine : a fully fledged 3D game engine made by G.Tech students

## Cloning this repository

```powershell
git clone https://github.com/GamingCampus-MillieBourgois-24-25/engine-futaba --recursive
```
Make sure to use the --recursive flag, since this repository makes use of Git submodules! 

## How to build

It is imperative you install [xmake](https://xmake.io/#/getting_started) first via either the official website or PowerShell using this simple command:
```powershell
Invoke-Expression (Invoke-Webrequest 'https://xmake.io/psget.text' -UseBasicParsing).Content
```

Once you're setup, you're almost there! To build, just do
```powershell
xmake
```

To run the editor, you can use this command:
```powershell
xmake run Editor
```

To run the runtime, you can use this command:
```powershell
xmake run Runtime
```

## How to generate a Visual Studio solution

Use this simple command:
```
xmake project -k vsxmake -m "debug,release,releasedbg"
```

## Authors

- Amélie Heinrich, Lead Programmer (aheinrich@gaming.tech)
- Alexandre Mans, FX Programmer (amans@gaming.tech)
- Coralie Thiery, FX Programmer (cthiery@gaming.tech)
- Axel Moras, FX Programmer (amoras@gaming.tech)
- Jonathan Devaulchier (jdevaulchier@gaming.tech)
- Timéo Delmon (timeo.delmon@gmail.com)
- Bastian N'Diaye (bndiaye@gaming.tech)

## Dependencies

- SDL3
- spdlog
- GLM
