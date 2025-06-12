// main.cpp
#include "include/WheelSimulator.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "include/json.hpp"
using json = nlohmann::json;


int main(int argc, char* argv[]) {
    // Process input data
    if (argc != 3) {
        // std::cerr << "Usage: ./WheelSimulator <slip> <sim_endtime> <batch_dir_name> <wheel_path> <terrain_path> <data_path>" << std::endl;
        std::cerr << "Usage: ./WheelSimulator <input_path>" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Could not open " << argv[1] << "\n";
        return 1;
    }

    json param;
    file >> param;

    // Use default values if keys are missing
    float width = param.value("width", 6);
    float rim_radius = param.value("rim_radius", 8.5);
    int grouser_num = param.value("grouser_num", 15);

    // Import slip and batch directory from CLI arguments
    double slip = param.value("slip", 0.2);
    double sim_endtime = param.value("sim_endtime", 5);
    std::string batch_dir = argv[2];
    std::string output_dir = param.value("output_dir", "");

    std::filesystem::path wheel_filepath = param.value("wheel_filepath", "/jet/home/matthies/moonranger_mobility/meshes/wheel.obj");
    std::filesystem::path terrain_filepath = param.value("terrain_filepath", "/jet/home/matthies/moonranger_mobility/terrain/GRC_3e5_Reduced_Footprint.csv");
    std::filesystem::path data_drivepath =  param.value("data_drivepath", "/ocean/projects/mch240013p/matthies/");

    try {
        WheelSimulator simulator(width, rim_radius, grouser_num, slip, sim_endtime, 
                    batch_dir, output_dir, wheel_filepath, terrain_filepath, data_drivepath, param);
        simulator.PrepareSimulation();
        simulator.RunSimulation();
    } catch (const std::exception& e) {
        std::cerr << "Simulation failed: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Simulation completed successfully." << std::endl;
    return EXIT_SUCCESS;
}
/*
parameters for WheelSimulator
width (6cm)                 ---top of json
rim_radius (8.5cm)
slip (0.2)
sim_endtime (10)
batch_dir ("batch_001")
output_dir ("")
wheel_filepath ("wheel.obj")
terrain_filepath ("terrian.idk")
data_drivepath ("/data")            ____ bottom of json
the json
*/