#include "lsl_cpp.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <chrono>
#include <thread>
#include <cassert>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

/**
* This example program offers an 8-channel stream, float-formatted, that resembles EEG data.
* The example demonstrates also how per-channel meta-data can be specified using the .desc() field of the stream information object.
*
* Note that the timer used in the send loop of this program is not particularly accurate.
*/


const char *channels[] = {"imu1_Ax","imu1_Ay","imu1_Az","imu1_Gx","imu1_Gy","imu1_Gz",
                          "imu2_Ax","imu2_Ay","imu2_Az","imu2_Gx","imu2_Gy","imu2_Gz",
                          "imu3_Ax","imu3_Ay","imu3_Az","imu3_Gx","imu3_Gy","imu3_Gz"};

int main(int argc, char* argv[]) {
    string name, type;
    if (argc != 3) {
        cout << "This opens a stream under some user-defined name and with a user-defined content type." << endl;
        cout << "Please enter the stream name and the stream type (e.g. \"BioSemi EEG\" (without the quotes)):" << endl;
        cin >> name >> type;
    } else {
        name = argv[1];
        type = argv[2];
    }

    ifstream inFreq("/sys/bus/iio/devices/iio:device1/in_accel_sampling_frequency");
    ifstream AScale("/sys/bus/iio/devices/iio:device1/in_accel_scale");
    ifstream GScale("/sys/bus/iio/devices/iio:device1/in_anglvel_scale");
    //cout.precision(4);
    int freq;
    inFreq >> freq;
    float accScale, gyroScale;
    AScale >> accScale;
    GScale >> gyroScale;
    inFreq.close();
    AScale.close();
    GScale.close();

    try {

        // make a new stream_info (100 Hz)
        lsl::stream_info info(name,type,18,100,lsl::cf_float32,string(name)+=type);

        // add some description fields
        info.desc().append_child_value("manufacturer", "Bosch");
        lsl::xml_element chns = info.desc().append_child("channels");
        for (int u=0; u<3; u++) {
            for (int k = u * 6; k < u * 6 + 3; k++)
                chns.append_child("channel")
                        .append_child_value("label", channels[k])
                        .append_child_value("unit", "mPs2")
                        .append_child_value("type", "ACC");
            for (int k = u * 6 + 3; k < u * 6 + 6; k++)
                chns.append_child("channel")
                        .append_child_value("label", channels[k])
                        .append_child_value("unit", "radPs")
                        .append_child_value("type", "GYRO");
        }

        // make a new outlet
        lsl::stream_outlet outlet(info);

        // send data forever
        cout << "Now sending data... " << endl;
        double starttime = ((double)clock())/CLOCKS_PER_SEC;
        for(unsigned t=0;;t++) {

            // wait a bit and create random data
            while (((double)clock())/CLOCKS_PER_SEC < starttime + t*0.036); // set the data frequency (now 100Hz)
            float sample[18];
	    /*
            for (int c=0;c<18;c++){
                sample[c] = (float)((rand()%1500)/500.0-1.5)*5*((c%6)+1) + 20*(c%6);
                std::cout << sample[c] << " ";
            }
	    */
	    ifstream imu1_Ax("/sys/bus/iio/devices/iio:device1/in_accel_x_raw");
	    ifstream imu1_Ay("/sys/bus/iio/devices/iio:device1/in_accel_y_raw");
	    ifstream imu1_Az("/sys/bus/iio/devices/iio:device1/in_accel_z_raw");
	    ifstream imu1_Gx("/sys/bus/iio/devices/iio:device1/in_anglvel_x_raw");
	    ifstream imu1_Gy("/sys/bus/iio/devices/iio:device1/in_anglvel_y_raw");
	    ifstream imu1_Gz("/sys/bus/iio/devices/iio:device1/in_anglvel_z_raw");
	    ifstream imu2_Ax("/sys/bus/iio/devices/iio:device2/in_accel_x_raw");
	    ifstream imu2_Ay("/sys/bus/iio/devices/iio:device2/in_accel_y_raw");
	    ifstream imu2_Az("/sys/bus/iio/devices/iio:device2/in_accel_z_raw");
	    ifstream imu2_Gx("/sys/bus/iio/devices/iio:device2/in_anglvel_x_raw");
	    ifstream imu2_Gy("/sys/bus/iio/devices/iio:device2/in_anglvel_y_raw");
	    ifstream imu2_Gz("/sys/bus/iio/devices/iio:device2/in_anglvel_z_raw");
	    ifstream imu3_Ax("/sys/bus/iio/devices/iio:device3/in_accel_x_raw");
	    ifstream imu3_Ay("/sys/bus/iio/devices/iio:device3/in_accel_y_raw");
	    ifstream imu3_Az("/sys/bus/iio/devices/iio:device3/in_accel_z_raw");
	    ifstream imu3_Gx("/sys/bus/iio/devices/iio:device3/in_anglvel_x_raw");
	    ifstream imu3_Gy("/sys/bus/iio/devices/iio:device3/in_anglvel_y_raw");
	    ifstream imu3_Gz("/sys/bus/iio/devices/iio:device3/in_anglvel_z_raw");
	    imu1_Ax >> sample[0];
	    imu1_Ay >> sample[1];
	    imu1_Az >> sample[2];
	    imu1_Gx >> sample[3];
	    imu1_Gy >> sample[4];
	    imu1_Gz >> sample[5];
	    imu2_Ax >> sample[6];
	    imu2_Ay >> sample[7];
	    imu2_Az >> sample[8];
	    imu2_Gx >> sample[9];
	    imu2_Gy >> sample[10];
	    imu2_Gz >> sample[11];
	    imu3_Ax >> sample[12];
	    imu3_Ay >> sample[13];
	    imu3_Az >> sample[14];
	    imu3_Gx >> sample[15];
	    imu3_Gy >> sample[16];
	    imu3_Gz >> sample[17];
	    imu1_Ax.close();
	    imu1_Ay.close();
	    imu1_Az.close();
	    imu1_Gx.close();
	    imu1_Gy.close();
	    imu1_Gz.close();
	    imu2_Ax.close();
	    imu2_Ay.close();
	    imu2_Az.close();
	    imu2_Gx.close();
	    imu2_Gy.close();
	    imu2_Gz.close();
	    imu3_Ax.close();
	    imu3_Ay.close();
	    imu3_Az.close();
	    imu3_Gx.close();
	    imu3_Gy.close();
	    imu3_Gz.close();

	    for(int i=0; i<3; i++){
		for(int j=i*6; j<i*6+3; j++)
	            sample[j] *= accScale;
	        for(int j=i*6+3; j<i*6+6; j++)
		    sample[j] *= gyroScale;
	    }

	    for(int c=0; c<18; c++)
		std::cout << sample[c] << " ";

            std::cout << std::endl;                                               
                                                                                  
            // send the sample                                                    
            outlet.push_sample(sample);                                          
        }                                                                         

    } catch(std::exception &e) {
        cerr << "Got an exception: " << e.what() << endl;
    }
    cout << "Press any key to exit. " << endl; cin.get();
    return 0;
}
