#include "lsl_cpp.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iostream>
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
            while (((double)clock())/CLOCKS_PER_SEC < starttime + t*0.01); // set the data frequency (now 100Hz)
            float sample[18];
            for (int c=0;c<18;c++){
                sample[c] = (float)((rand()%1500)/500.0-1.5)*5*((c%6)+1) + 20*(c%6);
                std::cout << sample[c] << " ";
            }
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