mkdir -p build
gcc -shared -fPIC -O5 effects.c -lm -o build/effects.so
