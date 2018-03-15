#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector> 
#include <sys/stat.h>
#include <cassert>
#include <bitset>
/*
  This code is derived from LZW@RosettaCode for UA CS435 
*/ 
 
// Compress a string to a list of output symbols.
// The result will be written to the output iterator
// starting at "result"; the final iterator is returned.
template <typename Iterator>
Iterator compress(const std::string &uncompressed, Iterator result) {
  // Build the dictionary.
  int dictSize = 256;
  std::map<std::string,int> dictionary;
  for (auto i = 0; i < 256; i++)
    dictionary[std::string(1, i)] = i;
 
  std::string w;
  for (std::string::const_iterator it = uncompressed.begin(); it != uncompressed.end(); ++it) {
    char c = *it;
    std::string wc = w + c;
    if (dictionary.count(wc))
      w = wc;
    else {
      *result++ = dictionary[w];
      // Add wc to the dictionary. Assuming the size is 4096!!!
      if (dictionary.size()<4096)
         dictionary[wc] = dictSize++;
      w = std::string(1, c);
    }
  }
 
  // Output the code for w.
  if (!w.empty())
    *result++ = dictionary[w];
  return result;
}
 
// Decompress a list of output ks to a string.
// "begin" and "end" must form a valid range of ints
template <typename Iterator>
std::string decompress(Iterator begin, Iterator end) {
  // Build the dictionary.
  int dictSize = 256;
  std::map<int,std::string> dictionary;
  for (int i = 0; i < 256; i++)
    dictionary[i] = std::string(1, i);
 
  std::string w(1, *begin++);
  std::string result = w;
  //std::cout << result<<"???:::\n";
  std::string entry;
  for ( ; begin != end; begin++) {
    int k = *begin;
    if (dictionary.count(k))
      entry = dictionary[k];
    else if (k == dictSize)
      entry = w + w[0];
    else
      throw "Bad compressed k";
 
    result += entry;
 
    // Add w+entry[0] to the dictionary.
    if (dictionary.size()<4096)
      dictionary[dictSize++] = w + entry[0];
 
    w = entry;
  }
  return result;
}

std::string int2BinaryString(int c, int cl) {
      std::string p = ""; //a binary code string with code length = cl
      int code = c;
      while (c>0) {         
		   if (c%2==0)
            p="0"+p;
         else
            p="1"+p;
         c=c>>1;   
      }
      int zeros = cl-p.size();
      if (zeros<0) {
         std::cout << "\nWarning: Overflow. code " << code <<" is too big to be coded by " << cl <<" bits!\n";
         p = p.substr(p.size()-cl);
      }
      else {
         for (int i=0; i<zeros; i++)  //pad 0s to left of the binary code if needed
            p = "0" + p;
      }
      return p;
}

int binaryString2Int(std::string p) {
   int code = 0;
   if (p.size()>0) {
      if (p.at(0)=='1') 
         code = 1;
      p = p.substr(1);
      while (p.size()>0) { 
         code = code << 1; 
		   if (p.at(0)=='1')
            code++;
         p = p.substr(1);
      }
   }
   return code;
}


int main(int argc, char *argv[] ) {


	//Throw error if not enough inputs given to program
	if(argc <2){
		std::cerr<<"Error: Not Enough arguments passed in \n";
		return 1;
	}

	std::string inFileName = argv[2];
	std::ifstream inFile(inFileName.c_str(), std::ios::binary);
	std::string input((std::istreambuf_iterator<char>(inFile)),std::istreambuf_iterator<char>());
	
	inFile.close();

	try{
		switch(*argv[1]){
			
			case 'c': {
				std::vector<int> writeOutVector;
				compress(input,std::back_inserter(writeOutVector));
				std::cout<<"Size of vector for bible.txt : "<<writeOutVector.size()<<"\n";
				//copy(writeOutVector.begin(), writeOutVector.end(), std::ostream_iterator<int>(std::cout, ","));
				std::cout << std::endl;
				
				//Taking the vector and coverting each element into binary
				std::string out;
				for(int i =0; i < writeOutVector.size(); i++){

					out.append(int2BinaryString(writeOutVector[i],12));
				}
		
		
				int bitLen = out.length() % 8;
				out.append(bitLen, '0');

				assert(out.length() == (out.length() % 8 + out.length()));
			

				std::string segment;
				//int newChar;
				//std::string out2 = out;
				for(int i = 0; i<out.size(); i++){
		
					segment = out.substr(i,8);
					//int newChar;
					out.replace(i,8,1,(char)binaryString2Int(segment));
				}
			 
				inFileName.append(".lzw");
				std::ofstream outFile(inFileName.c_str(),std::ios::binary);
				outFile<<out;
				outFile.close();	
				break;
			}
			case 'e': {
				
				std::string s; 
				for(int i = 0; i<input.length(); i++){
					//std::bitset<8>b(static_cast<int>(input.at(i)));
					std::bitset<8> bits((int)(input[i]));
					s.append(bits.to_string());
				}
				std::cout<<"Input length: " << input.length()<<"\n";
				std::cout<<"S : "<< s.size()<< "\n";
				int test =0;
				input = s;
				std::vector<int> vec;
				int zeros = 4;
				//assert((input.length() - zeros) % 12 == 0);
				for( int i = 0;i< input.size() - zeros; i+=12){
					std::string strin = input.substr(i,12);
					//std::cout<<strin<<"\n";
					assert(strin.length() == 12);
					vec.push_back(binaryString2Int(strin));
					//std::cout<<test<<"\n";
					//test++;
				}
			
				std::string d = decompress(vec.begin(), vec.end());
				inFileName.append("2");
				std::ofstream out(inFileName.c_str(), std::ios::binary);
				out<<d;
				out.close();
				

				break;

			}
		}
	}
	catch(char const *err){
		std::cout<<"The Library threw an exception: \n"<<err<<"\n";
	}
  
  return 0;
}
