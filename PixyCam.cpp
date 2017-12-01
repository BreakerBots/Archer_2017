#include <PixyCam.h>

long GetSystemTime(){
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}




PixyCam::PixyCam(unsigned int address):
	m_addr(address),
	m_bus(I2C::Port::kMXP, address),

	running(false),
	frame(-1),
	blocks()
	{}

PixyCam::ReadData(){
	unsigned char data[2];
	unsigned int prev_word = 0;
	unsigned int word;

	int blockNumber = -1;
	PixyFrame blocks();

	while (m_running){
		//Read 2 bytes form the pixycam bus
			// Pixy word = 256*data[1]+data[0]
		m_bus.ReadOnly(2, data);

		//Convert the 2 read bytes into a corresponding decimal integer
		word = 256*(unsigned int)(data[1]) + (unsigned int)(data[0]);


		if (0 == word){
			//Update the frame, it's collection of detected objects
			m_blocks = blocks;
			usleep(1000*20);
		} else if (START_BLOCK == word){
			blockNumber = 0;

			if (START_BLOCK == prev_word){
				//Update the frame, it's collection of detected objects
				m_blocks = blocks;
				m_frame++;

				blocks.clear();
			}
		} else if (blockNumber = 0){
			blocks.push_back(PixyObject());
			blocks.back().checksum = word;
			blockNumber++;
		} else if (blockNumber = 1){
			blocks.back().sig = word;
			blockNumber++;
		} else if (blockNumber = 2){
			blocks.back().x = word;
			blockNumber++;
		} else if (blockNumber = 3){
			blocks.back().y = word;
			blockNumber++;
		} else if (blockNumber = 4){
			blocks.back().w = word;
			blockNumber++;
		} else if (blockNumber = 5){
			blocks.back().h = word;
			blockNumber++;
		} else if (blockNumber = 6){
			std::cout << "Read an extra word from the pixy cam: " << word << std::endl;
		} else {
			std::cout << "Improperly parsing pixy cam output: " << word << std::endl;
		}

		prev_word = word;
	}//while
}//ReadData
