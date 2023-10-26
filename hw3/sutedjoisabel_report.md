## Part 0: Pre-checking: Compilation
  - Passed pre-checking.


------------------------------

## Part 1: Checking Permission String Handling
- 5 test cases, 6 points each (30 in total);
- For each case:
  - If printing to stdout not stderr: 3 points off;
  - If time out (couldn't finish your program under 5 seconds): 6 points off;
  - If wrong output or no output at all: 6 points off.

#### Testing: Empty string
- Command:
    ```
    ./a.out  /  
    ```
- Your output:
    ```
    Error: Permissions string '' is invalid.
    
    ```
- **[-0] Passed!**

#### Testing: One character string
- Command:
    ```
    ./a.out  /  x
    ```
- Your output:
    ```
    Error: Permissions string 'x' is invalid.
    
    ```
- **[-0] Passed!**

#### Testing: Multi-character string
- Command:
    ```
    ./a.out  /  83@**#ads
    ```
- Your output:
    ```
    Error: Permissions string '83@**#ads' is invalid.
    
    ```
- **[-0] Passed!**

#### Testing: Super long random string
- Command:
    ```
    ./a.out  /  heNijzSfOeDCScLkBlBNTtefGGPjTBDeYDYorvhwCEgxgXRBkJQtmmsrRjdIGOhZZlxEUyiboyySPbzQGuOkXcDRUNeqgzzZoVuw
    ```
- Your output:
    ```
    Error: Permissions string 'heNijzSfOeDCScLkBlBNTtefGGPjTBDeYDYorvhwCEgxgXRBkJQtmmsrRjdIGOhZZlxEUyiboyySPbzQGuOkXcDRUNeqgzzZoVuw' is invalid.
    
    ```
- **[-0] Passed!**

#### Testing: String with correct bits but wrong order
- Command:
    ```
    ./a.out  /  xrw-xr---
    ```
- Your output:
    ```
    Error: Permissions string 'xrw-xr---' is invalid.
    
    ```
- **[-0] Passed!**



------------------------------

## Part 2: Checking Main Task
- The 1st test directory has the following structure and permissions:
    ```
    /home/ubuntu/hw3grading//s23/test_env
    ├── [-rw-rw-r--]  .file3.txt
    ├── [-rw-rw-r--]  file1.2
    ├── [-rw-rw-r--]  file1.3
    ├── [-rw-rw-r--]  file1.txt
    ├── [----------]  file2.txt
    ├── [drwxrwxr-x]  subdir1
    └── [drwxrwxr-x]  subdir2
        └── [drwxrwxr-x]  subdir3
            ├── [----------]  file3.1
            ├── [-rw-rw-r--]  file3.2
            └── [drwxrwxr-x]  subdir4
                ├── [---x-w-r--]  file4.1
                └── [---x-w-r--]  file4.2
    
    4 directories, 9 files
    ```
- The 2nd test directory, `test_env2`, has the following permissions:
  `-wx-wx--x`
- The 3rd test directory, `test_env3`, has the following permissions:
  `---------`

- 10 test cases, 5 points each (50 in total);
- For each case:
  - If printing destination is wrong (stdout,stderr): 1 point off;
  - If not all files have been found: 5 points off;
  - If timeout after 5 seconds: 5 points off;
  - If prints to both stderr and stdout: 3 point off.

#### Testing:
- Command:
    ```
    ./a.out  /home/ubuntu/hw3grading//s23/test_env  rw-rw-r--
    ```
- Your ouput:
    ```
    ```
- Correct output:
    ```
    /home/ubuntu/hw3grading/s23/test_env/file1.2
    /home/ubuntu/hw3grading/s23/test_env/file1.3
    /home/ubuntu/hw3grading/s23/test_env/subdir2/subdir3/file3.2
    /home/ubuntu/hw3grading/s23/test_env/.file3.txt
    /home/ubuntu/hw3grading/s23/test_env/file1.txt
    ```
- **[-5] Wrong output & destination!**

#### Testing:
- Command:
    ```
    ./a.out  /home/ubuntu/hw3grading//s23/test_env  rw-r--r--
    ```
- Your ouput:
    ```
    ```
- Correct output:
    ```
    ```
- **[-5] Wrong output!**

#### Testing:
- Command:
    ```
    ./a.out  /home/ubuntu/hw3grading//s23/test_env  ---------
    ```
- Your ouput:
    ```
    ```
- Correct output:
    ```
    /home/ubuntu/hw3grading/s23/test_env/subdir2/subdir3/file3.1
    /home/ubuntu/hw3grading/s23/test_env/file2.txt
    ```
- **[-5] Wrong output & destination!**

#### Testing:
- Command:
    ```
    ./a.out  /home/ubuntu/hw3grading//s23/test_env  --x--x--x
    ```
- Your ouput:
    ```
    ```
- Correct output:
    ```
    ```
- **[-5] Wrong output!**

#### Testing:
- Command:
    ```
    ./a.out  /home/ubuntu/hw3grading//s23/test_env  --x-w-r--
    ```
- Your ouput:
    ```
    ```
- Correct output:
    ```
    /home/ubuntu/hw3grading/s23/test_env/subdir2/subdir3/subdir4/file4.1
    /home/ubuntu/hw3grading/s23/test_env/subdir2/subdir3/subdir4/file4.2
    ```
- **[-5] Wrong output & destination!**

#### Testing:
- Command:
    ```
    ./a.out  /home/ubuntu/hw3grading//s23/test_env  rwxrwxrwx
    ```
- Your ouput:
    ```
    ```
- Correct output:
    ```
    ```
- **[-5] Wrong output!**

#### Testing:
- Command:
    ```
    ./a.out  /home/ubuntu/hw3grading//s23/test_env  r-x-w-r-x
    ```
- Your ouput:
    ```
    ```
- Correct output:
    ```
    ```
- **[-5] Wrong output!**

#### Testing:
- Command:
    ```
    ./a.out  /home/ubuntu/hw3grading//s23/test_env2  rwxrwxrwx
    ```
- Your ouput:
    ```
    Cannot open directory '/home/ubuntu/hw3grading/grader//home/ubuntu/hw3grading//s23/test_env2'. Permission denied.
    ```
- Correct output:
    ```
    Error: Cannot open directory '/home/ubuntu/hw3grading/s23/test_env2'. Permission denied.
    ```
- **[-5] Wrong output!**

#### Testing:
- Command:
    ```
    ./a.out  /home/ubuntu/hw3grading//s23/test_env2  -wx-wx--x
    ```
- Your ouput:
    ```
    Cannot open directory '/home/ubuntu/hw3grading/grader//home/ubuntu/hw3grading//s23/test_env2'. Permission denied.
    ```
- Correct output:
    ```
    Error: Cannot open directory '/home/ubuntu/hw3grading/s23/test_env2'. Permission denied.
    ```
- **[-5] Wrong output!**

#### Testing:
- Command:
    ```
    ./a.out  /home/ubuntu/hw3grading//s23/test_env3  ---------
    ```
- Your ouput:
    ```
    Cannot open directory '/home/ubuntu/hw3grading/grader//home/ubuntu/hw3grading//s23/test_env3'. Permission denied.
    ```
- Correct output:
    ```
    Error: Cannot open directory '/home/ubuntu/hw3grading/s23/test_env3'. Permission denied.
    ```
- **[-5] Wrong output!**
------------------------------

**Automatic Testing Total: 80 - 50 = 30**
