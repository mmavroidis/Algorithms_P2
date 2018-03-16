Read me for Project 2 - LZW 
By: Mavroidis Mavroidis 

Part 1: 
	Implemented LZW compression with the provided code to handle fixed length compresison
Part 2:
	Is similar to part 1 but instead of having a fixed length, we have a dynamic length.
	This handles the bits between 9 and 16 bits.
_________________________________________________________________________________________
Directions for Compiling and Running
Part 1:
	Compile using the following:
		g++ -std=c++11 lzw435.cpp -o lzw

	Run Using Following
		Compress:
			./lzw c filename.txt
		Expand: 
			./lzw e filename.txt.lzw

Part 2:
	Compile using the following:
			g++ -std=c++11 lzw435M.cpp -o lzwM
	
	Run Using Following:
		Compress: 
			./lzwM c filename.txt
		Expand: 
			./lzwM e filename.txt.lzw 
_________________________________________________________________________________________
Notes:
-Files once they are compressed have the ".lzw" extension meaning they are compressed using LZW. 
-Files once they are expanded have the ".lzw2" extension meaning they are expanded.
-Trying to compress larger files such as bible.txt or monkey.gif, can take up to a minute or 2 to
compress. 
-There will be output to the console denoting what is happening like if it is compressiong or when it 
is finished to let the user know that it is not stuck. 
-You can check the sizes of the files using the ll command in bash
	- ll file.txt
-To check to see if the original file and compressed and expanded file are the same use diff
	-diff file.txt file.txt.lzw2
