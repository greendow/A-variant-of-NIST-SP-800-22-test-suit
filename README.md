## Introduction  
  &ensp;&ensp;&ensp;&ensp;A set of statistical tests for randomness is described in **NIST SP 800-22 Revision 1a** *A Statistical Test Suite for Random and Pseudorandom Number Generators for Cryptographic Applications*. The **NIST Statistical Test Suit** is a software tool released by NIST. The document and the software can be found on the website: [https://csrc.nist.gov/Projects/Random-Bit-Generation/Documentation-and-Software](https://csrc.nist.gov/Projects/Random-Bit-Generation/Documentation-and-Software) . A variant of NIST Statistical Test Suit is presented here. The differences are listed in the table below:  

|                                             | NIST Test Suit (version 2.1.2)                                                                                                                                                                                                                 | The variant                                                                                         |
| ------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------- |
| suported OS                                 | Linux only                                                                                                                                                                                                                                     | Linux, Windows                                                                                      |
| funcitons                                   | generating random number,  testing random number                                                                                                                                                                                               | testing random number only                                                                          |
| result display                              | The results are not in the finalAnalysisReport.txt file. They are in the results.txt files in the individual test directories (when running the sample data files these test directories are in the directory experiments/AlgorithmTesting/) . | The results are gathered up and displayed on the standard output device (*stdout*).                 |
| input data file format                      | in ASCII format, in binary format                                                                                                                                                                                                              | in binary format only                                                                               |
| the length in bits of a block in some tests | fixed value, hard-coded in the source files                                                                                                                                                                                                    | can be set in *config.json* file                                                                    |
| global variable                             | A global variable *epsilon* is used.                                                                                                                                                                                                           | The global variable is not used. Using global variable should be avoided.                           |
| variable naming                             | Names of some variables are hard to understand, such as *n*, *M*.                                                                                                                                                                              | Some variables are renamed. The new names are easy to understand, such as *blockLen*, *blockCount*. |
| function declaration                        | Not all functions are declared in header files.                                                                                                                                                                                                | Function declarations and relative header files have been added.                                    |
| input length check                          | absence                                                                                                                                                                                                                                        | Input length check has been added.                                                                  |
  
---  
## Set parameter values  
  &ensp;&ensp;&ensp;&ensp;Some parameter values can be set in *config.json* file. They are listed in the table below:  
  
| Parameter values            | Test                                   | Range (count in bits)                           |
| --------------------------- | -------------------------------------- | ----------------------------------------------- |
| the length of each block    | Frequency Test within a Block          | [2, *n*] (*n* is the length of the bit string.) |
| the length of each template | Non-overlapping Template Matching Test | [2, 21]                                         |
| the length of the template  | Overlapping Template Matchings Test    | NIST recommends m = 9 or m = 10.                |
| the length of each block    | Linear Complexity Test                 | [500, 5000]                                     |
| the length of each block    | Serial Test                            | [3, floor(ln(n) / ln(2)) - 2)                   |
| the length of each block    | Approximate Entropy Test               | [2, floor(ln(n) / ln(2)) - 5)                   |
---
## Compile  
- with Visual Studio on Windows:  
  nmake -f Makefile_for_VS.txt  
- with GCC on Linux:  
  make -f Makefile_for_Linux.txt  
## Run
- run the executable file **rnd_test**:  
  rnd_test *random_number_file*  
  e.g.  **rnd_test data.sha1** (on Windows) or **./rnd_test data.sha1** (on Linux)  
- Tip: the output can be re-directed to a file by using pipe command:  
  e.g.  **rnd_test data.sha1 > log.txt** (on Windows) or **./rnd_test data.sha1 > log.txt** (on Linux)  
- Note:  
  1. The executable file *rnd_test* and the folder *templates* must be put into the same directory.  
  2. *data.sha1* is a sample data file included in folder *sts-2.1.2\data* of *NIST Statistical Test Suit*. It is in binary format.
  3. The recommended length of the input data file in NIST SP 800-22 Revision 1a is 1,000,000 bits (i.e. about 122K Bytes). If the length is too small or to big, some tests cannot be performed. And even if the length is about 1,000,000 bits, some tests cannot be performed occasionally.
---  
## Screenshot  
  &ensp;&ensp;&ensp;&ensp;Partial results:  
  ![ScreenShot01](https://github.com/greendow/A-variant-of-NIST-SP-800-22-test-suit/blob/master/ScreenShot_1.png)
    
  ![ScreenShot02](https://github.com/greendow/A-variant-of-NIST-SP-800-22-test-suit/blob/master/ScreenShot_2.png)  
---  
## License  
&ensp;&ensp;&ensp;&ensp;This work is a compound of revised NIST Statistical Test Suit (v2.1.2), cJSON (v1.7.12) and few files written by myself. It's hard to find a common license suitable for it. Maybe distinct licenses should be used for specific files.  

| File                                                    | License                                                                                                 |
| ------------------------------------------------------- | ------------------------------------------------------------------------------------------------------- |
| files copied or revised from NIST Statistical Test Suit | software disclaimer on: https://csrc.nist.gov/Projects/Random-Bit-Generation/Documentation-and-Software |
| cJSON files                                             | MIT License, claimed on: https://github.com/DaveGamble/cJSON                                            |
| few files written by myself                             | MIT License (https://mit-license.org/), marked at the beginning of those files                          |
