mkdir build || \
rm build/*

i686-w64-mingw32-g++-win32 -static-libgcc -static-libstdc++ -shared \
-I "states_dll/include" -I "states_dll/include/mingw-std-threads" \
-I "states_dll/Detours/src" states_dll/src/* -o build/states.dll

cargo build --target=i686-pc-windows-gnu

cp target/i686-pc-windows-gnu/debug/*.dll build/Netplay.dll
