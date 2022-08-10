cd states_dll/src/. && \
i686-w64-mingw32-g++-win32 -static-libgcc -static-libstdc++ -I "../include" -shared *.cpp ../lib/*.o -o states.dll && \
mv states.dll /home/kali/th155_beta/. && \
cd ../../ && \
cargo build --target=i686-pc-windows-gnu --release && \
cd target/i686-pc-windows-gnu/release/. && \
mv -f just_another_modloader.dll /home/kali/th155_beta/Netplay.dll && \
cd ../../../