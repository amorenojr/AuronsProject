// AuronsProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "huffman.h"

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


int main()
{
    // Note, I hard coded the input and output args
    char* infile = (char*)"C:\\Users\\amorenojr\\source\\repos\\AuronsProject\\AuronsProject\\small.txt";
    char* outfile = (char*)"C:\\Users\\amorenojr\\source\\repos\\AuronsProject\\AuronsProject\\small.out";
    //char* infile;
    //char* outfile;
    FILE* in;
    FILE* out;

    //if (argc != 3)
    //{
    //    printf("wrong number of args\n");
    //    return 1;
    //}

    //infile = argv[1];
    //outfile = argv[2];

    in = fopen(infile, "rb");
    if (in == NULL)
    {
        printf("couldn't open %s for reading\n", infile);
        return 2;
    }

    out = fopen(outfile, "wb");
    if (out == NULL)
    {
        printf("couldn't open %s for writing\n", outfile);
        return 3;
    }

    encodeFile(in, out);

    fclose(in);
    fclose(out);

    return 0;

}

