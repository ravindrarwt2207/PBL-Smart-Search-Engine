# SmartSearch – Database & Automation Module

## Overview
This module is part of the Smart Autocomplete and Search Engine project. It manages file-processing tasks and stores document information using an SQLite database.

## Features
- SQLite database connection and table creation.
- Task queue using FIFO (First In, First Out).
- Reading text files.
- Storing filenames and file contents in the database.
- Retrieving stored documents from the database.

## Project Files
- `DatabaseAutomation.h` – Contains class declarations and data structures.
- `DatabaseAutomation.cpp` – Implements database operations and automation logic.
- `test_database.cpp` – Test program for checking the module.

## How It Works
1. Connects to the SQLite database.
2. Creates the documents table if it does not exist.
3. Adds file-processing tasks to a queue.
4. Reads the files and stores their contents in the database.
5. Retrieves stored documents when required.

## Technologies Used
- C++
- SQLite
- Data Structures (Queue)
- File Handling

## Requirements
- C++ compiler with C++17 support.
- SQLite development library and header files.

## Compilation
Example command (adjust SQLite paths according to your system):

```bash
g++ -std=c++17 test_database.cpp DatabaseAutomation.cpp -I<sqlite-include-path> -L<sqlite-library-path> -lsqlite3 -o test_database
```

## Testing
The `test_database.cpp` file is intended to test database connectivity, file processing, and document retrieval. Compilation and execution still need to be verified.

## Project Status
Source files have been created. Compilation and runtime testing are pending.

## Team Contribution
**Module:** Database and Automation  
**Project:** Smart Autocomplete and Search Engine
