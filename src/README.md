## Assignment 4: Peripheral Monitor
The goal of this assignment is to help you understand I2C and to allow you control over input/output ports. Refer to [hy428](https://www.csd.uoc.gr/~hy428/as1.html) home page to find more information about the assignment. 

## How to use git
1. Fork the repo (Follow the instructions in  [Assignments](https://www.csd.uoc.gr/~hy428/assignments.html)) 
2. Clone repo (git clone git@gitlab-csd.datacenter.uoc.gr:csdp1048/as4_monitor.git)
3. **Change** the permissions to **PRIVATE**, using the script "private.py" or by hand.

## Change the permissions
### By hand:

1. Go to Project Settings -> General
2. Visibility
3. Change Project Visibility to Private

Also add the TA in your project

1. Go to Project Settings -> Members
2. Add each TA as a Developer

### Using the script:
- The script requires gitlab lib
```
python3 -m pip install python-gitlab
```

- Run the script 
```
python3 private.py -t <yourToken> -p <ForkedProjectID>
```
## Submission 
- You should submit the **source files** of each assignment. **Do not** submit the toolchain and object or executable file
- Commit all files (not executables or object files!) 
```
git add file1.c file2.c file3.c  
git commit -m "Commit message"  
git push  
```
