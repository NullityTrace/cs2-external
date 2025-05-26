#include <iostream>
#include <thread>
#include "src/overlay/overlay.h"
#include "src/hacks/reader.h"
#include "src/updater/auto_updater.h"

bool finish = false;
void read_thread() {
	while (!finish) {
		g_game.loop();
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
	}
}

int main()
{
	updater::read();
	g_game.init();



	std::thread read(read_thread);

	read.detach();
	overlay::render();
	return 0;
}

