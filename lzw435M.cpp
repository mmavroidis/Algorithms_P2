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
      if (dictionary.size()<65536)
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
    if (dictionary.size()<65536)
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
	//Input File IO 
	std::string inFileName = argv[2];
	std::ifstream inFile(inFileName.c_str(), std::ios::binary);
	std::string input((std::istreambuf_iterator<char>(inFile)),std::istreambuf_iterator<char>());
	
	inFile.close();

	try{
		switch(*argv[1]){
			
			case 'c': {
				std::cout<<"Compressing.... \n";
				//Vector we are writing into from the input
				std::vector<int> writeOutVector;
				compress(input,std::back_inserter(writeOutVector));
				//Original bits needed as well as num of elems
				int b = 9; 
				int num = 256;
				std::string compStr;
				std::string temp;
				for(std::vector<int>::iterator iter = writeOutVector.begin(); iter != writeOutVector.end();iter++){
					//Handle Original ASCII Table and up to 512, then grabs number of bits based on the size 
					temp = int2BinaryString(*iter, b);
					num++;

				switch(num){
					case 512: 
						b =10;
						break;
					case 1024:
						b = 11;
						break;
					case 2048:
						b = 12;
						break; 
					case 4096:
						b =13;
						break;
					case 8192:
						b =14;
						break;
					case 16384:
						b = 15;
						break;
					case 32768:
						b = 16;
						break;

				}

					compStr+=temp;
				}

	
			//bin2IntString
			int remainder = compStr.length() % 8;
			compStr.append( 8 -remainder, '0');
			std::string str2; 
			
			for(int i =0; i< compStr.size(); i++){

			str2 = compStr.substr(i,8);
			compStr.replace(i,8,1,(char)binaryString2Int(str2));
			
			}

				//File output			 
				inFileName.append(".lzw");
				std::ofstream outFile(inFileName.c_str(),std::ios::binary);
				outFile<<compStr;
				outFile.close();	
				std::cout<<"Done! \n";
				break;
			}
			case 'e': {
				std::cout<<"Expanding.... \n";
				//Original bit size as well as num of elems in table					
				int b = 9;
				int num = 256;

				std::string str;
				std::vector<int> writeOutVector;

				//Binary string and string to binary string
				std::string BinString;
				std::string s2BinString;
			
				//Bits to string 
				for(int i = 0; i<input.length(); i++){
				
					std::bitset<8> bits((int)(input[i]));
					s2BinString.append(bits.to_string());
				}


				BinString = s2BinString;
				
				for(int i = 0; i<BinString.length(); i++){

					str+= BinString[i];
					if(str.length() >= b){
						writeOutVector.push_back(binaryString2Int(str));
						num++;
						
						switch(num){
							case 512:
								b = 10;
								break;
							case 1024:
								b = 11;	
								break;
							case 2048: 
								b = 12;
								break;
							case 4096:
								b = 13;
								break;
							case 8192:
								b = 14;
								break; 
							case 16384: 
								b = 15;
								break;
							case 32768:
								b =16;
								break; 		
						}
					str.clear();
					}
				}
				//Output IO	
				std::string d = decompress(writeOutVector.begin(),writeOutVector.end());
				inFileName.append("2");
				std::ofstream out(inFileName.c_str(), std::ios::binary);
				out<<d;
				out.close();	
				std::cout<<"Done! \n";
				break;

			}
		}
	}
	catch(char const *err){
		std::cout<<"The Library threw an exception: \n"<<err<<"\n";
	}
  
  return 0;
}
