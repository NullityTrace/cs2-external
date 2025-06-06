#include <iostream>
#include <windows.h>
#include "src/overlay/overlay.h"
#include "src/updater/offsets.h"
#include "src/memory/reader.h"



bool finish = false;
void read_thread() {
    while (!finish) {
        reader.loop();
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}


int main() {
    bool updated = offsets::FetchOffsets();

    reader.init();

    std::thread read(read_thread);

    read.detach();

    overlay::render();
    return 0;
}