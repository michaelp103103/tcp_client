See comments in tcp_client.c

To test the code using the sample imagery provided with openstartracker, run openstartracker using the following line:

      python3 startracker.py tests/science_cam_may8_0.05sec_gain40/calibration.txt 1991.25 tests/science_cam_may8_0.05sec_gain40/median_image.png

Then, run the client program and pass in your Python script as an argument. Assuming the program is compiled to "tcp_client" and the Python script is located in same directory and called "test.py":

      ./tcp_client test.py
