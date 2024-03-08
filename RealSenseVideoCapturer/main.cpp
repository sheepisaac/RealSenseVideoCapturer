#include <iostream>
#include <fstream>
#include <librealsense2/rs.hpp> 

using namespace std;
using namespace rs2;

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[]) {
	if (argc == 2 
		&& strcmp(argv[1], "-h") == 0) {
		cout << "-w: width of frame" << endl <<
			"-h: height of frame" << endl <<
			"-f: totla frames" << endl <<
			"-fr: frame rate" << endl;
	}
	else if (argc == 9 
		&& strcmp(argv[1], "-w") == 0
		&& strcmp(argv[3], "-h") == 0
		&& strcmp(argv[5], "-f") == 0
		&& strcmp(argv[7], "-fr") == 0) {
		int width = atoi(argv[2]);
		int height = atoi(argv[4]);
		int numberOfFrames = atoi(argv[6]);
		int frameRate = atoi(argv[8]);

		pipeline pipe;

		cout << "Start to set cfg of stream..." << endl;
		config cfg;
		cfg.enable_stream(RS2_STREAM_COLOR, width, height, RS2_FORMAT_RGB8, frameRate);
		
		cout << "Start to set profile of pipeline..." << endl;
		pipeline_profile profile = pipe.start(cfg);

		cout << "Start to set device profile..." << endl;
		rs2::device device = profile.get_device();

		cout << "Start to set file path..." << endl;
		string filePath = "./bin/captured_video";
		ofstream outfile_texture(filePath + "_texture.rgb", ios::binary);

		cout << "Start to record video and write rgb file..." << endl;
		for (size_t frameIndex = 0; frameIndex < numberOfFrames; ++frameIndex) {
			frameset frames = pipe.wait_for_frames();
			frame color_frame = frames.get_color_frame();
			
			const auto color_data = reinterpret_cast<const uint8_t*>(color_frame.get_data());
			outfile_texture.write(reinterpret_cast<const char*>(color_data), width * height * 3);
		}

		cout << "Finished to write rgb file :)" << endl;
		
		outfile_texture.close();

		cout << "Stop pipeline" << endl;
		pipe.stop();

	}
	else {
		cout << "Wrong argument." << endl;
	}

	return 0;
}