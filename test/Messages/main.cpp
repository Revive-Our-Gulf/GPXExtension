//--------------------------------------------------------
// Main entry point for the development of messages
// 
// @author: Wildboar
//--------------------------------------------------------

#include <fstream>
#include <iostream>
using namespace std;

#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>

//--------------------------------------------------------
// Entry Point
//--------------------------------------------------------

/**
 * Main application entry point
 */
int main(int, char**)
{
    auto distance_url = string("http://192.168.2.2/mavlink2rest/mavlink/vehicles/1/components/194/messages/DISTANCE_SENSOR");
    auto gps_url = string("http://192.168.2.2/mavlink2rest/mavlink/vehicles/1/components/1/messages/GLOBAL_POSITION_INT");
    auto imu_url = string("http://192.168.2.2/mavlink2rest/mavlink/vehicles/1/components/1/messages/RAW_IMU");
    auto yaw_url= string("http://192.168.2.2/mavlink2rest/mavlink/vehicles/1/components/1/messages/ATTITUDE");
    auto heart_url = string("http://192.168.2.2/mavlink2rest/mavlink/vehicles/1/components/1/messages/HEARTBEAT");
    auto altitude_url = string("http://192.168.2.2/mavlink2rest/mavlink/vehicles/1/components/1/messages/ALTITUDE");
    auto controls_url = string("http://192.168.2.2/mavlink2rest/mavlink/vehicles/1/components/1/messages/HIL_CONTROLS");

    auto url = string(controls_url);

    auto writer = ofstream("controls.json");

    writer << curlpp::options::Url(url);
    
    writer.close();

    return EXIT_SUCCESS;
}