## Development Installation

In order to compile `Alice` you will need to install a lot of dependencies... for real, a lot!

### command-line Build Tools

In order to compile c++ in powershell, you need to install visual studio's command-line Build Tools:

- Go to https://visualstudio.microsoft.com/downloads/
- expand `Tools for Visual Studio`
- Click on `Build Tools for Visual Studio`
- Install everything releated to Desktop and node

### Install Make

In order to compile SQLiteCPP you need make. Download and install it: http://gnuwin32.sourceforge.net/packages/make.htm. Also add it to your system env path

### Install OpenSSL

You need openSSL to handle cryptography, to install it do as follows:

- Download and install perl from: http://strawberryperl.com/ 
- Download the repo: https://github.com/openssl/openssl
- Open `Cross Tools Command Propmt`
- Go to the repo's dir
- Run `perl Configure no-asm VC-WIN64A`
- Run `nmake` and `nmake install`

### Build SQLite3 as .lib

You need to build `sqlite3.lib`. In order to do so:

- Download an amalgamation: https://www.sqlite.org/download.html
- Download a binary matching the same version (You will need the `.def`) 
- Open `Developer Command Prompt for VS`
- Run `lib /DEF:sqlite3.def /OUT:sqlite3.lib /MACHINE:x64`
- Reference the .lib into your vs solution

### Install deps

You need to have some compiled `.lib` in your computer. Open a powershell and type `.\install_deps.ps1`.
You also need to have `sqlite_modern_cpp` somewhere in your pc. use `git clone https://github.com/SqliteModernCpp/sqlite_modern_cpp.git`. Locate the folder `\hdr` inside the project, you need to include it into your vs solution.

### Include dirs and linked libs

You need to reference those installed dependencies in your vs solution:

example:

Include Dirs:
```
C:\Users\Example\Downloads\sqlite3
C:\Users\Example\Documents\Git\sqlite_modern_cpp\hdr
C:\Program Files (x86)\signal-protocol-c\include
C:\Program Files (x86)\spdlog\include
C:\Program Files (x86)\civetweb\include
/usr/include
/usr/local/include
```

Linked Libs:
```
C:\Users\Unable to decrypt\Downloads\sqlite3\sqlite3.lib
C:\Program Files\OpenSSL\lib\libcrypto.lib
C:\Program Files\OpenSSL\lib\libssl.lib
C:\Program Files (x86)\signal-protocol-c\lib\signal-protocol-c.lib
C:\Program Files (x86)\spdlog\lib\spdlog\spdlog.lib
C:\Program Files (x86)\civetweb\lib\civetweb.lib
```

Preprocessor Definitions: 
```
_CRT_SECURE_NO_WARNINGS
```

Also add `libcrypto-3.dll` generated when you compiled `OpenSSL` to your root solution. There's one already there but it's better if you replace it with the one you just compiled.