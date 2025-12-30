
#include <fstream>
#include <unordered_set>
#include <iostream>

struct parameters
{
    std::string sensor_type;
    std::string resolution;
    int total_frames;
    int failed_frames;
    std::string error_types;
}; 

int main()
{
    // std::unordered_set<parameters> simulations;
    
    std::ifstream inFile;
    inFile.open("example.txt");

    std::string line; 

    std::string subString1 = "started";
    std::string subString2 = "failed";
    std::string subString2 = "completed";


    std::string sensor_type = "sensor_type=";

    while (std::getline(inFile, line))
    {
        std::cout << line << std::endl;

        if (line.find(subString1) >=0) //SIMULATION STARTED
        {
            parameters simul;
            std::size_t start = line.find("sensor_type=") + std::string("sensor_type=").length();

             // Find the position of the comma after that
             std::size_t end = line.find(',', start);

                // Extract the substring between them
                std::string sensor_type = line.substr(start, end - start);

             std::cout << "Sensor type: " << sensor_type << std::endl;

             simul.sensor_type = sensor_type;

        }        
        
     

        // simulations.insert(p1);
    }


    return 0;
}