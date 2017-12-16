#include <PixyCam.h>

#include "I2C.h"
#include <chrono>
#include <thread>
#include <iostream>

long GetSystemTime(){
	return std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch()).count();
}

PixyCam::PixyCam(int address):
		m_addr(address),
		m_bus(new I2C(I2C::Port::kMXP, address)),
		m_thread(0),

		m_running(false),
		debug(""),
		m_frame(-1),
		m_blocks()
	{
	}

int PixyCam::frameCount(){
	return m_frame;
}//frameCount

PixyFrame PixyCam::objects(){
	return m_blocks;
}


void PixyCam::startThread (PixyCam& pixy){
	pixy.ReadData();
}

void PixyCam::Start (){
	if (!m_thread){
		m_running = true;
		m_thread = new std::thread(startThread, std::ref(*this));
	} else {
		std::cout << "PixyCam: Tried to start with active thread" << std::endl;
	}
}
void PixyCam::Stop (){
	if (m_thread){
		m_running = false;
		std::cout << "PixyCam: Waiting on read thread to stop" << std::endl;
		m_thread->join();
		m_thread = 0;
	} else {
		std::cout << "PixyCam: No thread to stop" << std::endl;
	}
}

void PixyCam::ReadData(){
	unsigned char data[2];
	unsigned int prev_word = 0;
	unsigned int word;

	int blockNumber = -1;
	PixyFrame blocks = PixyFrame();

	m_running = true;

	std::cout << "Starting Read Loop" << std::endl;

	while (true){
		//Read 2 bytes form the pixycam bus
			// Pixy word = 256*data[1]+data[0]
		m_bus->ReadOnly(2, data);

		//Convert the 2 read bytes into a corresponding decimal integer
		word = 256*(unsigned int)(data[1]) + (unsigned int)(data[0]);

//		if (m_frame < 10)
//			debug += "Frame: " + std::to_string(m_frame) + "  Word: " + std::to_string(word) + '\n';
////			std::cout << GetSystemTime() << "Frame: " << m_frame << "  Word: " << word << std::endl;
//		if (m_frame == 10)
//			std::cout << debug << std::endl;

		if (0 == word){
			std::cout << "PixyCam: No data" << std::endl;
			if (!m_running) break;
			//Update the frame, it's collection of detected objects
			m_blocks = blocks;
			usleep(1000*20);
		} else if (START_BLOCK == word){
			blockNumber = 0;

			if (START_BLOCK == prev_word){
				//Update the frame, its collection of detected objects
				m_blocks = blocks;
				m_frame++;

				blocks.clear();
				std::cout << "Frame: " << m_frame << std::endl;
			}
		} else if (blockNumber == 0){
			blocks.push_back(PixyObject());
			blocks.back().checksum = word;
			blockNumber++;
		} else if (blockNumber == 1){
			blocks.back().sig = word;
			blockNumber++;
		} else if (blockNumber == 2){
			blocks.back().x = word;
			blockNumber++;
		} else if (blockNumber == 3){
			blocks.back().y = word;
			blockNumber++;
		} else if (blockNumber == 4){
			blocks.back().w = word;
			blockNumber++;
		} else if (blockNumber == 5){
			blocks.back().h = word;
			blockNumber++;
		} else if (blockNumber == 6){
			std::cout << "Read an extra word from the pixy cam: " << word << std::endl;
		} else {
			std::cout << "Improperly parsing pixy cam output: " << word << std::endl;
		}

		prev_word = word;
	}//while
	std::cout << "Thread Stopped" << std::endl;
}//ReadData
