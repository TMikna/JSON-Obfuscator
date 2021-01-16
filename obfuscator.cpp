
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>



int main(int argc, char* argv[])
{
//*******---Preparing---************//
    if (argc < 2)
    {
        std::cout << "Please provide input and (optionally) mapping file names";
        return 0;
    }
    const std::string IMPUT_FILE = argv[1];
    const std::string OUTPUT_FILE = "output.json";
    const std::string MAPPING_FILE = argc >= 3 ? argv[2] : "mapping.txt";

    std::ifstream is(IMPUT_FILE);                    // open input file
    if (!is)
    {
        std::cout<<"Failed to open input file: "<< IMPUT_FILE <<"\n";
        return 0;
    }
        
    std::ofstream os(OUTPUT_FILE);                   // open output file

    if (!os)
    {
        std::cout << "Failed to open output file: " << OUTPUT_FILE << "\n";
        return 0;
    }

 //*******---Obfuscating---************//

    bool isString = false;
    std::string word = "";
    std::unordered_set<std::string> words;

    char c;
    while (is.get(c))                                   // loop getting single characters
    {   
        if (c == '"')
        {
            isString = !isString;
            os << c;

            if (isString == false)                       //word finished were
            {
                if (words.find(word) == words.end())     // insert to the list if it's not there yet
                    words.insert(word);     

                word = "";                               // refresh container
            }
        }
        else if(isString)
        { 
            int ic = c;
            os << "\\u00" << std::hex << ic;

            word += c;                                   // Collecting our words for mapping
        }
        else
            os << c;
    }
    is.close();                // close file
    os.close();                // close file

    std::cout << "\nFile obfuscated successfully!" << "\n";
    std::cout << "Obfuscated file name is: " << OUTPUT_FILE << "\n";

//*******---Generating mapping file---************//
    std::ofstream ms(MAPPING_FILE);         // open output file
    if (!ms)                                // check if open
    {
        std::cout << "Failed to open mapping file: " << MAPPING_FILE << "\n";
        return 0;
    }

    for (std::string word : words) 
    {
        ms << word << " -> ";
        std::cout << word << " ";
        for(char letter : word)
        {
            int asciiLetter = letter;
            ms << "\\x" << std::hex << asciiLetter;
        }
        ms << "\n";
    }
    ms.close();   

    std::cout << "\nMapping file created successfully!" << "\n";
    std::cout << "Mapping file name is: " << MAPPING_FILE << "\n";

    return 0;
}