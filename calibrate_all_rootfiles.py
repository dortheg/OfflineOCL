#A file that runs the calibrations of all the files, with their respective calibration files
from subprocess import Popen, PIPE, call
import numpy as np 

gainfile = open("input_files/SortPu240_d_p.batch", "r")
gainfile_new = open("input_files//SortPu240_d_p_new.batch", "w")

data_files = np.genfromtxt("data_file_list_py_reading.dat", skiprows=6, usecols=1)

line_counter = 0
for line in gainfile:
	if line_counter == 2:
		words = []
		words.append("Gain file /Applications/OfflineOCL/Mine/OfflineOCL/input_files/Calib_files_all_detectors/") 
		words.append("total_gain_shift_20180416_173844.dat")
		str1 = ''.join(str(e) for e in words)
		str1 = str1 + "\n"
		gainfile_new.write(str1)

	else:
		gainfile_new.write(line)
	line_counter += 1

gainfile.close()
gainfile_new.close()