## Part 0: Pre-checking: Compilation
  - Passed pre-checking.


------------------------------

## Part 2: Checking Main Task
- The test directory has the following structure and permissions:
    ```
    /home/ubuntu/hw3grading/s23/test_env
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
#### Testing:
- Command:
    ```
    ./a.out  /home/ubuntu/hw3grading/s23/test_env  rw-rw-r--
    ```
- Your output:
    ```
    /home/ubuntu/hw3grading/s23/test_env/.file3.txt
    /home/ubuntu/hw3grading/s23/test_env/file1.2
    /home/ubuntu/hw3grading/s23/test_env/file1.3
    /home/ubuntu/hw3grading/s23/test_env/file1.txt
    /home/ubuntu/hw3grading/s23/test_env/subdir2/subdir3/file3.2
    Total matches: 5
    ```
- Standard output:
    ```
    /home/ubuntu/hw3grading/s23/test_env/.file3.txt
    /home/ubuntu/hw3grading/s23/test_env/file1.2
    /home/ubuntu/hw3grading/s23/test_env/file1.3
    /home/ubuntu/hw3grading/s23/test_env/file1.txt
    /home/ubuntu/hw3grading/s23/test_env/subdir2/subdir3/file3.2
    Total matches: 5
    ```
- Your output (stderr):
    ```
    ```
- Standard output (stderr):
    ```
    ```
[-0] Passed!

#### Testing:
- Command:
    ```
    ./a.out  /home/ubuntu/hw3grading/s23/test_env  ---------
    ```
- Your output:
    ```
    /home/ubuntu/hw3grading/s23/test_env/file2.txt
    /home/ubuntu/hw3grading/s23/test_env/subdir2/subdir3/file3.1
    Total matches: 2
    ```
- Standard output:
    ```
    /home/ubuntu/hw3grading/s23/test_env/file2.txt
    /home/ubuntu/hw3grading/s23/test_env/subdir2/subdir3/file3.1
    Total matches: 2
    ```
- Your output (stderr):
    ```
    ```
- Standard output (stderr):
    ```
    ```
[-0] Passed!

#### Testing:
- Command:
    ```
    ./a.out  /home/ubuntu/hw3grading/s23/test_env  --x--x--x
    ```
- Your output:
    ```
    Total matches: 0
    ```
- Standard output:
    ```
    Total matches: 0
    ```
- Your output (stderr):
    ```
    ```
- Standard output (stderr):
    ```
    ```
[-0] Passed!

#### Testing:
- Command:
    ```
    ./a.out  /home/ubuntu/hw3grading/s23/test_env  --x-w-r--
    ```
- Your output:
    ```
    /home/ubuntu/hw3grading/s23/test_env/subdir2/subdir3/subdir4/file4.1
    /home/ubuntu/hw3grading/s23/test_env/subdir2/subdir3/subdir4/file4.2
    Total matches: 2
    ```
- Standard output:
    ```
    /home/ubuntu/hw3grading/s23/test_env/subdir2/subdir3/subdir4/file4.1
    /home/ubuntu/hw3grading/s23/test_env/subdir2/subdir3/subdir4/file4.2
    Total matches: 2
    ```
- Your output (stderr):
    ```
    ```
- Standard output (stderr):
    ```
    ```
[-0] Passed!

#### Testing:
- Command:
    ```
    ./a.out  /doesnt_exist  rw-rw-r--
    ```
- Your output:
    ```
    Total matches: 0
    ```
- Standard output:
    ```
    ```
- Your output (stderr):
    ```
    Error: Cannot stat '/doesnt_exist'. No such file or directory.
    ```
- Standard output (stderr):
    ```
    Error: Cannot stat '/doesnt_exist'. No such file or directory.
    ```
[-5] Stdout Mismatched!
------------------------------

**Automatic Testing Total: 50 - 5 = 45**
